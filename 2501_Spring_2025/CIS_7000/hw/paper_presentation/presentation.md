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

# Diversity-Guided Efficient Multi-Objective Bayesian Optimization with Batch Evaluations (DGEMO)

Luković et al.

<br><br><br><br><br>
<div class="right">Reviewed & Presented by Joon Hyeok Kim</div>

---
## Three-Minute Summary of DGEMO

<br>
<div class="center">

$\text{DGEMO solves } \underbrace{\text{multi-objective problem}}_{\text{MOO}} \text{ with } \underbrace{\text{black-box functions}}_{\text{BO}} \text{ } \underbrace{\text{fast in parallel}}_{\text{MOBO}}.$

</div><br>

### 1. Bayesian Optimization (BO)
- Gaussian Process


### 2. Multi-Objective Optimization (MOO)
- Pareto Frontier Approximation
- Hypervolume Indicator


### 3. Multi-Objective Bayesian Optimization (MOBO)
- Diversity Region Batch Selection : Running in parallel = Faster!
---


<div class="container2">
  <div class="div"> 

# Outline
0. Problem Setting : Multi-Black-Box-Objective Problem
2. Derive surrogate objective functions using GP (BO)
3. Approximate Pareto Front using KKT (MOO)
4. Run parallel using batch selection (MOBO)
5. Evaluate Pareto Fronts using HVI (MOO)

  </div>
  <img src="./images/pre 002.png"  class="img">
</div>


---
### What is the problem?
<br>
<div class="center"> 

Let's start with the Multi-~~Black-Box~~-Objective Problem first. 

</div>
<br>
<div class="container">
  <img src="./images/gardner%20002.png" class="image">
  <p class="text_left"> Recall the example of finding the optimal molecule for medicine. 
</div>

---


<br>
<div class="container">
  <img src="./images/gardner%20002.png" class="image">
  <p class="text_left">  <b>Main Issue : Conflicting Objectives</b><br><br> i.e.) Trade-off between Objectives <br><br> e.g.) The more effective, the more unstable, costly, severe side effect <br><br><img src="./images/pre 006"> </p>
</div>

---
How can we measure and compare the performance in a Multi-Objective case?

#### Rule) Pareto Optimality
$x^*$ is Pareto optimal if there is **no** other solution $x$ s.t. $\left\{\begin{array}{l} f_i(x) \ge f_i(x^*) & \forall \text{ objective } i \\ f_i(x) \gt f_i(x^*) & \exists \text{ objective } i \\  \end{array}\right.$

![h:400px](./images/gardner%20001.png)

---

#### Concept) Pareto Frontier
- The set of Pareto optimal points, i.e. the solution set of MOO.
  
|Maximization Problem| Minimization Problem|
|:-:|:-:|
|![](./images/pre%20003.png)|![](./images/pre%20004.png)|

---

### The Goal of Multiple Objective Optimization Problem
- Get the Pareto front.
  - i.e.) the set of Pareto optimal solutions
- Points on the Pareto fronts are better off than other available points.

![h:400px](./images/gardner%20001.png)

---
## Problem Setting) Multiple Objective Problem

#### Def.) Design Space and Constraints
- $\mathcal{X} = \{\mathbf{x}=(x^1, \cdots, x^D) \in\mathbb{R}^D : g_j(\mathbf{x})\le0, \forall j\in\{1,\cdot, K\} \}$
  - the design space where
    - $g_j$ represents a single constraint on $\mathbf{x}$
    - $G(\mathbf{x}) : \mathbb{R}^D \rightarrow \mathbb{R}^K \text{ s.t. } G(\mathbf{x}) = (g_1(\mathbf{x}), \cdots, g_K(\mathbf{x}))$
      - the concatenation of the $K$ constraints

