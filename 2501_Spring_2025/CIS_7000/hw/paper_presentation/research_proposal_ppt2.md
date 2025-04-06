---
marp: true
style: |
  section {
    margin-top: 5px;
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
    flex-direction: column;
    align-items: center;
  }
  .div_cent {
    width: 100%
  }
  .div_part {
    width: 50%
  }

  .flex-container {
    display: flex;
    align-items: center;
    justify-content: space-between;
  }

  .right-img {
    width: 500px;
    margin-left: 5px;
  }
---

# Enhancing DGEMO with Bayesian Optimization Properties: Towards DGEBO
## Summary
- Adding more Bayesian Optimization properties to DGEMO may improve its performance.

---

# How?

<div class="flex-container">
  <div>

#### 1. Treat $F$ as stochastic in the First-Order Approximation.
- As is) 
  - $\tilde{F} = (\mu_1, \cdots, \mu_d) \text{ where } \mu_j = k_jK_j^{-1}Y \forall j$
- To be)
  - $\tilde{F} \sim N(\mu, \Sigma)$
- Details will be covered later!

  </div>
  <img class="right-img" src="./images/pre%20013.png">
</div>

---

# How?
#### 2. Modify stochastic sampling to use a BO approach
|AS-IS|TO-BE|
|:-|:-|
|$\displaystyle\mathbf{x}_s = \mathbf{x}^j + \frac{1}{2^{\delta_p}}\mathbf{d}_p$ : random sampling|Exploration using BO acquisition functions such as EI.|

![height:300px](./images/pre%20005.png)

---

# DGEMO Review
- MOO problem with $F = (f_1, \cdots, f_d)$
- Use GP as a surrogate model of $F$:
  - $\tilde{F} = (\tilde{f}_1, \cdots, \tilde{f}_d) \quad \text{ where } \tilde{f}_j \sim N(m_j, k_j),\quad \begin{cases} m_j=0 \\ k_j \text{ is a Matern Kernel, } \end{cases} \forall j$
- Use the mean function as the acquisition function:
  - $\tilde{f}_j = \mu_j = k_jK_j^{-1}Y \forall j$
- Use affine subspaces $\mathcal{A}_i$ near samples derived from the First-Order Approximation:
  - Jacobian and Hessian of $\mu_j$
- Use batch selection $X_B$ to run in parallel when deriving the final Pareto front.

---

# DGEMO's Limits
### 1. Does not fully utilize the GP in First order Approximation
- Simply assumes $f_j$ equals $\mu_j$.
- Not fully utilizing the posterior variance $\Sigma^2_j$ may waste valuable information.

### 2. Arbitrary random sampling in the First-Order Approximation.
- From the previous candidate $\mathbf{x}_j$ in the performance buffer $B(j)$, a new sample $\mathbf{x}_s$ is generated as:
  - $\displaystyle\mathbf{x}_s = \mathbf{x}^j + \frac{1}{2^{\delta_p}}\mathbf{d}_p$, where $\mathbf{d}_p$ is a uniform random unit vector that defines a stochastic direction.

### 3. Treats $\tilde{F}$ as deterministic, though it is actually stochastic.
- $L_2$ norm optimization on the distribution : $\mathbf{x}_o = \arg\min_{\mathbf{x}\in\mathcal{X}} \Vert \tilde{F}(\mathbf{x}) - \mathbf{z(x_s)} \Vert^2$

---

# Suggestion: DGEBO
### 1. What if we treat $\tilde{F} \sim N(\mu, \Sigma)$ as we do in BO?
- We want to use the uncertainty information $\Sigma$ too!

---

## Related Treatments



#### 1-1. Approximate the Distribution and Sample from it.
- AS-IS 
  - Treat $F$ to be deterministic
    - i.e.) $\tilde{F}\approx[\mu_1, \cdots, \mu_d]$
  - Compute the Jacobian and Hessian of $\mu$.
  - Taylor approximation : $F(x+a) \approx \mu(x) + J_\mu(x)a + \frac{1}{2}a^\top H_\mu(x) a$
- TO-BE
  - Stochastic $\tilde{F}\sim N(\mu, \Sigma)$
  - Approximation using the distribution :    
    $F(x + a) \approx N\left( \underbrace{\mu(x) + J_\mu(x) a}_{\text{mean approx.}},\quad  \underbrace{\Sigma(x) + J_\Sigma(x) a}_{\text{var approx.}} \right)$

--- 

#### 1-2. Since we want to define $\tilde{F}$ as stochastic, the following optimization should also be modified:
- $\displaystyle\mathbf{x}_o = \arg\min_{\mathbf{x}\in\mathcal{X}} \Vert F(\mathbf{x}) - \mathbf{z(x_s)} \Vert^2$
- Why do this?
    - The goal of this optimization is to move the sample closer to the Pareto Front.
    - Zeleny's Compromise Programming suggests that using various weightings and distance functions (e.g., $L_p$ norms) can yield efficient solutions near the ideal point.
    - Schulz et al. used the $L_2$ norm.
- Problem:
    - $F$ is no longer deterministic.

---

- Solution?
  - Convert $\mathbf{z(x_s)}$ into a probability distribution:
    - Dirac Delta: $\mathbf{z(x_s)} \sim \delta(\mathbf{z(x_s)})$
    - Gaussian: $\mathbf{z(x_s)} \sim N(\mathbf{z(x_s)}, \sigma^2)$
  - Use distance metrics for probability distributions:
    - KL Divergence: $KL(\tilde{F}, \delta(\mathbf{z(x_s)}))$
    - Mutual Information
    - Wasserstein Distance:
      - $\displaystyle W_2(P, Q) = \left( \inf_{\gamma \in \Pi(P, Q)} \int_{\mathcal{X} \times \mathcal{X}} \| x - y \|^2 \, d\gamma(x, y) \right)^{\frac{1}{2}}$

---

### 2. When sampling a new point in the performance buffer, what if we use a BO acquisition function such as EI?

#### As is:
- $\displaystyle\mathbf{x}_s = \mathbf{x}^j + \frac{1}{2^{\delta_p}}\mathbf{d}_p$
#### To be:
- Expected Improvement with Information Gain

---

# Possible Costs and Improvements?
### 1. Treating $F$ as stochastic may be more expensive than treating it as deterministic.
- Check whether simple kernels like low-degree polynomials are effective.

### 2. Treating the multivariate stochastic function $F \sim N(\mu, \Sigma)$
- Is this setup compatible with the performance buffer in DGEMO?
- Is the new sampling scheme compatible?

---

### 3. Will this approach have advantages?
- More accurate approximation of the Pareto front may be possible.
- More efficient sampling using the BO approach.
- DGEMO's batch selection strategy is **not** affected by this approach:
  - The stochastic modification is applied only to the First-Order Approximation.
  - None of the following key factors are changed:
    - Initial LHS sampling
    - Local optimization on $\mathbf{z(x_s)} = \mathbf{x}_s + \mathbf{s(x_s)}C(\mathbf{x}_s)$
    - First-Order Approximation using the affine subspace
    - Graph-cut algorithm to ensure continuity
  - Therefore, we can still benefit from DGEMO's efficiency.
