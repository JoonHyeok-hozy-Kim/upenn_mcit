# Edit Flows: Flow Matching with Edit Operations
[@havasiEditFlowsFlow2025]

## 2. Preliminaries
### 2.1 Continuous-time Markov Chains (CTMC)
- Def.)
  - Settings)
    - $`\mathcal{X}`$ : a discrete space
  - The Markov process generates the trajectories $`(X_t)_{t\in[0,1]}`$ over $`\mathcal{X}`$ as
    - $`\mathbb{P}(x_{t+h}=x \mid X_t=x_t) = \delta_{x_t}(x) + h u_t(x\mid x_t) + o(h)`$
      - where
        - $`u_t`$ : a rate denoting the infinitesimal transition probabilities from a state $`x_t`$ to any other state $`x`$ at time $`t`$
        - $`o(h)`$ s.t. $`\displaystyle\lim_{h\rightarrow0}\frac{o(h)}{h} = 0`$