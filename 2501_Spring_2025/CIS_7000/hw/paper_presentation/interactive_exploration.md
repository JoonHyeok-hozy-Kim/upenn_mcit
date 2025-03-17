# Interactive Exploration of Design Trade-Offs
Schulz et al.

- Summary
  - An algorithm to discover, represent, parameterize, and explore the Pareto front.
  - Uses the first-order approximation of the Pareto front derived from duality theory (KKT)
    - Idea)
      - Relatively few of the linear subspaces are needed to capture local variability
        - i.e.) Dimensionality of the Pareto front generally is lower than the dimensionality of performance space.

### 4.1 Definitions
#### Def. 4.1) Design Space and Constraints
- $`\mathcal{X} = \{\mathbf{x}=(x^1, \cdots, x^D) \in\mathbb{R}^D : g_j(\mathbf{x})\le0, \forall j\in\{1,\cdot, K\} \}`$
  - the design space where
    - $`g_j`$ represents a single constraint on $`\mathbf{x}`$
    - $`G(\mathbf{x}) : \mathbb{R}^D \rightarrow \mathbb{R}^K \text{ s.t. } G(\mathbf{x}) = (g_1(\mathbf{x}), \cdots, g_K(\mathbf{x}))`$
      - the concatenation of the $`K`$ constraints

<br>

#### Def. 4.2) Performance metric and Space
- $`f_i : \mathbb{R}^D \rightarrow \mathbb{R}`$ : the $`i`$-th performance metric
  - cf.)
    - Conventionally small values of $`f_i(\mathbf{x})`$ is desirable
- $`F(\mathbf{x}) : \mathbb{R}^D \rightarrow \mathbb{R}^d \text{ s.t. } F(\mathbf{x}) = (f_1(\mathbf{x}), \cdots, f_d(\mathbf{x}))`$
  - the concatenation of the $`d`$ performance metrics
  - $`d\ge2`$ : multi-objective problem!
  - $`d \ll D`$
- $`\mathcal{S} = F(\mathcal{X}) \subseteq\mathbb{R}^d`$ : the performance space

<br>

#### Def. 4.3) Pareto Optimality
- $`\mathbf{x}\in\mathcal{X}`$ is Pareto optimal if there does not exist any $`\mathbf{x}' \in\mathcal{X}`$ s.t. $`\begin{cases} f_i(\mathbf{x}) \ge f_i(\mathbf{x}'), \;\forall i \\ f_i(\mathbf{x}) \gt f_i(\mathbf{x}'), \;\exists i \end{cases}`$
- $`\mathcal{P}\subseteq\mathbb{R}^D`$ : the Pareto set
- $`F(\mathcal{P})\subseteq\mathbb{R}^d`$ : the Pareto front

<br><br>

### 4.2 KKT Conditions
#### Proposition 4.4) 
- Prop.)
  - For every nonnegative $`\alpha\in\mathbb{R}^d`$, there exists at most one $`t\gt 0`$ s.t. $`t\alpha \in \mathcal{P}`$
- Intuition)
  - Recall that for a pareto optimal point $`\mathbf{x}\in\mathcal{X}`$,
    - $`\begin{cases} f_i(\mathbf{x}) \ge f_i(\mathbf{x}'), \;\forall i \\ f_i(\mathbf{x}) \gt f_i(\mathbf{x}'), \;\exists i \end{cases}, \quad\forall \mathbf{x}'\in\mathcal{X}`$
  - If we draw a ray in performance space along which there can be at most one Pareto optimal point, it will reach that just one point on the Pareto frontier   
    ![](./images/interactive%20001.png)


