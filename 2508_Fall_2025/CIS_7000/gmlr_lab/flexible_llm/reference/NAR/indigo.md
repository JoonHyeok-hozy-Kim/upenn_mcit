# Insertion-based Decoding with Automatically Inferred Generation Oder
[@guInsertionbasedDecodingAutomatically2019]

## 2. Neural Autoregressive Decoding
- Problem)
  - Generate a sequence $`\boldsymbol{y}=(y_1,\cdots,y_T)`$
    - conditioned on some inputs $`\boldsymbol{x}=(x_1,\cdots,x_{T'})`$
- Goal)
  - Build a model parameterized by $`\theta`$ that models the conditional probability of $`\boldsymbol{y}`$ given $`\boldsymbol{x}`$ as
    - $`p_\theta(\boldsymbol{y\mid x}) = \displaystyle\prod_{t=0}^T p_\theta(y_{t+1}\mid y_{0:t}, x_{1:T'})`$
      - where
        - $`y_0 := \langle s \rangle`$
        - $`y_T := \langle /s \rangle`$
- Learning)
  - Maximize the conditional likelihood 
    - $`\log p_\theta(\boldsymbol{y\mid x}) = \displaystyle\sum_{t=0}^T \log p_\theta(y_{t+1}\mid y_{0:t}, x_{1:T'})`$
- Decoding)
  - Predict one word at each step.
  - Given any source $`\boldsymbol{x}`$, follow the order of factorization to generate tokens sequentially using some heuristic based algorithms
    - e.g.) Greedy decoding, beam-search
- Prop.)
  - **L2R**
    - i.e.) Sequentially predicts the conditional probability of the next token at each step $`t`$.
  - Can be implemented by any function approximator
    - e.g.) RNN, Transformer

<br><br>

## 3. InDIGO
- Idea)
  - Why **L2R** as the [Neural Autoregressive Models](#2-neural-autoregressive-decoding)
  - Any order of generating the sequence $`\boldsymbol{y}`$ is possible in principle.
  - We may train a model for each permutation separately.

### 3.1 Orders as Latent Variables
- Def.) 
  - The target sequence $`\boldsymbol{y}`$ is generated in a particular order $`\boldsymbol{\pi} = (z_2,\cdots,z_T,z_{T+1})\in\mathcal{P}_T`$
    - where
      - $`\boldsymbol{y}=(y_2,\cdots,y_{T+1})`$
      - $`\boldsymbol{\pi} = (z_2,\cdots,z_T,z_{T+1})\in\mathcal{P}_T`$ : 
      - $`\mathcal{P}_T`$ : the set of all the permutations $`(1,\cdots,T)`$
  - We may rewrite as
    - $`\boldsymbol{y_\pi}=\left\{ (y_2,z_2),\cdots,(y_{T+1},z_{T+1}) \right\}`$
      - cf.) Special Tokens
        - $`(y_0, z_0) = \left(\langle s\rangle, 0\right)`$ : left bound
        - $`(y_1, z_1) = \left(\langle /s\rangle, T+1\right)`$ : right bound
  - Then the conditional probability can be derived as
    - $`p_\theta(\boldsymbol{y\mid x}) = \displaystyle\sum_{\boldsymbol{\pi}\in\mathcal{P}_T} p_\theta(\boldsymbol{y_\pi}\mid\boldsymbol{x})`$
      - where
        - $`\displaystyle p_\theta(\boldsymbol{y_\pi}\mid\boldsymbol{x}) = \underbrace{p_\theta(y_{T+2}\mid y_{0:T+1}, z_{0:T+1}, x_{1:T'})}_{\text{End of decoding Probability}} \cdot \prod_{t=1}^T p_\theta(y_{t+1}, z_{t+1}\mid y_{0:t}, z_{0:t}, x_{1:T'})`$ : 
        - $`y_{T+2} := \langle eod \rangle`$ : end of decoding signal
        - $`p(y_{T+2}\mid\cdot)`$ : the end of decoding probability.
- Prop.)
  - Decoding predicts $`(y_{t+1},z_{t+1})`$ 
    - i.e.) the pair of the next word and its position 