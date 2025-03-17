---
marp: true
style: |
  section {
    margin-top: 5px; /* 전체 슬라이드의 윗 마진을 50px로 설정 */
    line-height: 1;
  }
  .left { text-align: left; }
  .center { text-align: center; }
  .right { text-align: right; }


  .container {
    display: flex;
    align-items: center;
  }
  .image {
    width: 45%;
    margin-right: 5%;
  }
  .text_left {
    text-align: left;
    width: 45%;
  }


  .container2 {
    display: flex;
    align-items: center;
  }
  .div {
    text-align: left;
    width: 70%;
    margin-right: 5%;
  }
  .img {
    width: 27%;
  }


  .vert_center {
    display: flex;
    flex-direction: column; /* 세로 방향 정렬 */
    align-items: center; /* 가로 중앙 정렬 */
  }
  .div_cent {
    width: 100%
  }
  .div_part {
    width: 50%
  }
---

# DGEBO
## Summary
- Adding more Bayesian Optimization properties to DGEMO may improve its performance.

---
### How?
#### 1. Treating the multivariate objective as stochastic.
- As is) 
  - $\tilde{F} = (\mu_1, \cdots, \mu_d) \text{ where } \mu_j = k_jK_j^{-1}Y \forall j$
- To be)
  - $\tilde{F} \sim N(\mu, \Sigma)$



#### 2. Modify Performance Buffer Sampling to use a BO approach
- As is) 
  1. $\displaystyle\mathbf{x}_s = \mathbf{x}^j + \frac{1}{2^{\delta_p}}\mathbf{d}_p$
  2. $\displaystyle\mathbf{x}_o = \arg\min_{\mathbf{x}\in\mathcal{X}} \Vert F(\mathbf{x}) - \mathbf{z(x_s)} \Vert^2$
- To be)
  1. Expected Improvement with Information Gain 
  2. Use distance metric for probability distribution : KL Divergence, Mutual Information, Wasserstein Distance


---


# DGEMO Review
- MOO problem with $F = (f_1, \cdots, f_d)$
- Use GP as a surrogate model of $F$ as    
  - $\tilde{F} = (\tilde{f}_1, \cdots, \tilde{f}_d) \quad \text{ where } \tilde{f}_j \sim N(m_j, k_j),\quad \begin{cases} m_j=0 \\ k_j \text{ is a Matern Kernel, } \end{cases} \forall j$
- Use the mean function as the acquisition function.
  - $\tilde{f}_j = \mu_j = k_jK_j^{-1}Y \forall j$
- Use affine subspaces $\mathcal{A}_i$ near the samples derived with the First-Order Approximation.
  - Jacobian and Hessian of $\mu_j$
- Use batch selection $X_B$ to run parallel when deriving the final Pareto front.

---

# DGEMO's Limit
### 1. Not fully utilizes the GP.
- Simply using the mean function $\mu_j$ as the acquisition function.
- Discarding the variance function $\sigma^2_j$ might be wasting the valuable info.

### 2. Arbitrary Sampling procedure in the First-Order Approximation.
- From the previous candidate $\mathbf{x}_j$ in the performance buffer $B(j)$, it generates the new sample $\mathbf{x}_s$ as
  - $\displaystyle\mathbf{x}_s = \mathbf{x}^j + \frac{1}{2^{\delta_p}}\mathbf{d}_p$ where $\mathbf{d}_p$ is a uniform random unit vector that defines the stochastic direction

### 3. Treats $\tilde{F}$ as definitive but in reality it is stochastic.
- When optimizing the newly generated sample is uses the single objective of 
  - $\mathbf{x}_o = \arg\min_{\mathbf{x}\in\mathcal{X}} \Vert F(\mathbf{x}) - \mathbf{z(x_s)} \Vert^2$

---

# Suggestion : DGEBO
### 1. What if we treat $\tilde{F} \sim N(\mu, \Sigma)$ as we did in BO.
- According to the assumption of the model, each $f_j$ was independent of each other.

