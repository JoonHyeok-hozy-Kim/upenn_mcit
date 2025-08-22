# Multi-Objective Bayesian Optimization over High-Dimensional Search Spaces (MORBO)
Daulton et al. (2022)

### Problem Settings) MOO
- Settings)
  - $`\boldsymbol{f(x)} = \left[ f^{(1)}(\boldsymbol{x}), \cdots, f^{(M)}(\boldsymbol{x}) \right] `$ : a vector valued objective function
    - where
      - $`M\ge2`$ : the number of objectives
      - $`x\in\mathcal{X}\in\mathbb{R}^d`$ where $`\mathcal{X}`$ is a compact set
  - $`\boldsymbol{g(x)\ge0}\in\mathbb{R}^V`$ : black box constraints
    - where
      - $`V\ge0`$ : the number of constraints
  - $`\boldsymbol{f(x)}\succ\boldsymbol{f(x')}`$ : Pareto Dominance
    - i.e.)
      - $`f^{(m)}(\boldsymbol{x}) \ge f^{(m)}(\boldsymbol{x'}), \quad\forall m=1,\cdots,M`$
      - $`f^{(m)}(\boldsymbol{x}) \gt f^{(m)}(\boldsymbol{x'}), \quad\exists m\in\{1,\cdots,M\}`$
  - $`\mathcal{P}(X)`$ : Pareto Frontier 
    - where 
      - $`\mathcal{P}(X) = \left\{ \boldsymbol{f(x) : x} \in X,\;\nexists\boldsymbol{x'}\in X \text{ s.t. } \boldsymbol{f(x')}\succ\boldsymbol{f(x)} \right\}`$
      - $`X\subseteq\mathcal{X}`$
  - $`\mathcal{P}_{\text{feas}}(X)`$ : the feasible PF under black box constraints
    - where $`\mathcal{P}_{\text{feas}}(X) = \mathcal{P}\left( \left\{ \boldsymbol{x}\in X: \boldsymbol{g(x) \ge 0} \right\} \right)`$
  - $`\text{HV}(\mathcal{P}(X)\mid\boldsymbol{r})`$ : the hypervolume indicator
    - where $`\boldsymbol{r}\in\mathbb{R}^M`$ is a reference point typically provided by the practitioner.
- Goal)
  - Identify an approximate PF $`\mathcal{P}(X_n)`$ of the true PF $`P(\mathcal{X})`$
    - where $`\vert X_n\vert = n`$ : the pre-specified budget of function evaluations.
  - The quality of PF is evaluated using the HV indicator.

<br>

### Concept) Collaborative Batch Selection via Global Utility Maximization
- Limit of EHVI)
  - EHVI is a popular acquisition function that integrates HVI over the GP posterior.
    - cf.) S. Daulton et al. (2021) *Parallel bayesian optimization of multiple noisy objectives with expected hypervolume improvement*
  - However, its computationally expensive.
    - Why?) Maximizing EHVI directly requires re-computing the GP posterior and sampling from it in each gradient step.
- Alternative) [Thompson Sampling](../../bayes_opt_textbook/notes/07.md#concept-thompson-sampling)
  - How?)
    - Select $`q`$ points $`\boldsymbol{x}_1, \cdots, \boldsymbol{x}_q`$ for the next batch in a sequentially greedy fashion.
    - Condition them upon the previously selected points in a batch by computing the HVI w.r.t. the current $`\mathcal{P}`$
      - Desc.)
        - Suppose we are at the $`i`$-th iteration and want to select the $`i`$-th point from $`r`$ candidate points $`\hat{\boldsymbol{x}_1},\cdots,\hat{\boldsymbol{x}_r}`$.
          - How to get $`\{\hat{\boldsymbol{x}_1},\cdots,\hat{\boldsymbol{x}_r}\}`$
            - Perturb existing $`\{ \boldsymbol{x}_1, \cdots, \boldsymbol{x}_{i-1} \}`$
        - Draw a sample from the joint posterior over $`\boldsymbol{f}\left( \underbrace{\{ \boldsymbol{x}_1, \cdots, \boldsymbol{x}_{i-1} \}}_{\text{existing PF}} \cup \{\hat{\boldsymbol{x}}_1,\cdots,\hat{\boldsymbol{x}}_r\} \right)`$.
        - Then, we get realization $`\left\{ \tilde{\boldsymbol{f}}(\boldsymbol{x}_{1}), \cdots, \tilde{\boldsymbol{f}}(\boldsymbol{x}_{i-1}), \tilde{\boldsymbol{f}}(\hat{\boldsymbol{x}}_{1}), \cdots, \tilde{\boldsymbol{f}}(\hat{\boldsymbol{x}}_{r}) \right\}`$
        - Select the point that maximizes the HVI jointly with the realizations $`\tilde{\boldsymbol{f}}(\boldsymbol{x}_{1}), \cdots, \tilde{\boldsymbol{f}}(\boldsymbol{x}_{i-1})`$
  - Advantage)
    - Leads to more diverse batch selection compared to selecting each point independently.
      - i.e.) Global utility maximization
    - Enables TRs to target different parts of the PF
    - Allows to implement fully asynchronous optimization

