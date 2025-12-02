# Any Order AR Model

## Existing Works
### AR Models
- Pros.)
  - Proven success LLM (MLE Training!)
  - Scalability
  - Parallel training
    - by Teacher Forcing?
- Cons.)
  - Left to right order generation
  - Sequential inference
    - No parallel generation

### Masked Diffusion Model
- Pros.)
  - Flexible generation
  - Bi-directional generation in parallel
- Cons.)
  - Dependency on time step T, which slows the sampling
  - Complicated Loss
    - ELBO approximation

<br>

## Suggestion : Any Order AR
- Advantage)
  - Benefits from AR Modeling
    - MLE Training
    - Scalability
    - Parallel Training
  - Flexible Generation
- Limits)
  - Sequential generation as AR

<br>

## Methodology
### Problem Settings)
- Suppose we are given an $`L`$-length sequence $`x = (x_1,\cdots,x_L)`$.
- We may randomly permute the given sequence into $`\tilde{x} = (\tilde{x}_1,\cdots, \tilde{x}_L)`$.
- Suppose we are given $`(\tilde{x}_0, i_0)`$ where $`\tilde{x}_0 = x_{i_0}`$, $`i_0\in[0, L-1]`$
  - We may simplify this by starting with... 
    - $`(\text{[BOS]}, 0)`$ 
    - or $`(\text{[EOS]}, L-1)`$
- Then, we may predict the next most probable token on a random position by
  - First predicting the next token $`\tilde{x}_1`$.
  - Then, predicting the related position $`i_1`$ s.t. $`\tilde{x}_1 = x_{i_1}`$.
- We may repeat this $`L`$ times until we find $`\tilde{x}_L = i_L`$.
- As a result, we have recovered $`x`$ by re-ordering $`\tilde{x}`$ with $`i_{0:L}`$

<br>

### Problem Definition)
  - We may define the prediction at $`t`$ as
    - $`x_t \in V`$ : the $`t`$-th token
      - where $`V`$ is the vocabulary set
    - $`i_t \in [0, L-1]`$ : the $`t`$-th position
      - given $`x_{0:t-1}, i_{0:t-1}`$ the previous tokens and positions
      - $`L`$ : the length of the sequence
        - which can be either flexible or fixed
  - We want to predict
    - $`p(x_t, i_t\mid x_{0:t-1}, i_{0:t-1})`$
- Optimization)
  - MLE on $`p(x_t, i_t\mid x_{0:t-1}, i_{0:t-1})`$
    - i.e.)
      - $`\mathcal{L} = -\log p(x_t, i_t\mid x_{0:t-1}, i_{0:t-1})`$  
- Loss Parameterization)   
  $`\begin{aligned}
    \mathcal{L} &= -\log p(x_t, i_t\mid x_{0:t-1}, i_{0:t-1}) \\
    &= -\log \underbrace{p(i_t\mid x_t, x_{0:t-1}, i_{0:t-1})}_{\text{position prediction given }x_t} -\log \underbrace{p(x_t\mid x_{0:t-1}, i_{0:t-1})}_{\text{token prediction}}
  \end{aligned}`$

<br>

## Implementations)
### (What we discussed) Two Models Strategy
- Consists of two transformer models:
  - First Model
    - Input : sequence $`x`$
    - Output : logit vectors for the token choice
  - Seconde Model
    - Input : Output from the first model
    - Output : Positional information of the model

<br>

### (Suggestion) One Model Strategy?
- Using the transformer, we may set our model to output $`V+L`$ dimensional logit 
  - where 
    - $`V`$ is the size of the vocabulary
    - $`L`$ is the length of the sequence.
- We may give the conditional (sequential) relation between the token prediction and the positional prediction by defining the logits as
  - $`L_x = W_x H_x`$
  - $`L_i = W_i (H_i \oplus L_x')`$
    - where 
      - $`\oplus`$ denotes the concatenation
      - $`L_x'`$ is the dimensionally reduced $`L_x`$.
        - Recall that $`L_x\in\mathbb{R}^V`$, which is massive!
  - And then backprop!