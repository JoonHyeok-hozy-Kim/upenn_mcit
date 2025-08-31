# Auto-Encoding Variational Bayes
Kingma et al.

## 2. Method
- Problem Setting)
  - Assumptions)
    - i.i.d. dataset with latent variables per datapoint
    - fixed dataset for simplicity
      - But can be applied to online, non-stationary settings
  - Goal)
    - Perform on the global parameters either
      - maximum likelihood (ML) inference
      - maximum a posteriori (MAP) inference
    - Perform variational inference on the latent variables
    - Extend to performing variational inference on the global parameters

### 2.1 Problem Scenario
- Settings)
  - $`\mathbf{X} = \{\mathbf{x}^{(i)}\}_{i=1}^N`$ : i.i.d. dataset
    - i.e.) samples of some continuous or discrete variable $`\mathbf{x}`$
  - $`\mathbf{z}`$ : an unobserved continuous random variable that generated the data $`\mathbf{X}`$ in two steps
    - a value $`\mathbf{z}^{(i)}`$ is generated from some **prior** distribution $`p_{\boldsymbol{\theta}^*}(\mathbf{z})`$
    - a value $`\mathbf{x}^{(i)}`$ is generated from some conditional (**likelihood**) distribution $`p_{\boldsymbol{\theta}^*}(\mathbf{x\vert z})`$
  - $`p_{\boldsymbol{\theta}^*}(\mathbf{z})`$ and $`p_{\boldsymbol{\theta}^*}(\mathbf{x\vert z})`$
    - came from parametric families of distributions $`p_{\boldsymbol{\theta}}(\mathbf{z})`$ and $`p_{\boldsymbol{\theta}}(\mathbf{x\vert z})`$
    - their PDFs are differentiable almost everywhere w.r.t. both $`\boldsymbol{\theta, z}`$
  - No common simplifying assumptions about the marginal or posterior probabilities.
    - cf.) Still the algorithm works well in cases of...
      - Intractable
        - $`p_{\boldsymbol{\theta}}(\mathbf{x}) = \displaystyle\int p_{\boldsymbol{\theta}}(\mathbf{z})\; p_{\boldsymbol{\theta}}(\mathbf{x\vert z}) \text{d}\mathbf{z}`$ : the marginal likelihood
        - $`p_{\boldsymbol{\theta}}(\mathbf{z\vert x}) = \displaystyle\frac{p_{\boldsymbol{\theta}}(\mathbf{x\vert z})p_{\boldsymbol{\theta}}(\mathbf{z})}{p_{\boldsymbol{\theta}}(\mathbf{x})} `$ : the posterior distribution
      - Large Dataset
  - $`q_{\boldsymbol{\phi}}(\mathbf{z\vert x})`$ : the recognition model
    - Desc.)
      - $`q_{\boldsymbol{\phi}}(\mathbf{z\vert x})`$ is an approximation to the intractable true posterior $`p_{\boldsymbol{\theta}}(\mathbf{z\vert x})`$
    - Props.)
      - It is not necessarily factorial.
      - Its parameters $`\boldsymbol{\phi}`$ are not computed from me closed-form expectation.
- Application)
  - This model can be used in solving...
    - Approximate $`\boldsymbol{\theta}_{\text{ML}}`$ or $`\boldsymbol{\theta}_{\text{MAP}}`$
    - Approximate posterior inference of the latent variable $`\mathbf{z}`$ given $`\mathbf{x}`$
      - i.e.) $`p_{\boldsymbol{\theta}}(\mathbf{z\vert x})`$
    - Approximate marginal inference of the variable $`\mathbf{x}`$
      - When?)
        - Cases where a prior over $`\mathbf{x}`$ is required.
      - e.g.)
        - image denoising, inpainting, super-resolution

<br>

### 2.2 The Variational Bound
#### Concept) The Variational Lower Bound
- Def.)
  - $`\mathcal{L}(\boldsymbol{\theta,\phi};\mathbf{x}^{(i)})`$ : the variational lower bound on the marginal likelihood of datapoint $`i`$
    - where 
      - $`\underbrace{\log p_{\boldsymbol{\theta}} \left( \mathbf{x}^{(i)} \right)}_{\text{marginal likelihood of a datapoint}} = \underbrace{D_{KL} \left( \left. q_{\boldsymbol{\phi}} \left(\mathbf{z\vert x}^{(i)} \right) \right\Vert p_{\boldsymbol{\theta}} \left(\mathbf{z\vert x}^{(i)} \right)  \right)}_{\text{KL-div of the approximate from the true posterior}} + \mathcal{L}(\boldsymbol{\theta,\phi};\mathbf{x}^{(i)})`$
    - cf.)
      - We may get the marginal likelihood as
        - $`\log p_{\boldsymbol{\theta}} \left( \mathbf{x}^{(1)}, \cdots, \mathbf{x}^{(N)}  \right) = \displaystyle\sum_{i=1}^N \log p_{\boldsymbol{\theta}} \left(\mathbf{x}^{(i)} \right)`$
  - Why is this a lower bound?)
    - Consider that the KL-divergence is non-negative.
    - Thus,   
      $`\begin{aligned}
          \log p_{\boldsymbol{\theta}} \left( \mathbf{x}^{(i)} \right) &= \underbrace{D_{KL} \left( \left. q_{\boldsymbol{\phi}} \left(\mathbf{z\vert x}^{(i)} \right) \right\Vert p_{\boldsymbol{\theta}} \left(\mathbf{z\vert x}^{(i)} \right)  \right)}_{\text{always non negative}} + \mathcal{L}(\boldsymbol{\theta,\phi};\mathbf{x}^{(i)}) \\
          &\ge \mathcal{L}(\boldsymbol{\theta,\phi};\mathbf{x}^{(i)})
      \end{aligned}`$
- Prop.)
  - $`\mathcal{L}(\boldsymbol{\theta,\phi};\mathbf{x}^{(i)}) = \mathbf{E}_{q_{\boldsymbol{\phi}} (\mathbf{z\vert x})} \left[ -\log q_{\boldsymbol{\phi}} (\mathbf{z\vert x}) + \log p_{\boldsymbol{\phi}} (\mathbf{x,z}) \right]`$
    - Derivation)
      - By definition, we have   
        $`\begin{aligned}
            \mathcal{L}(\boldsymbol{\theta,\phi};\mathbf{x}^{(i)}) 
            &= \log p_{\boldsymbol{\theta}} \left( \mathbf{x}^{(i)} \right) - D_{KL} \left( \left. q_{\boldsymbol{\phi}} \left(\mathbf{z\vert x}^{(i)} \right) \right\Vert p_{\boldsymbol{\theta}} \left(\mathbf{z\vert x}^{(i)} \right)  \right) \\
            &= \log p_{\boldsymbol{\theta}} \left( \mathbf{x}^{(i)} \right) - q_{\boldsymbol{\phi}} \left(\mathbf{z\vert x}^{(i)} \right) \log\left( \frac{q_{\boldsymbol{\phi}} \left(\mathbf{z\vert x}^{(i)} \right)}{p_{\boldsymbol{\theta}} \left(\mathbf{z\vert x}^{(i)} \right)} \right)
        \end{aligned}`$