### To-Dos)
- Check the architecture of current video models.
  - Are they really using cube of $`H\times W\times T`$ as the patch?
  - Is the fact that they consider $`T_i`$ as a chunk the real problem?
    - Did somebody prove this?
    - Can you?
- Find ways to generate video in ODE way
  - e.g.)
    - Suppose a first frame image and a text prompt is given.
    - We may generate a final frame.
      - i.e.) The deterministic end point!
    - Fill in the intermediate points using the recursive method that we trained!
- Can COT involves in this model?
  - Describing the intermediate state using language.
- Possible Limit
  - What if the training video data is repetitive?
    - e.g.) Watching a clock from Jan 1st 00:00 till Jan 5th 00:00?
    - Sol?)
      - Pre processing by...
      - What if we hash all the frames and find patterns first.
      - If a repeating pattern exists, we may partition and consider only the unique part.

<br>

### Goal)
- Continuous time consideration
  - Existing attention models train attention between patches where
    - $`\text{Patch} = H\times W\times T`$
  - Here, the relation between $`T_i,T_j`$ is learned but the mechanism within $`T_i`$ is not considered.
  - This may cause awkward video generation.
  - However, training every frame regressively may be too expensive.
  - What if we divide and conquer the time?
- Combination of the global and local trend in the video


### Methodology)
- Training
  - Recursively divide and conquer the time axis
  - Limit the depth of search
    - If the start and the end frame are too similar stop the recursive learning.
    - Or, we could set the maximum depth
  - Cross attention between two frames
    - When learning the transition between two frames, use cross attention
  - Add time-embedding
    - The learning between the long-term trend (e.g. frame 1 and frame 100) should be treated differently from the instant shift (e.g. frame 1 and frame 2).

#### Pseudo Code
```python
# Training
def train_recursively(model, video, text_embedding, t1, t2, threshold):
    if t1 == t2:
        return 0
    
    frame1, frame2 = video.get_latent(t1), video.get_latent(t2)
    if is_similar(frame1, frame2):
        return 0
    
    mid = (t1+t2)//2
    ground_truth_mid = video.get_latent(mid)
    loss = learn_transition(model, frame1, frame2, ground_truth_mid, t2-t1)

    loss += train_recursively(model, video, text_embedding, t1, mid, threshold)
    loss += train_recursively(model, video, text_embedding, mid, t2, threshold)

    return loss

def is_similar(latent1, latent2, threshold):
    return distance(latent1, latent2) < threshold

def learn_transition(model, latent1, latent2, ground_truth_mid, delta_time, text_embedding):
    # Time considered Cross-Attention
    time_embedding = embedding(delta_time)
    predicted_mid = model.predict_mid_frame(latent1, latent2, time_embedding, text_embedding)

    return MSE_loss(predicted_mid, ground_truth_mid)

M = Model()
V = Video()
text_prompt = "A dog yarning on the ground"
text_embedding = text_encoder.encode(text_prompt)
optimizer = Adam(model.parameters())

total_loss = train_recursively(M, V, text_embedding, 0, V.last_frame, threshold=0.1)
total_loss.backward()
optimizer.step()
```

- Sampling
  - ODE?
    - How do we define the drift?
    - Can we use weighted sum of delta_t1 (instant change) and delta_t100(long term change)?
```python
# Sampling
def sample()
```