#### Def.) Performance metric and Space
- $f_i : \mathbb{R}^D \rightarrow \mathbb{R}$ : the $i$-th performance metric (objective function)
- $F(\mathbf{x}) : \mathbb{R}^D \rightarrow \mathbb{R}^d \text{ s.t. } F(\mathbf{x}) = (f_1(\mathbf{x}), \cdots, f_d(\mathbf{x}))$
  - the concatenation of the $d$ performance metrics
  - $d\ge2$ : multi-objective problem!
  - $d \ll D$
- $\mathcal{S} = F(\mathcal{X}) \subseteq\mathbb{R}^d$ : the performance space

---
## Be Careful : Design Space vs Performance Space

<br>


<div class="vert_center">
<div class="div_cent">

|Design Space: $\mathcal{X}\subseteq\mathbb{R}^D$|$\rightarrow$|Performance Space: $F(\mathcal{X}) \subseteq\mathbb{R}^d$|
|:-|:-:|:-|
|$\mathbf{x} = (x_1, \cdots, x_D)$|$F : \mathbb{R}^D\rightarrow\mathbb{R}^d$<br><br>$F=(f_1,\cdots,f_d)$|$F(\mathbf{x}) = (f_1(\mathbf{x}),\cdots,f_d(\mathbf{x}))$|
|- Sample in here: $\mathbf{x}_s$ <br>- Solution is in here: $\mathbf{x}^*$<br>- $B(j) = \{\mathbf{x}_{j1}, \cdots,\mathbf{x}_{jK}\}$||- Performance : $F(\mathbf{x}_s), F(\mathbf{x}^*)$ <br>- Pareto Front : $\mathcal{P}_f$|

</div>

<div class="div_part">

<img src="./images/interactive%20008.png">

</div>
</div>

---



<div class="container2">
  <div class="div"> 

# Outline
0. Problem Setting : Multi-Black-Box-Objective Problem
2. **v Derive surrogate objective functions using GP (BO)**
3. Approximate Pareto Front using KKT (MOO)
4. Run parallel using batch selection (MOBO)
5. Evaluate Pareto Fronts using HVI (MOO)

  </div>
  <img src="./images/pre 002.png"  class="img">
</div>


---
Going back to the Multi-**Black-Box**-Objective Problem

<br><br>