#### Proposition 4.5) KKT Conditions
  - Assuming that 
    - $`f_i`$ and $`g_k`$ are continuously differentiable 
    - the vectors $`\{ \nabla g_{k'}(\mathbf{x}^*) \vert k' \text{ is an index of an active constraint} \}`$ are linearly independent
  - Then for any solution $`\mathbf{x}^*`$ to $`\displaystyle\min_{x} f_i(x) \text{ s.t. } x\in\mathcal{X}`$
    - there exists dual variables $`\alpha\in\mathbb{R}^d`$ and $`\beta\in\mathbb{R}^K`$ s.t.   
      $`\left\{ \begin{array}{ll}
          \mathbf{x}^* \in \mathcal{X} \\
          \alpha_i \ge 0, \; \forall i\in \{1, \cdots, d\} \\
          \beta_k \ge 0, \; \forall k\in \{1, \cdots, K\} & \text{: Dual Feasibility} \\
          \beta_k g_k(\mathbf{x}^*) = 0, \; \forall k\in \{1, \cdots, K\} & \text{: Complementary Slackness Condition} \\
          \sum_{i=1}^d \alpha_i = 1 \\
          \sum_{i=1}^d \alpha_i \nabla f_i(\mathbf{x}^*) + \sum_{k=1}^K \beta_k \nabla g_k(\mathbf{x}^*) = 0 & \text{: Stationary Condition} \\
      \end{array} \right.`$
- Idea)
  - The space of solutions that are Pareto-optimal is large, disconnected, and prone to local minima
  - But once we find a single point, we can easily uncover its neighborhood under certain conditions.
  - The paper uses dual problem approach for this.

<br><br>

### 5 First-Order Approximation
#### Proposition 5.1) KKT Perturbation
- Proposition)
  - Suppose $`\mathbf{x}(t)\in\mathcal{P}, \; \forall t\in(-\epsilon, \epsilon)`$
    - $`\mathbf{x}(t) : (-\epsilon, \epsilon) \rightarrow \mathbb{R}^D`$ is in the Pareto set in a neighborhood of $`t=0`$.
  - Taking $`\alpha \in\mathbb{R}^D, \beta \in\mathbb{R}^K`$ the KKT dual variables corresponding to $`\mathbf{x}^* = \mathbf{x}(0)`$, we have
    - $`H \mathbf{x}'(0) \in \text{Im}\left( {DF}^\top (\mathbf{x}^*) \right) \oplus \text{Im}\left( {DG}_{K'}^\top (\mathbf{x}^*) \right)`$
      - where
        - $`\displaystyle H = \sum_{i=1}^d \alpha_i H_{f_i}(\mathbf{x}^*) + \sum_{k=1}^{K'} \beta_k H_{g_k}(\mathbf{x}^*) \in\mathbb{R}^{D\times D}`$
          - for $`\begin{cases} H_{f_i}(\mathbf{x}^*) = \begin{bmatrix} H_{f_1}(\mathbf{x}^*) \cdots H_{f_d}(\mathbf{x}^*) \end{bmatrix} \in \mathbb{R}^{D\times D\times d} & \text{Hessian of } F(\mathbf{x}) \\ H_{g_k}(\mathbf{x}^*) = \begin{bmatrix} H_{g_1}(\mathbf{x}^*) \cdots H_{g_K}(\mathbf{x}^*) \end{bmatrix} \in \mathbb{R}^{D\times D\times K}  & \text{Hessian of } G(\mathbf{x}) \\  \end{cases}`$
        - $`\mathbf{x}'(0)`$ : the first order differentiation of $`\mathbf{x}(t)`$ on $`t=0`$
        - $`{DF} (\mathbf{x}^*) = \begin{bmatrix} \nabla f_1(\mathbf{x}^*)^\top \\ \vdots \\ \nabla f_d(\mathbf{x}^*)^\top \end{bmatrix} \in\mathbb{R}^{D\times d}`$ : the Jacobian of $`F(\mathbf{x})`$
        - $`{DG}_{K'} (\mathbf{x}^*) = \begin{bmatrix} \nabla g_1(\mathbf{x}^*)^\top \\ \vdots \\ \nabla g_{K}(\mathbf{x}^*)^\top \end{bmatrix} \in\mathbb{R}^{D\times K}`$ : the Jacobian of $`F(\mathbf{x})`$
        - $`\text{Im}(\cdot)`$ : the Span of the column space of $`\cdot`$
- Interpretation)
  - Recall the Newton's model
    - $`f(x + x') \approx f(x) + \nabla f(x)^\top x' + \frac{1}{2}{x'}^\top H_f(x) x'`$
  - Put $`x' = \mathbf{x}'(0)`$.
  - Then we have the second order term of  $`\frac{1}{2}{\mathbf{x}'(0)}^\top \underbrace{H_f(x) \mathbf{x}'(0)}_{\text{here!}}`$