<br>

### Concept) Coordinated Trust Region Center Selection
- Problem)
  - In single-objective optimization, TuRBO centered the local TR at the best feasible observed point.
  - However, in the MOO problem, there is no single best solution.
- Sol.)
  - Use the Hypervolume Contribution (HVC)
    - Desc.) HVC
      - Given a reference point, the HVC of a point on the PF is the reduction in HV if that point is removed.
    - How?)
      - Center a TR at the point with maximal HVC collected by that TR.
      - Exclude points that are already selected by other TRs.
- Advantage)
  - Promotes coverage across the PF
    - Why?) Crowded regions will have lower contribution.

<br>

### Concept) Local Modeling
- Problem)
  - Most BO methods use
    - a single global GP model with a stationary kernel using ard
      - cf.) ARD : Different length scale on each feature
  - Less scalable
    - Why?) $`O(n^3)`$ time complexity of GP inference
- Sol.)
  - MORBO requires each model to be accurate within the corresponding TR.
    - i.e.)
      - Include the observations contained within a local modeling hypercube with edge length $`2L`$.
      - Use slightly larger hypercube to improve teh model close to the TR boundary.
- Advantage)
  - Scalability
    - $`O\left(\displaystyle\frac{n_{\text{TR}}^2}{\eta^3}\right)`$
      - where
        - $`n_{\text{TR}}`$ : the number of TRs
        - $`\eta`$ is the average number of TR modeling spaces a datapoint resides in.
    - $`\eta\lt1`$ : the optimization progresses and the TR shrink
      - Interpretation)
        - Speedups of two orders of magnitude relative to global modeling


<br>

### Concept) Re-Initialization Strategy
- Goal)
  - When a trust region's length reaches $`L_{\text{min}}`$, we should terminate and assign a new TR $`\mathcal{T}_i`$
  - Using the below methodology, we cat get the center of the new TR.
    - Based on Zhang and Golovin (2020) *Random hypervolume scalarizations for provable multi-objective black box optimization*