### Why BO?
Because the objectives are the Black-Boxes! (i.e. We don't even know the objectives...)


### How?
Use **Gaussian Process** (GP) as a surrogate model to each objective independently.

1. For each objective $f_j$, get **Posterior** $\tilde{f}_j\sim N(m_j, k_j)$ using **Prior** and **Likelihood**
2. Take only the mean function and use it as the objective function.


### Difference
Only one update on Posterior and apply batch selection (TBD).

---

For the $j$-th  independent objective function $f_j, \;\forall j\in\{1, \cdots, m\}$...

### 1. Prior
- Any prior beliefs about the objective functions if available, without depending on the input data
- Mean Function : $m_j : \mathcal{X}\rightarrow\mathbb{R}$
  - $m_j(\mathbf{x}) = 0$
- Kernel Function $k_j : \mathcal{X}\times\mathcal{X} \rightarrow \mathbb{R}$
  - Matern Kernel
    - $\displaystyle k(\mathbf{x},\mathbf{x}') = \sigma^2 \frac{2^{1-\nu}}{\Gamma(\nu)}\left(\sqrt{2\nu}\frac{\|\mathbf{x}-\mathbf{x}'\|}{\ell}\right)^\nu K_\nu\left(\sqrt{2\nu}\frac{\|\mathbf{x}-\mathbf{x}'\|}{\ell}\right)$
      - where
        - $\sigma^2$ : the variance parameter
        - $\ell$ : the length scale
        - $\nu$ : the smoothness parameter
        - $K_\nu$ : the modified Bessel function

---
### 2. Likelihood) 
- The log marginal likelihood of  on the available dataset $\{X, Y\}$
- $p_j(\mathbf{y}\vert\mathbf{x},\theta_j)$ : 
  - where $\theta_j$ is the parameters set
    - i.e.) $\sigma^2, \ell, \nu \in \theta_j$
  - Recall, $\mathbf{y} = \mathbf{f}(\mathbf{x}) + \epsilon$


### 3. Posterior
- $\tilde{f_j} \sim N(\mu_j(\mathbf{x}), \Sigma_j(\mathbf{x}, \mathbf{x}))$
  - where
    - $\begin{cases} \mu_j(\mathbf{x}) = m_j(\mathbf{x}) + k_jK_j^{-1}Y = k_jK_j^{-1}Y \\ \Sigma_j(\mathbf{x}) = k_j(\mathbf{x}, \mathbf{x}) - k_jK_j^{-1}k_j^\top \end{cases} \text{ for } \begin{cases} k_j=k_j(\mathbf{x}, X) \\ K_j = k_j(X, X) \end{cases}$

---

## Result
$d$ objective functions derived by GP as...

<br><br>

$\begin{aligned}
    \tilde{F}(\mathbf{x}) &= (\tilde{f}_1(\mathbf{x}),\cdots, \tilde{f}_d(\mathbf{x})) \\
    &= (\mu_1(\mathbf{x}),\cdots, \mu_d(\mathbf{x})) \\
\end{aligned}$

<br><br>

where    $\begin{aligned} \mu_j(\mathbf{x}) &= m_j(\mathbf{x}) + k_jK_j^{-1}Y = k_jK_j^{-1}Y & (\because m_j(\mathbf{x})=0) \end{aligned}$

---



<div class="container2">
  <div class="div"> 

# Outline
0. Problem Setting : Multi-Black-Box-Objective Problem
1. Derive surrogate objective functions using GP (BO)
2. **V Approximate Pareto Front using KKT (MOO)**
3. Run parallel using batch selection (MOBO)
4. Evaluate Pareto Fronts using HVI (MOO)

  </div>
  <img src="./images/pre 002.png"  class="img">
</div>


---




<div class="container2">
  <div class="div"> 

Thanks to GP, now we can do MOO because we know the multiple objectives $\tilde{F} = (\tilde{f}_1, \cdots, \tilde{f}_d)$

<br>

MOO's goal was to get the Pareto Front $\mathcal{P}_f$

<br>

  </div>
  <img src="./images/pre 002.png"  class="img">
</div>

---

### Problem) Pareto Front is not nice and friendly in higher-dimensional space
Recall that we had $d$ number of objectives.   
Thus, the performance space is the subset of $\mathbb{R}^d$.

|$d=2$|$d=10$|
|:-:|:-:|
|![height:400px](./images/pre%20003.png)|![height:400px](./images/pre%20005.jpeg)|

---

### Solution) Pareto Front Approximation
Schulz et al. "Interactive Exploration of Design Trade-Offs"


![height:500px](./images/interactive%20002.png)

---


![height:450px](./images/interactive%20001.png) ![height:450px](./images/interactive%20004.png)

---

### Question
But how can we find the intersection point between the ray and the Pareto front if we don't know what the Pareto frontier looks like?

<br>

### Solution
- Performance Buffer (Buffer Cell)
- Stochastic Sampling
- Local Optimization
---

### Performance Buffer (Buffer Cell)
- $(d-1)$-dimensional array discretized using (hyper)spherical coordinates
- Partitions the **performance space** $\mathcal{S}=F(\mathcal{X}) \subseteq\mathbb{R}^d$
- Each buffer cell $B(j)$ contains a list of candidate solutions.
  - Why multiple candidates?)
    - Useful for extracting sparse approximation of the Pareto front
    - top $K(=50)$ candidates   
      ![](./images/interactive%20011.png) 

---

#### Stochastic Sampling
- Goal)
  - Choose $N_S$ samples in the design space $\mathcal{X}$
    - i.e.) $\mathbf{x}_s^i \in\mathcal{X}$ for $i=1,\cdots,N_S$
