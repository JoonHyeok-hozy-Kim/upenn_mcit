# DGEBO
### Summary
- Adding more Bayesian Optimization properties to DGEMO may improve its performance.
  - How?)
    - Treating the multivariate objective as stochastic.
      - As is) 
        - $`\tilde{F} = (\mu_1, \cdots, \mu_d)`$
      - To be)
        - $`\tilde{F} = (\tilde{f}_1, \cdots, \tilde{f}_d)`$ where $`\tilde{f}_j \sim N(\mu_j, \Sigma_j)`$
    - Performance Buffer Sampling using a BO approach, such as employing EI as the acquisition function.
      - As is) 
        - $`\displaystyle\mathbf{x}_s = \mathbf{x}^j + \frac{1}{2^{\delta_p}}\mathbf{d}_p`$
      - To be)
        - Expected Improvement with Information Gain 


<br>

### DGEMO Description
- MOO problem with $`F = (f_1, \cdots, f_d)`$
- Use GP as a surrogate model of $`F`$ as    
  - $`\tilde{F} = (\tilde{f}_1, \cdots, \tilde{f}_d) \quad \text{ where } \tilde{f}_j \sim N(m_j, k_j),\quad \begin{cases} m_j=0 \\ k_j\text{ is a Matern Kernel, } \end{cases} \forall j `$
- Use the mean function as the acquisition function.
  - $`\tilde{f}_j = \mu_j = k_jK_j^{-1}Y \forall j`$
- Use affine subspaces near the samples derived with the First-Order Approximation.
  - Jacobian and Hessian of $`\mu_j`$
- Use batch selection to run parallel when deriving the final Pareto front.

<br>

### DGEMO Limit
- Not fully utilizes the GP.
  - Simply using the mean function $`\mu_j`$ as the acquisition function.
  - Discarding the variance function $`\sigma^2_j`$ might be wasting the valuable info.
- Arbitrary Sampling procedure in the First-Order Approximation.
  - From the previous candidate $`\mathbf{x}_j`$ in the performance buffer $`B(j)`$, it generates the new sample $`\mathbf{x}_s`$ as
    - $`\displaystyle\mathbf{x}_s = \mathbf{x}^j + \frac{1}{2^{\delta_p}}\mathbf{d}_p`$
      - where $`\mathbf{d}_p`$ is a uniform random unit vector that defines the stochastic direction
- Treats $`\tilde{F}`$ as definitive but in reality it is stochastic.
  - When optimizing the newly generated sample is uses the single objective of 
    - $`\displaystyle\mathbf{x}_o = \arg\min_{\mathbf{x}\in\mathcal{X}} \Vert F(\mathbf{x}) - \mathbf{z(x_s)} \Vert^2`$

<br>

### Suggestion : DGEBO
- What if we treat $`\tilde{F} \sim N(\mu, \Sigma)`$ as we did in BO.
  - According to the assumption of the model, each $`f_j`$ was independent of each other.
  - In this case can we use EI as the acquisition function?
- When sampling a new point in the performance buffer, what if we use BO acquisition function such as EI?
  - Additionally, what if we update the $`F`$'s posterior using this new sample?
    - Data structure)
      - Maintain a hashmap of $`F=(f_1, \cdots, f_d)`$
- Since we want to define $`\tilde{F}`$ to be stochastic, the following optimization problem should be fixed as well.
  - $`\displaystyle\mathbf{x}_o = \arg\min_{\mathbf{x}\in\mathcal{X}} \Vert F(\mathbf{x}) - \mathbf{z(x_s)} \Vert^2`$
    - Why doing this?)
      - The reason that we are optimizing this is to make our sample closer to the Pareto Front.
      - Zeleny's Compromise Programming says using various weightings and distance functions $`L_p`$ norms may obtain efficient solutions close to the ideal point.
      - Schulz et al. used the $`L_2`$ Norm.
    - Problem)
      - $`F`$ is not deterministic anymore.
    - Sol?)
      - Making $`\mathbf{z(x_s)}`$ a probability distribution?
        - Dirac Delta : $`\mathbf{z(x_s)} \sim \delta(\mathbf{z(x_s)})`$
        - Gaussian : $`\mathbf{z(x_s)} \sim N(\mathbf{z(x_s)}, \sigma^2)`$
      - KL Divergence, Mutual Information?
        - But $`\mathbf{z(x_s)}`$ is a point, not a distribution.
        - Can this be applied?
      - Wasserstein Distance?
        - $`\displaystyle W_2(P, Q) = \left( \inf_{\gamma \in \Pi(P, Q)} \int_{\mathcal{X} \times \mathcal{X}} \| x - y \|^2 \, d\gamma(x, y) \right)^{\frac{1}{2}}`$
- First order approximation should be changed as well.
  - Existing method of treating $`F`$ to be deterministic.
    - Calculate the Jacobian and Hessian of $`\mu`$
  - Stochastic Case)
    - We should get the Jacobian and Hessian of $`F\sim N(\mu, \Sigma)`$
      - Is this possible?
        - Gaussian, so yeah?

<br>

### Possible Cost and Concerns?
- Treating $`F`$ to be stochastic may be more expensive than treating it to be deterministic.
  - Still, the key performance boosting part of DGEMO was the batch selection strategy.
  - These procedures take place before the batch selection.
  - Thus, we can still take advantage of the DGEMO's efficiency.
- Treating the multivariate stochastic function $`F\sim N(\mu, \Sigma)`$
  - Is this set up compatible with the performance buffer set up in DGEMO?
  - Is the new sampling scheme compatible with this?
- Will this more BO style of approach have advantage?
  - Maybe yes?
  - DGEMO achieved diversity by...
    - Initial LHS sampling
    - Setting $`\mathbf{z(x_s)} = \mathbf{x}_s + \mathbf{s(x_s)}C(\mathbf{x}_s)`$ where 
      - $`\mathbf{s(x_s)}`$ was a unit length search direction pushing $\mathbf{x}_s$ toward the Pareto front $\mathcal{P}$
      - Selected the search direction assigned to a cell on the neighborhood of cell $j$ selected uniformly at random, within distance $\delta_N$
    - Using affine subspace around the candidates in the performance buffer as the fraction of the Pareto Frontier
  - We do not change any of those settings.
    - So we may still take advantage of DGEMO.
    - BO may enhance this?

<br><br>

# Dr. Gardner's Opinion
 