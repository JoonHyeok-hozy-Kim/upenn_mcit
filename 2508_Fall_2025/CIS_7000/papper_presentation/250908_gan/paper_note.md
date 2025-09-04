# Generative Adversarial Nets
Goodfellow et al.

## 1. Introduction
### Concept) Adversarial Nets Framework
- Desc.)
  - Two Agents
    - Generative Model
      - Goal)
        - Sample by passing random noise through multilayer perceptron
    - Discriminative Model
      - Goal)
        - Determine whether a sample is from the model distribution or the data distribution
      - Desc.)
        - This is also a multilayer perceptron.
- Prop.)
  - We can train both models using backpropagation and dropout algorithm.
  - We can sample from the generative model using only forward propagation.
  - No approximate inference or Markov chains are necessary.

<br>

## 3. Adversarial Nets
### Concept) Two-Player Minimax Game
- Setting)
  - Generator
    - $`G(\boldsymbol{z}; \theta_g)`$
      - Input
        - $`\boldsymbol{z}`$ : the noise 
          - where
            - $`\boldsymbol{z}\sim p_{\boldsymbol{z}}(\boldsymbol{z})`$ 
            - $`p_{\boldsymbol{z}}(\boldsymbol{z})`$ is a prior.
      - Output
        - $`p_g`$ : the generator's distribution over data $`\boldsymbol{x}`$.
      - Props.)
        - MLP with parameters $`\theta_g`$
        - Differentiable function
  - Discriminator
    - $`D(\boldsymbol{x}; \theta_d)`$ : 's mapping
      - Input
        - $`\boldsymbol{x}`$ : the data from either $`p_{\text{data}}(\boldsymbol{x})`$ or $`p_g(\boldsymbol{x})`$
      - Output
        - the probability that $`\boldsymbol{x}`$ came from $`p_{\text{data}}(\boldsymbol{x})`$
          - cf.) $`\displaystyle D^*(\boldsymbol{x}) = \frac{p_{\text{data}}(\boldsymbol{x})}{p_{\text{data}}(\boldsymbol{x}) + p_g(\boldsymbol{x})}`$
      - Props.)
        - MLP with parameters $`\theta_d`$
        - Differentiable function
- Problem Def.)
  - $`\displaystyle \min_G\max_D\; V(D,G) = \mathbb{E}_{\boldsymbol{x}\sim p_{\text{data}}(\boldsymbol{x})} \left[ \log D(\boldsymbol{x}) \right] + \mathbb{E}_{\boldsymbol{z}\sim p_{\boldsymbol{z}}(\boldsymbol{z})} \left[ \log (1 - D(G(\boldsymbol{z}))) \right]`$
    - Desc.)
      - We train...
        - $`D`$ to maximize the probability of assigning the correct label to both training examples and samples from $`G`$.
        - $`G`$ to minimize $`\log(1-D(G(\boldsymbol{z})))`$
- Implementation)   
  ![](./images/001.png)
  - Desc.)
    - One iteration
      - Optimize $`D`$ for $`k`$ times and optimize $`G`$ once.
        - Why doing this?)
          - Optimizing $`D`$ to completion would result in overfitting!
    - $`G`$'s optimization target in the early stage
      - $`\displaystyle\max_G \log D(G(\boldsymbol{z}))`$ instead of $`\displaystyle\min_G \log (1-D(G(\boldsymbol{z})))`$
        - Why?)
          - Early in learning, $`G`$ may be poor, so that $`D`$ can reject samples with high confidence.
          - Then $`\log (1-D(G(\boldsymbol{z})))`$ saturates.
            - Why?)
              - Suppose $`\boldsymbol{x}`$ is from $`G`$ and $`D`$ works well.
              - Then, $`D(G(\boldsymbol{z})) \approx 0 \Rightarrow \log (1-D(G(\boldsymbol{z}))) \approx \log(1) = 0`$