- How?)
  - First iteration
    - Uniformly sample from $\mathcal{X}$
  - Rest of the iteration
    - $B(j)$ from the previous state contains the point $\mathbf{x}^j$ with the minimal distance to the origin.
    - Sample $\displaystyle\mathbf{x}_s = \mathbf{x}^j + \frac{1}{2^{\delta_p}}\mathbf{d}_p$
      - where
        - $\delta_p$ : a uniform random number in $[0, \delta_p]$ (Scaling Factor)
        - $\mathbf{d}_p$ : a uniform random unit vector that defines the stochastic direction

---

![](./images/pre%20005.png)

---

#### Local Optimization
- Goal)
  - For each $\mathbf{x}_s^i$, find $\mathbf{x}_o^i$ that optimizes for Pareto optimality
  - A scalarization scheme is used to convert the problem into **single** objective problem
    - **Diversification** is essential to avoid having solutions cluster in certain areas, failing to provide a good representation for the shape.
            ![](./images/interactive%20010.png)

---


- Problem Setting)
  - $\displaystyle\mathbf{x}_o = \arg\min_{\mathbf{x}\in\mathcal{X}} \Vert F(\mathbf{x}) - \mathbf{z(x_s)} \Vert^2$
    - where $\mathbf{z(x_s)} \in\mathbb{R}^d$ is a reference point defined for each sample.
      - This paper uses $\mathbf{z(x_s)} = \mathbf{x}_s + \mathbf{s(x_s)}C(\mathbf{x}_s)$
        - where 
          - $\mathbf{s(x_s)}$ is a unit length search direction pushing $\mathbf{x}_s$ toward the Pareto front $\mathcal{P}$
            - Select the search direction assigned to a cell on the neighborhood of cell $j$ selected uniformly at random, within distance $\delta_N$
          - $C(\mathbf{x}_s) = \delta_s \Vert \mathbf{x}_s \Vert$ is a scaling factor depending on the distance to origin   
            - Key to **Diversification**!

---

![](./images/pre%20007.png)

---

Our initial goal was to perform **First-Order approximation** from a point on the $\mathcal{P}_f$.

<br>

Instead of a single point on $\mathcal{P}_f$, we have $K$ candidates in $B(j)$ that are the closest to $\mathcal{P}_f$.

<br>

Let's perform **First-Order approximation** using our candidates.

---

### First-Order Approximation of the Pareto Front
- Goal)
  - For each point $\mathbf{x}_o^i$ perform first-order approximation to find an affine subspace $\mathcal{A}_i$ around $\mathbf{x}_o^i$
  - $\mathcal{A}_i$ is stored in a matrix $M^i$ which is defined by the $d-1$ directions for local exploration.
    - e.g.) $\mathcal{A}_i$ in $D=3$ design space   
      ![height:350px](./images/interactive%20012.png)

---
- How?)
  - First-order approximation using the KKT condition.
  - The output is the affine subspace $\mathcal{A}_i$ represented by the point $\mathbf{x}_o$ and the bases represented by the matrix $M_i$

![](./images/interactive%20008.png)

---

![](./images/pre%20008.png)

---
<!-- _backgroundColor: #ADD8E6 -->

