# Distillation of Discrete Diffusion through Dimensional Correlations
[@hayakawaDistillationDiscreteDiffusion2025]

# 2 Preliminaries
## 2.1 Discrete Diffusion Models
- Settings)
  - $`q_0 := q_\text{data}`$ : a data distribution over the space $`\mathcal{X}`$
  - Markov Process $`(x_t)_{0\le t\le T}`$
    - where
      - $`x_0\sim q_0`$
      - $`x_T\sim q_T`$ s.t. $`q_T`$ is tractable
- Goal)
  - Generate samples from $`q_{0\mid T}(\cdot\mid x_T)`$ with $`x_T \sim q_T`$
- Model) Denoiser
  - $`p_{s\mid t}`$ to approximate $`q_{s\mid t}`$