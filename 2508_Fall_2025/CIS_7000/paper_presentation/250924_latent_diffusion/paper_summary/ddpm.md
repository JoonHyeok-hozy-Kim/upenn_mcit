# Denoising Diffusion Probabilistic Models
Ho et al. 2020

## 2. Background
### Concept) Diffusion Model
- First introduced in...
  - Sohl-Dickstein et al. 2015, *Deep unsupervised learning using nonequilibrium thermodynamics.*
- Goal)
  - Learn the [forward](#concept-forward-process) and [reverse](#concept-reverse-process) process.
  - Generate data using the reverse process

#### Concept) Forward Process
- Goal)
  - Gradually destroy the original data $`x_0`$ until it becomes pure noise.
- Def.)
  - A **fixed** Markov chain that gradually adds Gaussian noise to data as
    - $`q(\mathbf{x}_t\mid \mathbf{x}_{t-1}) = \displaystyle\mathcal{N}\left(\mathbf{x}_t; \sqrt{1-\beta_t} \mathbf{x}_{t-1}, \beta_t I \right)`$
      - where $`\beta_t\in\mathbb{R}`$ is a scalar variance schedule parameter for step $`t`$
- Props.)
  - **No learnable** parameters
  - As $`t\to T`$, the sample $`x_t \rightarrow \mathbf{x}_T\sim\mathcal{N}(0,I)`$ : the pure noise!
  - $`\mathbf{x}_t = \displaystyle\sqrt{\bar{\alpha}_t} \mathbf{x}_0 + \sqrt{1-\bar{\alpha}_t} \epsilon`$
    - where 
      - $`\alpha_t = 1-\beta_t`$
      - $`\bar{\alpha}_t = \displaystyle\prod_{s=1}^t \alpha_s`$
      - $`\epsilon\sim\mathcal{N}(0,I)`$ : the Gaussian noise added to corrupt $`\mathbf{x}_0`$ into $`\mathbf{x}_t`$

#### Concept) Reverse Process
- Goal)
  - Recover data from noise made by the [forward process](#concept-forward-process)
  - We may generate synthetic data using this.
    - cf.) Recall inputting latent code $`\mathbf{z}`$ in [GAN](../../250908_style_gan/paper_note_gan.md)
- Def.)
  - A parameterized Markov chain that aims to invert the forward process and recover data from noise as
    - $`p_\theta(\mathbf{x}_{t-1}\mid \mathbf{x}_t) = \mathcal{N}(\mathbf{x}_{t-1}; \mu_\theta(\mathbf{x}_t, t), \Sigma_\theta(\mathbf{x}_t, t))`$
- Props.)
  - $`\theta`$ : the learnable parameters
  - Learned via minimizing a denoising objective:
    - $`\mathbb{E}_{\mathbf{x}_0,\epsilon,t} \left[ \Vert \epsilon - \epsilon_\theta(\mathbf{x}_t, t) \Vert^2 \right]`$
      - Why?)
        - Recall from [forward process](#concept-forward-process) that 
          - $`\mathbf{x}_t = \displaystyle\sqrt{\bar{\alpha}_t} \mathbf{x}_0 + \sqrt{1-\bar{\alpha}_t} \epsilon`$ 
          - $`\epsilon\sim\mathcal{N}(0,I)`$
        - Since $`\mathbf{x}_t`$ is literally a mixture of the clean data $`\mathbf{x}_0`$ and noise $`\epsilon`$, recovering $`\mathbf{x}_0`$ is equivalent to recovering $`\epsilon`$ 
          - where $`\epsilon`$ is the true Gaussian noise sampled during the [forward process](#concept-forward-process)
        - Predicting $`\epsilon`$ is easier and leads to a simpler loss function.
        - Put $`\epsilon_\theta(\mathbf{x}_t, t)`$: the model’s prediction of that noise.
  - Sampling starts from $`\mathbf{x}_T \sim \mathcal{N}(0,I)`$ and progressively denoises until $x_0$.

<br>

#### Model) Diffusion Model
- $`p_\theta(\mathbf{x}_0) := \displaystyle\int p_\theta(\mathbf{x}_{0:T}) \text{d} \mathbf{x}_{1:T}`$ : the latent variable model
  - where
    - $`\mathbf{x}_0`$ : the data sample (e.g. pixel vector) s.t. $`\mathbf{x}_0\sim q(\mathbf{x}_0)`$
      - where $`q`$ is the ground truth distribution that data is generated from.
    - $`p_\theta(\mathbf{x}_0)`$ : the learned distribution
      - cf.) We want $`p_\theta(x_0) \approx q(x_0)`$
    - $`\mathbf{x}_1,\cdots,\mathbf{x}_T`$ : the latents of the same dimensionality as the data $`\mathbf{x}_0`$
    - $`p_\theta(\mathbf{x}_{0:T}) := p(\mathbf{x}_T)\displaystyle\prod_{t=1}^T p_\theta(\mathbf{x}_{t-1}\mid\mathbf{x}_t)`$ : the [reverse process](#concept-reverse-process)
      - i.e.) the joint distribution defined as a Markov chain with learned Gaussian transitions starting at $`p(\mathbf{x}_T) = \mathcal{N}(\mathbf{x}_T; \mathbf{0, I})`$ : purely Gaussian so does not depend on $`\theta`$
      - Desc.)
        - Diffusion models generate data $`\mathbf{x}_0`$ using the reverse Markov chain as   
          $`\begin{aligned}
              & x_T \sim \mathcal{N}(0,I) & \text{(pure Gaussian noise)} \\
              \rightarrow& x_{T-1} \sim p_\theta(x_{T-1}\mid x_T) \\
              \rightarrow& \cdots \\
              \rightarrow& x_0 \sim p_\theta(x_0 \mid x_1) \\
          \end{aligned}`$
      - $`p_\theta(\mathbf{x}_{t-1}\mid\mathbf{x}_t) := \mathcal{N}(\mathbf{x}_{t-1}; \mu_\theta(\mathbf{x}_t, t), \Sigma_\theta(\mathbf{x}_t, t))`$