### KKT Conditions
  - Assuming that 
    - $f_i$ and $g_k$ are continuously differentiable 
    - the vectors $\{ \nabla g_{k'}(\mathbf{x}^*) \vert k' \text{ is an index of an active constraint} \}$ are linearly independent
  - Then for any solution $\mathbf{x}^*$ to $\displaystyle\min_{x} f_i(x) \text{ s.t. } x\in\mathcal{X}$
    - there exists dual variables $\alpha\in\mathbb{R}^d$ and $\beta\in\mathbb{R}^K$ s.t.   
      $\left\{ \begin{array}{ll}
          \mathbf{x}^* \in \mathcal{X} \\
          \alpha_i \ge 0, \; \forall i\in \{1, \cdots, d\} \\
          \beta_k \ge 0, \; \forall k\in \{1, \cdots, K\} & \text{: Dual Feasibility} \\
          \beta_k g_k(\mathbf{x}^*) = 0, \; \forall k\in \{1, \cdots, K\} & \text{: Complementary Slackness Condition} \\
          \sum_{i=1}^d \alpha_i = 1 \\
          \sum_{i=1}^d \alpha_i \nabla f_i(\mathbf{x}^*) + \sum_{k=1}^K \beta_k \nabla g_k(\mathbf{x}^*) = 0 & \text{: Stationary Condition} \\
      \end{array} \right.$

---
<!-- _backgroundColor: #ADD8E6 -->

### KKT Perturbation

- Suppose $\mathbf{x}(t)\in\mathcal{P}, \; \forall t\in(-\epsilon, \epsilon)$
  - $\mathbf{x}(t) : (-\epsilon, \epsilon) \rightarrow \mathbb{R}^D$ is in the Pareto set in a neighborhood of $t=0$.
- Taking $\alpha \in\mathbb{R}^D, \beta \in\mathbb{R}^K$ the KKT dual variables corresponding to $\mathbf{x}^* = \mathbf{x}(0)$, we have
  - $H \mathbf{x}'(0) \in \text{Im}\left( {DF}^\top (\mathbf{x}^*) \right) \oplus \text{Im}\left( {DG}_{K'}^\top (\mathbf{x}^*) \right)$
    - where
      - $\displaystyle H = \sum_{i=1}^d \alpha_i H_{f_i}(\mathbf{x}^*) + \sum_{k=1}^{K'} \beta_k H_{g_k}(\mathbf{x}^*) \in\mathbb{R}^{D\times D}$
        - for $\begin{cases} H_{f_i}(\mathbf{x}^*) = \begin{bmatrix} H_{f_1}(\mathbf{x}^*) \cdots H_{f_d}(\mathbf{x}^*) \end{bmatrix} \in \mathbb{R}^{D\times D\times d} & \text{Hessian of } F(\mathbf{x}) \\ H_{g_k}(\mathbf{x}^*) = \begin{bmatrix} H_{g_1}(\mathbf{x}^*) \cdots H_{g_K}(\mathbf{x}^*) \end{bmatrix} \in \mathbb{R}^{D\times D\times K}  & \text{Hessian of } G(\mathbf{x}) \\  \end{cases}$
      - $\mathbf{x}'(0)$ : the first order differentiation of $\mathbf{x}(t)$ on $t=0$

---
<!-- _backgroundColor: #ADD8E6 -->

- ${DF} (\mathbf{x}^*) = \begin{bmatrix} \nabla f_1(\mathbf{x}^*)^\top \\ \vdots \\ \nabla f_d(\mathbf{x}^*)^\top \end{bmatrix} \in\mathbb{R}^{D\times d}$ : the Jacobian of $F(\mathbf{x})$
- ${DG}_{K'} (\mathbf{x}^*) = \begin{bmatrix} \nabla g_1(\mathbf{x}^*)^\top \\ \vdots \\ \nabla g_{K}(\mathbf{x}^*)^\top \end{bmatrix} \in\mathbb{R}^{D\times K}$ : the Jacobian of $F(\mathbf{x})$
- $\text{Im}(\cdot)$ : the Span of the column space of $\cdot$


---
<!-- _backgroundColor: #ADD8E6 -->

#### First-order or Second order?
  - Recall the Second Order Taylor approximation
    - $f(x + x') \approx f(x) + \nabla f(x)^\top x' + \frac{1}{2}{x'}^\top H_f(x) x'$
  - Put $x' = \mathbf{x}'(0)$.
  - Then we have the second order term of  $\frac{1}{2}{\mathbf{x}'(0)}^\top \underbrace{H_f(x) \mathbf{x}'(0)}_{\text{here!}}$

#### Approximation using...
- $H_f(x)$ : the curvature of $f$   
- $\mathbf{x}'(0)$ : the direction of $f$

---
<!-- _backgroundColor: #ADD8E6 -->

### Property) First order approximation reduces the dimensionality!

<br>

$\mathbf{x}'(0) \in \mathbb{R}^{d+K'-1}$ where $K'$ is the number of active constraints.

<br>

- Meaning)
  - Recall that the design space was $\mathcal{X}\subseteq\mathbb{R}^D$.
  - This approximation reduces the dimension of the direction that $\mathbf{x}\in\mathcal{X}$ must move to $d+K'-1 \lt D$ dimension.
    - where
      - $d$ : the number of objective functions
      - $K'$ : the number of active constraints
  - Projecting on the subspace of Hessian and Jacobian.
    - cf.) Dimensionality reduction and PCA

---

Now, we have $K$ approximations for each performance buffer $B(j)$.

We want to choose the best $\mathcal{A}_j$ for each $B(j)$.

### Sparse Approximation
- Goal)
  - Among $K$ solution candidates for each buffer cell $B(j)$, find a single solution $\mathbf{x}_j^*$.
  - Output)
    - $\{(\mathbf{x}_1^*, \mathcal{A}_1^*), \cdots, (\mathbf{x}_{N_S}^*, \mathcal{A}_{N_S}^*)\}$
      - Single solution and a corresponding affine subspace for each buffer cell
        - i.e.) The Pareto Front!
    - $\displaystyle\bigcup_{j=1}^{N_S} \mathcal{A}_j \approx \text{Pareto Front}$