- Properties)
  - $`\mathbf{x}'(0) \in \mathbb{R}^{d+K'-1}`$ where $`K'`$ is the number of active constraints.
    - Pf.)
      - $`\text{Im}\left( {DF}^\top (\mathbf{x}^*) \right) \oplus \text{Im}\left( {DG}_{K'}^\top (\mathbf{x}^*) \right) \in \mathbb{R}^{d+K'-1}`$
        - Why?)
            - $`\text{Im}\left( {DF}^\top (\mathbf{x}^*) \right) \in\mathbb{R}^{d}`$
              - Recall that $`{DF} (\mathbf{x}^*) \in\mathbb{R}^{D\times d}`$
              - Then, $`\text{Rank}(DF) = \min\{D, d\} = d \quad (\because d \ll D)`$
              - Hence, $`\text{Im}\left( {DF}^\top (\mathbf{x}^*) \right) = \text{Span}\left( \nabla f_1(\mathbf{x}^*), \cdots, \nabla f_d(\mathbf{x}^*) \right) \in\mathbb{R}^{d}`$
            - $`\text{Im}\left( {DG}_{K'}^\top (\mathbf{x}^*) \right) \in\mathbb{R}^{K'}`$
              - Likewise, $`{DG}_{K'} (\mathbf{x}^*) \in\mathbb{R}^{D\times K'}`$.
              - Here, some constraints may be inactive.
              - Consider only the active constraints we may put $`K' \le K`$ the number of active constraints.
                - For in active $`k, \; \beta_k = 0`$
              - Thus, $`\text{Rank}({DG}_{K'}) = \min\{D, K'\} = K' \quad (\because K' \ll D)`$
              - Hence, $`\text{Im}\left( {DG}_{K'}^\top (\mathbf{x}^*) \right) = \text{Span}\left( \nabla g_1(\mathbf{x}^*), \cdots, \nabla g_K(\mathbf{x}^*) \right) \in\mathbb{R}^{K'}`$
            - **Reduce one dimension** due to the [Stationary Constrain in KKT](#42-kkt-conditions).
                - cf.) $`\sum_{i=1}^d \alpha_i \nabla f_i(\mathbf{x}^*) + \sum_{k=1}^K \beta_k \nabla g_k(\mathbf{x}^*) = 0`$
    - Meaning)
      - Recall that the design space was $`\mathcal{X}\subseteq\mathbb{R}^D`$.
      - This approximation reduces the dimension of the direction that $`\mathbf{x}\in\mathcal{X}`$ must move to $`d+K'-1 \lt D`$ dimension.
        - where
          - $`d`$ : the number of objective functions
          - $`K'`$ : the number of active constraints
      - Projecting on the subspace of Hessian and Jacobian.
        - cf.) Dimensionality reduction and PCA

<br><br>

### 6 Pareto Front Discovery
- Settings)
  - Data Structure
    - Performance Buffer : $`B(j)`$
      - Desc.)
        - $`(d-1)`$-dimensional array discretized using (hyper)spherical coordinates
        - Partitions the performance space $`\mathcal{S}=F(\mathcal{X}) \subseteq\mathbb{R}^d`$
        - Each buffer cell $`B(j)`$ contains a list of candidate solutions.
          - Why multiple candidates?)
            - Useful for extracting sparse approximation of the Pareto front
            - top $`K(=50)`$   
              ![](./images/interactive%20011.png)
      - e.g.)
        - A stranded person in the middle of the sea, looking for the closest shore.
        - Partition the 360 degrees into $`j`$ directions and find the shore on the line.
        - Map is 2-dimensional $`(x,y)\text{-coordinate}`$
        - Direction is 1-dimensional $`\theta = \frac{360}{j}`$   
          ![](./images/interactive%20004.png)
- Algorithm)   
  ![](./images/interactive%20002.png)
  - Steps)
    1. Stochastic Sampling
    2. Local Optimization
    3. Deriving affine subspace with First-Order Approximation of the Pareto Front   
       ![](./images/interactive%20009.png)   
       ![](./images/interactive%20013.png)   
    4. Sparse Approximation : Choosing solutions using Graph-cut algorithm