- $`q(\mathbf{x}_{1:T}\mid\mathbf{x}_0) := \displaystyle\prod_{t=1}^T q(\mathbf{x}_t\mid \mathbf{x}_{t-1})`$ : the [forward process](#concept-forward-process) (diffusion process)   
  - where
    - $`q(\mathbf{x}_t\mid \mathbf{x}_{t-1}) := \displaystyle\mathcal{N}(\mathbf{x}_t; \sqrt{1-\beta_t} \mathbf{x}_{t-1}, \beta_t \mathbf{I})`$
- Loss Function)
  - What we want is $`\log p_\theta(\mathbf{x}_0)`$.
  - However, this is intractable due to the integral : $`p_\theta(\mathbf{x}_0) := \displaystyle\int p_\theta(\mathbf{x}_{0:T}) \text{d} \mathbf{x}_{1:T}`$
  - Instead, just like the [VAE](../../250917_vae/paper_note.md), we may get the lower bound using the variational distribution as   
    $`\begin{aligned}
        \log p_\theta(\mathbf{x}{0})
        &= \log \int q(\mathbf{x}_{1:T}\mid \mathbf{x}_0) \frac{p_\theta(\mathbf{x}_{0:T})}{q(\mathbf{x}_{1:T}\mid \mathbf{x}_0)} \text{d}\mathbf{x}_{1:T} \\
        &\ge \mathbb{E}_{q(\mathbf{x}_{1:T}\mid \mathbf{x}_0)} \left[ \log\frac{p_\theta(\mathbf{x}_{0:T})}{q(\mathbf{x}_{1:T}\mid \mathbf{x}_0)} \right] & \because (\text{Jensen Inequality})
    \end{aligned}`$
  - Taking the negative, we may get the loss function as   
    $`\begin{aligned}
        \mathcal{L} &= \mathbb{E}_q \left[ -\log\frac{p_\theta(\mathbf{x}_{0:T})}{q(\mathbf{x}_{1:T}\mid \mathbf{x}_0)} \right] \\
        &= \mathbb{E}_q \left[ -\log\frac{p(\mathbf{x}_T)\displaystyle\prod_{t=1}^T p_\theta(\mathbf{x}_{t-1}\mid\mathbf{x}_t)}{\displaystyle\prod_{t=1}^T q(\mathbf{x}_t\mid \mathbf{x}_{t-1})} \right] \\
        &= \mathbb{E}_q \left[ -\log p(\mathbf{x}_T) -\sum_{t=1}^T \log\frac{p_\theta(\mathbf{x}_{t-1}\mid\mathbf{x}_t)}{q(\mathbf{x}_t\mid \mathbf{x}_{t-1})} \right] &\ge \mathbb{E}_q \left[ -\log p_\theta(\mathbf{x}{0}) \right] \\
    \end{aligned}`$
  - Using the prop. from the [forward process](#concept-forward-process) that $`\mathbf{x}_t \mid \mathbf{x}_0 = \displaystyle\sqrt{\bar{\alpha}_t} \mathbf{x}_0 + \sqrt{1-\bar{\alpha}_t} \epsilon`$, we have   
    - $`q(\mathbf{x}_t\mid \mathbf{x}_0) =\mathcal{N}\left( \mathbf{x}_t; \sqrt{\bar{\alpha}_t} \mathbf{x}_0, (1-\bar{\alpha}_t)\mathbf{I} \right)`$
  - Using the KL-Divergence, we may rewrite $`\mathcal{L}`$ as
    - $`\displaystyle\mathcal{L} = \mathbb{E}_q \left[ \underbrace{D_{\text{KL}}(q(\mathbf{x}_T\mid\mathbf{x}_0) \Vert p(\mathbf{x}_{T}))}_{L_T} + \sum_{t\gt1} \underbrace{D_{\text{KL}}(q(\mathbf{x}_{t-1} \mid \mathbf{x}_t, \mathbf{x}_0) \;\Vert\; p_\theta (\mathbf{x}_{t-1} \mid \mathbf{x}_t))}_{L_{t-1}} - \underbrace{\log p_\theta(\mathbf{x}_0 \mid \mathbf{x}_1)}_{L_0} \right]`$
  - In the previous loss' $`L_{t-1}`$, we may derive
    - $`q(\mathbf{x}_{t-1} \mid \mathbf{x}_t, \mathbf{x}_0) = \mathcal{N}(\mathbf{x}_{t-1}; \tilde{\boldsymbol{\mu}}_t(\mathbf{x}_t, \mathbf{x}_0), \tilde{\beta}_t\mathbf{I})`$ : the forward process posterior
      - where
        - $`\tilde{\boldsymbol{\mu}}_t(\mathbf{x}_t, \mathbf{x}_0) := \displaystyle\frac{\sqrt{\bar{\alpha}_{t-1}}\beta_t}{1-\bar{\alpha}_t} \mathbf{x}_0 + \frac{\sqrt{\alpha_t}(1-\bar{\alpha}_{t-1})}{1-\bar{\alpha}_t}\mathbf{x}_t`$
        - $`\displaystyle \tilde{\beta}_t := \frac{1-\bar{\alpha}_{t-1}}{1-\bar{\alpha}_t} \beta_t`$
      - How?)
        - Using the Bayes Rule
    - Then, since $`q(\mathbf{x}_{t-1} \mid \mathbf{x}_t, \mathbf{x}_0)`$ and $`p_\theta (\mathbf{x}_{t-1} \mid \mathbf{x}_t)`$ are both Gaussian, we may get the closed form expression of $`L_{t-1}`$

