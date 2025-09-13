# Improved Denoising Diffusion Probabilistic Models
Nichol et al. 2021

## 2. Denoising Diffusion Probabilistic Models
### 2.1 Definitions
- $`x_0\sim q(x_0)`$ : a given dataset
- Forward Process
  - Def.)
    - $`q`$ : the forward noising process which produces latents $`x_1,\cdots,x_T`$ by adding Gaussian noise at time $`t`$ with variance $`\beta_t\in(0,1)`$ as
      - $`q(x_1,\cdots,x_T\mid x_0) := \displaystyle\prod_{t=1}^T q(x_t\mid x_{t-1})`$
        - where $`q(x_t\mid x_{t-1}) := \displaystyle\mathcal{N}(x_t;\; \sqrt{1-\beta_t} x_{t-1}, \beta_t\mathbf{I})`$
    - Prop.)
      - Given sufficiently large $`T`$ and well behaved schedule of $`\beta_t`$, we have
        - $`x_T\sim\mathcal{N}(0,\mathbf{I})`$
      - Directly sampling an arbitrary $`t`$-th step
        - $`q(x_t\mid x_0) = \mathcal{N}(x_t;\; \sqrt{\bar{\alpha}_t} x_0, (1-\bar{\alpha}_t)\mathbf{I})`$
          - where
            - $`\alpha_t := 1-\beta_t`$
            - $`\bar{\alpha}_t := \displaystyle\prod_{s=0}^t \alpha_s`$
        - $`x_t = \sqrt{\bar{\alpha}_t}x_0 + \sqrt{1-\bar{\alpha}_t}\epsilon`$
          - where $`\epsilon\sim\mathcal{N}(0,\mathbf{I})`$
      - $`1-\bar{\alpha}_t`$ tells us the variance of the noise for an arbitrary step.
      - Posterior $`q(x_{t-1}\mid x_t, x_0)`$
        - Let 
          - $`q(x_{t-1}\mid x_t, x_0) = \mathcal{N}(x_{t-1};\;\tilde{\mu}(x_t, x_0), \tilde{\beta}_t\mathbf{I})`$
            - $`\tilde{\mu}(x_t, x_0)`$ : the posterior mean
            - $`\tilde{\beta}_t`$ : the posterior variance.
        - By the Bayes rule, we have
          - $`q(x_{t-1}\mid x_t, x_0)\varpropto q(x_t\mid x_{t-1}, x_0)q(x_{t-1}\mid x_0)`$
        - Thus, we may denote the moments of the posterior $`q`$ as
          - $`\tilde{\beta}_t := \displaystyle\frac{1-\bar{\alpha}_{t-1}}{1-\bar{\alpha}_t} \beta_t`$
          - $`\mu_t(x_t,x_0) := \displaystyle\frac{\sqrt{\bar{\alpha}_{t-1}}\beta_t}{1-\bar{\alpha}_t} x_0 + \frac{\sqrt{\alpha_{t}}(1-\bar{\alpha}_{t-1})}{1-\bar{\alpha}_t} x_t`$
- Reverse Process
  - Def.)
    - $`p_\theta`$ : approximate $`q`$ as
      - $`p_\theta(x_{t-1}\mid x_t) := \mathcal{N}(x_{t-1};\; \mu_\theta(x_t,t), \Sigma_\theta(x_t, t))`$
        - where $`p_\theta(x_{t-1}\mid x_t) \approx \underbrace{q(x_{t-1}\mid x_{t})}_{\text{reverse order!}}`$
- Variational Lower Bound
  - Def.)
    - $`L_{\text{vlb}} := L_0 + L_1 + \cdots + L_{T-1} + L_T`$
      - where
          - $`L_0 := -\log p_\theta(x_0\mid x_1)`$
          - $`L_{t-1} := D_{KL}\left( q(x_{t-1}\mid x_t, x_0) \;\Vert\; p_\theta(x_{t-1}\mid x_t) \right)`$
          - $`L_{T} := D_{KL}\left( q(x_{T}\mid x_0) \;\Vert\; p(x_{T}) \right)`$
            - cf.) $`L_T`$ is independent of $`\theta`$
              - Why?) $`x_T`$ is a pure noise if the forward process has done its job properly.
  - Prop.)
    - If $`q,p_\theta`$ are all Gaussians, we may get the closed form solutions for $`\forall L`$.
    - DDPM used [discretization](../../../paper_presentation/250924_latent_diffusion/paper_summary/ddpm.md#concept-gaussian-discretization) to calculate the likelihood $`p_\theta(x_{0}\mid x_1)`$


<br>

### 2.2 Training in Practice
- Sampling $`t`$
  - $`t\sim\text{Uniform}(1,T)`$ in each mini-batch