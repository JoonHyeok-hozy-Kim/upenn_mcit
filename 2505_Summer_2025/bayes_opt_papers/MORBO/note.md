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
- 