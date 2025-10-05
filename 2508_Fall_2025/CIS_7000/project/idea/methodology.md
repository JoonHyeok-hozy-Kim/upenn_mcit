# Methodologies

### 1. Train a Video Model
- Model Type)
  - Diffusion Video Generative Model
  - Transformer Architecture
- Training Dataset)
  - Should be containing plenty of physics phenomena
    - What I found)
      - Kinetics-400 : Google Deepmind
        - FVD score applicable
        - Human actions.
        - May not include phenomena like burning, flying, etc.
  - Will the physics info be included during the pretraining?
    - Why does this matters?
      - [Physics of LM](../references/methodologies/physics_of_lm.md) shows that knowledge augmentation during the pretraining drastically improves the OOD inference.
    - How?)
      - **Q) How can physics phenomena be added to the pretraining?**
        - Text embeddings?
        - Sample videos?
- Implementation)
  - Based on [Phyworld](../references/methodologies/phyworld.md)

<br>

### 2. Controlled Experiment
- Reference
  - [Head Hunter](../references/methodologies/attention_head_selection.md#41-head-hunter) from [Attention Head Selection](../references/methodologies/attention_head_selection.md) 
    - How?)
      - Iterate through the attention heads.
        - Apply perturbation([PAG](../references/methodologies/perturbed_attention_guidance.md)) on that head.
        - For each (prompt-seed) pair
          - Generate an image
          - Evaluate the Objective Function Score
            - [PickScore]() is used for the [Head Hunter](../references/methodologies/attention_head_selection.md#41-head-hunter) method
        - Calculate the average objective score of that head.
      - Add top-k heads that had the largest objective score improvement
- Application to Our setting)
  - Iterate through the attention heads
    - Apply perturbation on that head.
      - **Q) What perturbation should we use?**
        - Attention Specific
          - [PAG](../references/methodologies/perturbed_attention_guidance.md)
          - [SEG](../references/methodologies/smoothed_energy_guidance.md)
            - Is Gaussian blur applicable in Video model?
        - Not attention specific
          - [CFG](../references/basics/classifier_free_guidance.md)
          - [AutoGuidance](../references/basics/autoguidance.md)
      - **Q) Would Guidance worked on image quality improvement work on the physics phenomenon modeling as well?**
        - We may never know until we try tho...
    - For each (prompt-seed) pair
      - Generate a video.
      - Evaluate on the Objective Function Score
        - **Q) What evaluation score can we use for the Video model?**
          - Existing scores
            - FVD : FID on Video Models
              - Cheap evaluation
              - **Q) Can we assume that improvement on the FVD guarantees the better understanding of the physics phenomena?**
            - [WM-ABench](../references/evaluation_methods/wm-abench.md)
              - 2 Stage : Perception, Prediction
              - Human Evaluation.
                - Is this reusable?
                - Hiring people from Amazon Mechanical Turk?
            - [PhysBench](../references/evaluation_methods/physbench.md)
              - 4 Categories : Property, Relation, Scene, Dynamics
              - Four option questionnaire
              - Evaluated by LLMs.
                - **Is this reusable?**
                - **Is this applicable to our generated models?**
            - [PhysicsIQ](../references/evaluation_methods/physicsIQ.md)
              - Get 3 seconds video and generate 5 seconds after that.
              - Compare the original 5 sec and generated 5 sec.
              - Evaluated with four scores
                - MSE, various IOUs
              - **Q) Can our model get 3 sec video input?**
          - Building Our own score?
            - e.g.) Pick Score Style?
              - How?) 
                - Get human feed back and create a video version score like PickScore
              - Drawback)
                - Hard to get meaningful amount of user preference data
            - e.g.) Physics IQ Style?
    - Add top-k heads that had the largest objective score improvement
    - Evaluate an individual head's contribution to the improvement

<br>

### 3. Evaluation on the Experiment result
- Reference)
  - Head Hunter
    - Evaluate an individual head's contribution to the improvement
      - Partial Perturbation on other generations.
      - Interpolation on the strength of the perturbation
- Application to Our setting)
  - Evaluate individual heads?