- Settings)
  - Recall that $`f:\mathbb{R}^d\rightarrow\mathbb{R}^M`$.
    - Then $`M`$ is the dimension of the performance space.
  - Let
    - $`\boldsymbol{\lambda}\sim S_+^{M-1}`$ : a random unit vector in the performance space
      - where $`\underbrace{S_+^{M-1}=\{\boldsymbol{w}\in\mathbb{R}_+^M:\Vert\boldsymbol{w}\Vert_2=1\}}_{\text{a quadrant in }\mathbb{R}^M}`$ (i.e. 사분구, 구의 양의영역)
    - $`\tilde{f}_{t-1}\sim P(f\mid\mathcal{D}_{t-1})`$ : the realization on the GP posterior at $`t-1`$
      - cf.) This make this procedure the Thompson Sampling.
        - Why?)
          - $`\tilde{f}_{t-1}(x)`$ is a sample path.
          - Below, we optimize the problem of $`x_t = \displaystyle\arg\max_{x\in\mathcal{X}} s_{\boldsymbol{\lambda}}\left[ \tilde{f}_{t-1}(x) \right]`$.
          - i.e., finding the best point in the sample path, which is the Thompson Sampling
    - $`(\cdot)_m`$ : the $`i`$-th element of $`(\cdot)`$
    - $`s_{\boldsymbol{\lambda}}[\boldsymbol{y}] = \displaystyle\left(\min_{m\in\{1,\cdots,M\}}\left\{\max\left(\frac{y_m}{\lambda_m}, 0\right)\right\}\right)^M`$: an HV scalarization
      - Meaning)
        - Each dimension of the objective value $`y_m`$ is scaled by the corresponding value $`\lambda_m`$ of a random unit vector $`\boldsymbol{\lambda}`$.
        - Using the $`\max`$ function we trim the dimensions with the negative values.
          - Why?) Not needed for the HVI calculation.
        - Find the dimension that has the worst performance with $`\displaystyle\min_{m\in\{1,\cdots,M\}}`$
          - Why?) Assuming that the reference point $`\boldsymbol{z=0}`$, the smallest positive value will denote the worst performance.
        - Scale it to the power of $`M`$ to match the dimension of the performance space.
      - cf.)
        - Zhang et al. used $`s_{\boldsymbol{\lambda}}[\boldsymbol{y-z}]`$ where $`\boldsymbol{z}`$ was a reference point for the HVI.
        - In this context, we assume $`\boldsymbol{z=0}`$
- Optimization)
  - $`x_t = \displaystyle\arg\max_{x\in\mathcal{X}} s_{\boldsymbol{\lambda}}\left[ \tilde{f}_{t-1}(x) \right]`$
    - Interpretation)
      - Pick the point $`x_t`$ that its realization is located near the pareto front.
      - This is the Thompson Sampling.
        - Why?) Refer to the $`\tilde{f}_{t-1}\sim P(f\mid\mathcal{D}_{t-1})`$ description above.

<br><br>

### Algorithm
- Input
  - $`f`$ : the objective function
  - $`n_{\text{TR}}`$ : the number of Trust Regions
  - $`L_{\text{init}}, L_{\text{min}}, L_{\text{max}}`$ : initial/minimum/maximum TR length
- Output
  - $`\mathcal{P}_n`$ : Approximate Pareto Front
- Procedure
  - Evaluate an initial set of points.
  - $`\mathbf{X}_0\leftarrow\emptyset,\;\mathbf{y}_0\leftarrow\emptyset,\;t\leftarrow1`$
  - `while` budget not exhausted `do`
    - Pick $`n_{\text{TR}}`$ centers using the [HVC](#concept-coordinated-trust-region-center-selection)
    - Initialize trust regions $`\mathcal{T}_1,\cdots,\mathcal{T}_{n_{\text{TR}}}`$
    - `for` $`i\in\{1,\cdots,n_{\text{TR}}\}`$
      - Fit local GP to observations within $`\mathcal{T}_i`$
    - Select $`q`$ candidates $`\{ \boldsymbol{x}_1, \cdots, \boldsymbol{x}_q \}`$ using the [sequential greedy HVI procedure](#concept-collaborative-batch-selection-via-global-utility-maximization).
    - Evaluate $`\{ \boldsymbol{x}_1, \cdots, \boldsymbol{x}_q \}`$
    - Update budget count.
    - `for` $`i\in\{1,\cdots,n_{\text{TR}}\}`$
      - Pick $`n_{\text{TR}}`$ centers using the [HVC](#concept-coordinated-trust-region-center-selection)
      - Update $`\mathcal{T}_i`$
      - Count Success/Failure within $`\mathcal{T}_i`$
      - Update edge length $`L_i`$
      - `if` $`L_i\lt L_{\text{min}}`$ `then`
        - [Re-initialize](#concept-re-initialization-strategy) $`\mathcal{T}_i`$ centered on $`x_t = \displaystyle\arg\max_{x\in\mathcal{X}} s_{\boldsymbol{\lambda}}\left[ \tilde{f}_{t-1}(x) \right]`$.
        - Set $`\mathbf{X}_t = \mathbf{X}_{t-1} \cup \{x_t\}`$