---

How?

### Use Graph-Cut Algorithm
- Criteria)
  - Continuity : adjacent cells share similar affine space $\mathcal{A}_i$
  - Optimality : Minimal distance to Origin
- Optimization using two constraints
  - $E_B(j,k)$ : binary term for continuity
    - How?) Lable $\mathcal{A}_i$ with $l_i$ and compare labels between solutions in adjacent buffer cells $B(j)$ and $B(k)$
  - $E_U(j,i)$ : unary term for optimality

---

![height:600px](./images/interactive%20013.png)

---

![](./images/)

---


---

### How to measure the improvement on Pareto Fronts

1. Calculate the initial Hypervolume of the Pareto Front.
2. Update the Pareto Front and calculate the Hypervolume again.
3. If the Hypervolume increased, then there was an improvement : HVI

<br>
<img src="./images/dgemo%20001.png" width="500px">

---
### Hypervolume
- Def.)
  - Let
    - $\mathcal{P}_f$ : a Pareto Front **approximation** in an $m$-dim'l performance space
    - $r\in\mathbb{R}^m$ : a reference point
      - i.e.) a fixed point deliberately chosen so that its performance is “inferior” to that of all candidate solutions (or Pareto-optimal solutions)
  - Then the **hypervolume** $\mathcal{H}(\mathcal{P}_f)$ is defined as   
    - $\displaystyle \mathcal{H}(\mathcal{P}_f) = \int_{\mathbb{R}^m} \mathbb{1}_{H(\mathcal{P}_f)}(z)dz$ 
      - where 
        - $H(\mathcal{P}_f) = \{ z\in Z \;\vert\; \exists1\le i\le \vert\mathcal{P}_f \vert : r \preceq z \preceq \mathcal{P}_f(i) \}$
        - $\mathcal{P}_f(i)$ : the $i$-th solution in $\mathcal{P}_f$
        - $\preceq$ : the relation operator of objective dominance
        - $\mathbb{1}_{H(\mathcal{P}_f)} = \begin{cases} 1 & \text{ if } z\in H(\mathcal{P}_f) \\ 0 & \text{ otherwise} \end{cases}$ : a Dirac Delta function

---


<div class="container2">
<div class="div">


### Hypervolume Improvement (HVI)
- Def.)
  - $\text{HVI}(P, \mathcal{P}_f) = \mathcal{H}(\mathcal{P}_f \cup P) - \mathcal{H}(\mathcal{P}_f)$
- Meaning)
  - How much the hypervolume would **increase** if a set of new points $P(\mathbf{p}_1, \cdots, \mathbf{p}_n)\subset\mathbb{R}^m$ is added to the current Pareto front approximation $\mathcal{P}_f$   

</div>

</div>

---