#### 6.2.2. Stochastic Sampling
- Goal)
  - Choose $`N_S`$ samples in the design space $`\mathcal{X}`$
    - i.e.) $`\mathbf{x}_s^i \in\mathcal{X}`$ for $`i=1,\cdots,N_S`$
- How?)
  - First iteration
    - Uniformly sample from $`\mathcal{X}`$
  - Rest of the iteration
    - $`B(j)`$ from the previous state contains the point $`\mathbf{x}^j`$ with the minimal distance to the origin.
    - Sample $`\displaystyle\mathbf{x}_s = \mathbf{x}^j + \frac{1}{2^{\delta_p}}\mathbf{d}_p`$
      - where
        - $`\delta_p`$ : a uniform random number in $`[0, \delta_p]`$ (Scaling Factor)
        - $`\mathbf{d}_p`$ : a uniform random unit vector that defines the stochastic direction      
          ![](./images/interactive%20008.png)

#### 6.2.3. Local Optimization
- Goal)
  - For each $`\mathbf{x}_s^i`$, find $`\mathbf{x}_o^i`$ that optimizes for Pareto optimality
  - A scalarization scheme is used to convert the problem into single objective problem
    - **Diversification** is essential to avoid having solutions cluster in certain areas, failing to provide a good representation for the shape.
- Problem Setting)
  - $`\displaystyle\mathbf{x}_o = \arg\min_{\mathbf{x}\in\mathcal{X}} \Vert F(\mathbf{x}) - \mathbf{z(x_s)} \Vert^2`$
    - where $`\mathbf{z(x_s)} \in\mathbb{R}^d`$ is a reference point defined for each sample.
      - This paper uses $`\mathbf{z(x_s)} = \mathbf{x}_s + \mathbf{s(x_s)}C(\mathbf{x}_s)`$
        - where 
          - $`\mathbf{s(x_s)}`$ is a unit length search direction pushing $`\mathbf{x}_s`$ toward the Pareto front $`\mathcal{P}`$
            - Select the search direction assigned to a cell on the neighborhood of cell $`j`$ selected uniformly at random, within distance $`\delta_N`$
          - $`C(\mathbf{x}_s) = \delta_s \Vert \mathbf{x}_s \Vert`$ is a scaling factor depending on the distance to origin   
            ![](./images/interactive%20010.png)

### 6.3. First-Order Approximation of the Pareto Front
- Goal)
  - For each point $`\mathbf{x}_o^i`$ perform first-order approximation to find an affine subspace $`\mathcal{A}_i`$ around $`\mathbf{x}_o^i`$
  - $`\mathcal{A}_i`$ is stored in a matrix $`M^i`$ which is defined by the $`d-1`$ directions for local exploration.
    - e.g.) $`\mathcal{A}_i`$ in $`D=3`$ design space   
      ![](./images/interactive%20012.png)
- How?)
  - First-order approximation using the KKT condition.
  - The output is the affine subspace $`\mathcal{A}_i`$ represented by the point $`\mathbf{x}_o`$ and the bases represented by the matrix $`M_i`$

### 6.4 Sparse Approximation
- Goal)
  - Among $`K`$ solution candidates for each buffer cell $`B(j)`$, find a single solution $`\mathbf{x}_j^*`$.
  - Output)
    - $`\{(\mathbf{x}_1^*, \mathcal{A}_1^*), \cdots, (\mathbf{x}_{N_S}^*, \mathcal{A}_{N_S}^*)\}`$
      - Single solution and a corresponding affine subspace for each buffer cell
        - i.e.) The Pareto Front!
    - $`\displaystyle\bigcup_{j=1}^{N_S} \mathcal{A}_j \approx \text{Pareto Front}`$
- How?)
  - Use Graph-Cut Algorithm
    - Criteria)
      - Continuity : adjacent cells share similar affine space $`\mathcal{A}_i`$
      - Optimality : Minimal distance to Origin
    - Optimization using two constraints
      - $`E_B(j,k)`$ : binary term for continuity
        - How?) Lable $`\mathcal{A}_i`$ with $`l_i`$ and compare labels between solutions in adjacent buffer cells $`B(j)`$ and $`B(k)`$
      - $`E_U(j,i)`$ : unary term for optimality