<br>

## 3. Diffusion Models and denoising autoencoders
### 3.1 Forward Process and L_T
- Summary)
  - Treat the approximate posterior $`q(\mathbf{x}_{t-1} \mid \mathbf{x}_t, \mathbf{x}_0)`$ to be fixed and have no parameter.
- Desc.)
  - This paper ignores that the forward process variances $`\beta_t`$ are learnable by reparameterization.
  - Instead, it fix them to constants.
  - Thus, the approximate posterior $`q(\mathbf{x}_{t-1} \mid \mathbf{x}_t, \mathbf{x}_0)`$ has no parameter.
    - Why?) $`\alpha_t`$ is made of $`\beta_t`$

### 3.2 Reverse Process and L_{1:T-1}
- Goal)
  - Choose (parameterize) $`p_\theta(\mathbf{x}_{t-1}\mid \mathbf{x}_t) = \mathcal{N}(\mathbf{x}_{t-1}; \mu_\theta(\mathbf{x}_t, t), \Sigma_\theta(\mathbf{x}_t, t))`$
- Options)
  - Variance Parameterization $`(\boldsymbol{\Sigma}_\theta)`$
    - Def.)
      - $`\displaystyle\boldsymbol{\Sigma}_\theta(\mathbf{x}_t, t) = \sigma_t^2 \mathbf{I}`$
    - Options)
      - $`\displaystyle \sigma_t^2 = \tilde{\beta}_t`$
        - Result)
          - Optimal for $`\mathbf{x}_0\sim\mathcal{N}(\mathbf{0, I})`$
      - $`\displaystyle \sigma_t^2 = \frac{1-\bar{\alpha}_{t-1}}{1-\bar{\alpha}_{t}}\beta_t`$
        - Result)
          - Optimal for $`\mathbf{x}_0`$ deterministically set to one point
  - Mean Parameterization $`(\mu_\theta)`$