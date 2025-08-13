# Scalable Global Optimization via Local Bayesian Optimization (TuRBO)
Eriksson et al. 2019

### Problem Settings)
- Problem)
  - Find $`\mathbf{x}^* \in\Omega \text{ s.t. } f(\mathbf{x}^*) \le f(\mathbf{x}),\quad\forall\mathbf{x}\in\Omega`$
    - where
      - $`f:\Omega\rightarrow\mathbb{R}`$
      - $`\Omega = [0,1]^d`$
  - $`y(\mathbf{x}) = f(\mathbf{x})+\epsilon`$ : observation
    - where $`\epsilon\sim\mathcal{N}(0,\sigma^2)`$ is the noise
- Sol.)
  - Maintain several independent local models


<br>

### Concept) Local Modeling
- Desc.)
  - Make suggestions using a simple surrogate model inside a [TR](#concept-trusted-region-tr).
- Advantage
  - Local optimization in the gradient-free settings
- Challenges)
  - Deterministic examples such as CONYLA are notorious for handling noisy observations poorly.
  - Simple surrogate models require overly small TRs.
- Sol.)
  - Use GP as the surrogate model.

<br>

### Concept) Trusted Region (TR)
- Def.)
  - A Hyperrectangle centered at the best solution found so far denoted by $`\mathbf{x}^\star`$
    - Noise free case)
      - $`\mathbf{x}^\star`$ is the location of the best observation so far
    - Noisy case)
      - Use the smallest posterior mean under the surrogate model
  - $`L`$ : Side Length of the TR
    - Then the total volume of the TR is $`L^d`$
- Hyperparams)
  - $`L_{\text{init}}, L_{\text{max}}, L_{\text{min}}`$
  - $`\tau_{\text{succ}}`$, $`\tau_{\text{fail}}`$
- How it works)
    - Initialize the base side length of the TR to $`L\leftarrow L_{\text{init}}`$
      - The actual side length for each dimension is obtained from this base side length by rescaling according to its lengthscale $`\lambda_i`$ in the GP model
        - i.e.) $`L_i = \displaystyle\lambda_i\;\frac{L}{\left(\prod_{j=1}^d \lambda_j\right)^{1/d}}`$
    - Single Local Optimization Run
      - Check Points)
        1. TR should be sufficiently large to contain good solutions.
        2. TR should be small enough to ensure that the local model is accurate within the TR.
      - Adjusting $`L`$
        - Def.)
          - Success : a candidate that improves upon $`\mathbf{x}^\star`$
          - Failure : a candidate that does not improve upon $`\mathbf{x}^\star`$
            - Q) No candidate?
        - Rule)
          - After $`\tau_{\text{succ}}`$ consecutive successes, double the size
            - i.e.) $`L\leftarrow\min\left\{L_{\text{max}},2L\right\}`$
          - After $`\tau_{\text{fail}}`$ consecutive failures, halve the size
            - i.e.) $`L\leftarrow L/2`$
          - Reset success/failure counters after adjusting $`L`$
    - Simultaneous $`m`$ TRs
      - $`\text{TR}_\ell`$ : $`\ell`$-th TR with $`\ell\in\{1,\cdots,m\}`$
        - Each $`\text{TR}_\ell`$ is modeled to be a lever of a multi-armed bandit problem solving local BO problem.
      - In each iteration, the we should...
        - Select a batch of $`q`$ candidates : $`\left\{ \mathbf{x}_1^{(t)}, \cdots, \mathbf{x}_q^{(t)} \right\}`$
        - Update all local optimization problems for which candidates were drawn.
      - How?)
        - Use Thompson Sampling (TS) to...
          - select candidates within a single TR
          - select candidates across the set of trust regions simultaneously
      - Model)
        - Select the $`i`$-th candidate s.t. it minimizes the function value across 
          - all $`m`$ samples 
          - all trust regions
        - i.e.) $`\displaystyle\mathbf{x}_i^{(t)} \in \underbrace{\arg\min_{\ell}}_{\text{Across TRs}} \; \underbrace{\arg\min_{\mathbf{x}\in\text{TR}_\ell}}_{\text{Within TR}} \; f\ell^{(i)}`$
          - where 
            - $`f_\ell^{(i)}\sim\mathcal{GP}_\ell^{(t)}\left( \mu_\ell(\mathbf{x}), k_\ell(\mathbf{x}, \mathbf{x}') \right)`$ : the posterior function from the local GP within $`\text{TR}_{\ell}`$
            - $`\mathcal{GP}_\ell^{(t)}`$ : the GP posterior of $`\text{TR}_{\ell}`$ at $`t`$

<br>


### [Code Implementation](https://github.com/uber-research/TuRBO)