---

#### 1-1.  Since we want to define $\tilde{F}$ to be stochastic, the following optimization problem should be modified as well.
- $\displaystyle\mathbf{x}_o = \arg\min_{\mathbf{x}\in\mathcal{X}} \Vert F(\mathbf{x}) - \mathbf{z(x_s)} \Vert^2$
- Why doing this?)
    - The reason that we are optimizing this is to make our sample closer to the Pareto Front.
    - Zeleny's Compromise Programming says using various weightings and distance functions $L_p$ norms may obtain efficient solutions close to the ideal point.
    - Schulz et al. used the $L_2$ Norm.
- Problem)
    - $F$ is not deterministic anymore.

---
- Sol?)
  - Use Distance Metrics for Probability Distributions
    - KL Divergence : $KL(\tilde{F}, \delta(\mathbf{z(x_s)}))$
    - Mutual Information
    - Wasserstein Distance?
      - $\displaystyle W_2(P, Q) = \left( \inf_{\gamma \in \Pi(P, Q)} \int_{\mathcal{X} \times \mathcal{X}} \| x - y \|^2 \, d\gamma(x, y) \right)^{\frac{1}{2}}$
  - Making $\mathbf{z(x_s)}$ a probability distribution?
    - Dirac Delta : $\mathbf{z(x_s)} \sim \delta(\mathbf{z(x_s)})$
    - Gaussian : $\mathbf{z(x_s)} \sim N(\mathbf{z(x_s)}, \sigma^2)$



#### 1-2. First order approximation should be changed as well.
- Deterministic $F$
  - Calculate the Jacobian and Hessian of $\mu$
- Stochastic $F$
  - We should get the Jacobian and Hessian of $F\sim N(\mu, \Sigma)$
    - Is this possible? Gaussian, so yeah?

---



### 2.  When sampling a new point in the performance buffer, what if we use BO acquisition function such as EI?

#### As is) 
1. $\displaystyle\mathbf{x}_s = \mathbf{x}^j + \frac{1}{2^{\delta_p}}\mathbf{d}_p$
2. $\displaystyle\mathbf{x}_o = \arg\min_{\mathbf{x}\in\mathcal{X}} \Vert F(\mathbf{x}) - \mathbf{z(x_s)} \Vert^2$
#### To be)
1. Expected Improvement with Information Gain 
2. Use distance metric for probability distribution : KL Divergence, Mutual Information, Wasserstein Distance

- Additionally, what if we update the $F$'s posterior using these new samples?
  - Data structure)
    - Maintain a hashmap of $F=(f_1, \cdots, f_d)$

---

# Possible Costs and Improvements?
### 1. Treating $F$ to be stochastic may be more expensive than treating it to be deterministic.
- Still, the key performance boosting part of DGEMO was the batch selection strategy.
- These procedures take place before the batch selection.
- Thus, we can still take advantage of the DGEMO's efficiency.

### 2. Treating the multivariate stochastic function $F\sim N(\mu, \Sigma)$
- Is this set up compatible with the performance buffer set up in DGEMO?
- Is the new sampling scheme compatible with this?

---

### 3. Will this more BO style of approach have advantage?
  - Maybe yes?
  - DGEMO achieved diversity by...
    - Initial LHS sampling
    - Setting $\mathbf{z(x_s)} = \mathbf{x}_s + \mathbf{s(x_s)}C(\mathbf{x}_s)$ where 
      - $\mathbf{s(x_s)}$ was a unit length search direction pushing $\mathbf{x}_s$ toward the Pareto front $\mathcal{P}$
      - Selected the search direction assigned to a cell on the neighborhood of cell $j$ selected uniformly at random, within distance $\delta_N$
    - Using affine subspace around the candidates in the performance buffer as the fraction of the Pareto Frontier
  - We do not change any of those key factors.
    - So we may still take advantage of DGEMO.
    - May BO enhance this even more?