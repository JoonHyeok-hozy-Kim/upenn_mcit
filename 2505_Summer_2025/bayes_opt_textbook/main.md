# Bayesian Optimization 
Roman Garnet


|No|Subject|Keywords|
|:-:|:-|:-|
| 1|[Introduction](./notes/01.md)|- Observation Model <br> - Prior, Likelihood, Posterior <br> - Stochastic Process, Posterior Process, Posterior Predictive Distribution|
| 2|[Gaussian Process](./notes/02.md)|- Gaussian Process $`\mathcal{GP}(f;\mu,K)`$, GP Prior, GP Posterior <br> - GP Inference with Exact Function Evaluations <br> - Joint Gaussian Process on Combined Functions <br> - Sample Path <br> --- Sample Path Continuity, Continuity in Mean Square <br> --- Sample Path Differentiability, Differentiability in Mean Square <br> - Bayesian Quadrature (Monte Carlo) <br> - Global Maximum of GP <br> - Non-Gaussian Inference : Factorization, Monte Carlo Sampling, Approximation (Laplace Approximation, Gaussian Expectation Propagation, Variational Inference)|
| 3|[Modeling with Gaussian Process](./notes/03.md)|- Prior Mean Function $`\mu(x)`$ : Interpolation vs Extrapolation, Constant Mean <br> - Prior Covariance Function $`K(x,x')`$: Stationary, Isotropic, Bochner Thm. <br> --- Matérn Family, Spectral Mixture, Linear Covariance <br> --- Output Space Scaling (Length Scale), Domain Transformation (ARD), General Transformation (Mahalanobis distance), Nonlinear Warping, Combining Covariance Functions <br> - Neural Imbedding|
| 4|[Model Assessment, Selection, and Averaging](./notes/04.md)|- Model Assessment: Latent Function, Model Space, Hyperparameter <br> --- Model Prior $`p(\boldsymbol{\theta})`$, Model Posterior $`p(\boldsymbol{\theta}\mid\mathcal{D})`$, Model Evidence (Model Likelihood) $`p(\mathbf{y\mid x},\boldsymbol{\theta})`$, Bayesian Occam's Razor <br> - Model Selection: Maximum A Posteriori (MAP) <br> - Model Averaging: $`p(f\mid\mathcal{D})`$, $`p(y\mid x,\mathcal{D})`$, Monte Carlo (HMC, NUTS) <br> - Multiple Model Structures|
| 5|[Decision Theory for Optimization](./notes/05.md)|- Sequential Optimization <br> - Acquisition Function <br> - Isolated Decision, Decision Horizon $`(\tau)`$ <br> - Bellman Optimality $`O(n^\tau q^\tau)`$, Approximate Dynamic programming: Limited Lookahead, One Step Lookahead, Rollout <br> - Termination, Cost-Aware Optimization|
| 6|[Utility Functions for Optimization](./notes/06.md)|- Terminal Recommendation <br> - Utility Function <br> --- Risk-Tolerance : Neutral, Averse, Seeking $`\mu+\beta\sigma`$, Certainty Equivalent <br> - Expected Utilities: Simple Reward, Global Reward, Cumulative Reward, Information Gain|
| 7|[Common Bayesian Optimization Policies](./notes/07.md)|- Multi-Armed Bandit, One-Step Lookahead <br> - Acquisition Functions <br> --- Expected Improvement (EI) <br> --- Knowledge Gradient (KG) <br> --- Probability of Improvement (PI) <br> --- Mutual Information (MI) $`I(\omega,\psi)`$ for Entropy Search <br> --- Principle of Optimism : Upper Confidence Bound (UCB) <br> --- Thompson Sampling (TS)|
| 8|[]()|- |
| 9|[]()|- |
|10|[]()|- |