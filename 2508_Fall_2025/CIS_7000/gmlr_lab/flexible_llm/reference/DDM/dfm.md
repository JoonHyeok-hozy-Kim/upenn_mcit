# Discrete Flow Matching
[@gatDiscreteFlowMatching2024]

## 2 Discrete Flow Matching
### 2.1 Setup and notations
- $`x = \left( x^1, x^2, \cdots, x^N \right)`$ : a sequence as an array of $`N`$ elements
  - where
    - each element (token) $`x^i`$ is selected from a vocabulary of size $`d`$
      - i.e.) $`x^i \in [d] = \{1,\cdots,d\}`$
- $`\mathcal{D} = [d]^N`$ : the entire set of possible sequences
  - where 
    - $`[d] = \{1,\cdots,d\}`$
- $`X`$ : a random variable taking values $`\mathcal{D}`$
  - where
    - $`P(X=x)`$ is the corresponding probability mass function 
      - Notation) $`p(x)`$ denotes the PMF as well.
- $`p(x^i) = \displaystyle\sum_{x^{\bar{i}}} p(x)`$ : the $`x^i`$ marginal of $`p`$
  - where
    - $`x^{\bar{i}} = (\cdots, x^{i-1}, x^{i+1}, \cdots) \in [d]^{N-1}`$
      - i.e.) $`x^i`$ is excluded from $`x`$
  - Desc.)
    - By marginalizing out $`x^{\bar{i}}`$, we focus only on a single token $`x^i`$
- $`\delta_y(x) = \displaystyle\prod_{i=1}^N \delta_{y^i}(x^i)`$ : the delta PMF function
  - where
    - $`x,y\in\mathcal{D}`$
    - $`\delta_{y^i}(x^i) = \begin{cases} 1 & x^i = y^i \\ 0 & x^i \ne   y^i \end{cases}`$
  - Prop.)
    - $`\delta_y(x) = 1 \Leftrightarrow x^i = y^i,\quad \forall i=1,\cdots,N`$
  - Notation)
    - $`\delta_y(x^{i}) = \delta_{y^{i}}(x^{i})`$
    - $`\delta_y(x^{\bar{i}}) = \delta_{y^{\bar{i}}}(x^{\bar{i}}) = \displaystyle\prod_{j\ne i}\delta_{y^{j}}(x^{j}) = \displaystyle\prod_{j\in\{1,\cdots,N\}\backslash i}\delta_{y^{j}}(x^{j})`$

<br>

### 2.2 Source and target distributions
- Settings)
  - $`X_0\sim p`$ : a sample from the source distribution $`p`$
  - $`X_1\sim q`$ : a sample from the target(data) distribution $`q`$
  - $`(X_0, X_1) \sim \pi(x,y)`$ : the training data sampled from a joint distribution $`\pi`$
    - where
      - $`p(x) = \displaystyle\sum_{y\in\mathcal{D}} \pi(x,y)`$ : the marginals
      - $`q(y) = \displaystyle\sum_{x\in\mathcal{D}} \pi(x,y)`$ : the marginals
      - $`\pi`$ can have two types of couplings
        - [Unconditional Coupling (U-coupling)](#concept-unconditional-coupling-u-coupling)
        - [Conditional Coupling (C-coupling)](#concept-conditional-coupling-c-coupling)
  - $`\mathbf{m}\in[d]`$ : a mask token
    - We may further assume the source distribution $`p`$ to be all-mask sequences.
      - i.e.) $`p(x) = \delta_{\mathbf{m}}(x)`$

#### Concept) Unconditional Coupling (U-coupling)
- Def.)
  - $`\pi(x_0, x_1) = p(x_0)q(x_1)`$
    - i.e.) the training pairs are sampled independently from the source and target distributions.
      - i.e.) $`(X_0, X_1)\sim p(X_0)q(X_1)`$
  - Using the mask token $`\mathbf{m}`$, we have
    - $`(X_0, X_1) = \big( (\mathbf{m}, \cdots, \mathbf{m}), X_1 \big)`$


#### Concept) Conditional Coupling (C-coupling)
- Def.)
  - $`\pi(x_0, x_1) = p(x_0\mid x_1)q(x_1)`$
  - Using the mask token $`\mathbf{m}`$, we may denote
    - $`(X_0, X_1) = \left( \overbrace{\underbrace{\mathbb{I}\odot X_1}_{\text{unmasked}} + \underbrace{(\mathbf{1} - \mathbb{I}) \odot (\mathbf{m}, \cdots, \mathbf{m})}_{\text{masked!}}}^{\text{partially masked}},\; X_1 \right)`$
      - where
        - $`\mathbb{I}\in\{0,1\}^N`$ : a random variable indicating the conditioning
        - $`\odot`$ : the entrywise product
        - $`\mathbf{1} = \{1\}^N`$ : a vector with all ones

<br><br>

### 2.3 Probability paths
- Def.)
  - $`p_t`$ : the probability path in $`t\in[0,1]`$
    - where
      - $`p_0 = p`$
      - $`p_1 = q`$
  - Or, we may re-right as
    - $`p_t(x) = \displaystyle\sum_{x_0, x_1\in\mathcal{D}} p_t(x\mid x_0, x_1) \pi(x_0, x_1)`$
      - where
        - $`p_t(x\mid x_0, x_1) = \displaystyle\prod_{i=1}^N p_t(x^i\mid x_0, x_1)`$
          - for 
            - $`p_t(x^i\mid x_0, x_1)`$ : the time dependent probability on the space of tokens $`[d]`$ conditioned on $`(x_0, x_1)`$
            - $`p_0(x^i\mid x_0, x_1) = \delta_{x_0}(x^i)`$
            - $`p_1(x^i\mid x_0, x_1) = \delta_{x_1}(x^i)`$

<br>

#### Concept) Flow Matching Framework
- Def.)
  - $`p_t(x^i\mid x_0, x_1) = \displaystyle\sum_{j=1}^m \kappa_t^{i,j} w^j(x^i\mid x_0, x_1)`$
    - where
      - $`w^j(x^i\mid x_0, x_1)`$ : the $`j`$-th base conditional probability
        - e.g.)
          - $`w^1 = \delta_{x_1}`$ : the data distribution
          - $`w^2 = p_{\text{u}}`$ : the uniform distribution
          - $`w^3 = \delta_{x_0}`$ : the source distribution
          - and so on...
      - $`\kappa_t^{i,j}`$ : the weight on $`w^j(x^i\mid x_0, x_1)`$
        - for $`\displaystyle\sum_j\kappa_t^{i,j} = 1, \;\kappa_t^{i,j}\ge 0`$
      - $`m`$ : the number of possible base path
- e.g.)
  - Convex Interpolant
    - Model)   
      $`\begin{aligned}
        p_t(x^i\mid x_0, x_1) 
        &= \kappa_t^{i,3} \delta_{x_0}(x^i) + \kappa_t^{i,1} \delta_{x_1}(x^i) \\
        &= (1-\kappa_t) \underbrace{\delta_{x_0}(x^i)}_{\text{source}} + \kappa_t \underbrace{\delta_{x_1}(x^i)}_{\text{data}} \\
      \end{aligned}`$
      - where
        - $`\begin{cases} \kappa_0=0\\ \kappa_1=1 \end{cases}`$ and $`\kappa_t`$ monotonically increases with $`t`$
    - Desc.)
      - All tokens $`x^i`$ share the same weight $`\kappa_t`$ and $`(1-\kappa_t)`$ at time $`t`$
  - Convex Interpolant + Uniform Noise added
    - Model)   
      - $`p_t(x^i\mid x_0, x_1) = \kappa_t^1 \underbrace{\delta_{x_1}(x^i)}_{\text{data}} + \kappa_t^2 \underbrace{p_{\text{u}}(x^i)}_{\text{uniform}} + \kappa_t^3 \underbrace{\delta_{x_0}(x^i)}_{\text{source}}`$
        - where
          - $`\begin{cases} \kappa_0^1=0\\ \kappa_1^1=1 \end{cases}`$ : data
          - $`\kappa_0^2 = \kappa_1^2=0`$ : no uniform added at $`t=0\vee t=1`$
          - $`\begin{cases} \kappa_0^3=1\\ \kappa_1^3=0 \end{cases}`$ : source

<br><br>

### 2.4 Generating Probability Velocities
#### Concept) Continuous Flow Matching
- Model)
  - Let
    - $`X_t\in\mathbb{R}^N`$ : the current sample at $`t`$
    - $`t\in[0,1)`$
    - $`u_t^i : \mathbb{R}^N \rightarrow \mathbb{R}`$ : a learned generating velocity field for $`i\in[N]`$
      - e.g.)
        - a neural network
        - parameterized via denoiser
        - noise-prediction (e.g. $`\epsilon`$-prediction from DDPM)
      - cf.) $`u_t`$ is a velocity (vector) field!
  - Then, Euler sampling follows the deterministic rule of
    - $`X_{t+h}^i = X_t^i + h\cdot u_t^i(X_t)`$
      - where
        - $`h\gt0`$ : a user-defined time step
- Prop.)
  - If the sampling provides $`X_{t+h}\sim p_{t+h} + o(h)`$, then $`u_t`$ generates $`p_t`$.
    - where 
      - $`o(h^\ell)`$ means a function going to zero faster than $`h^\ell`$ as $`h\rightarrow0`$
    - Desc.)
      - $`u_t`$ is the deterministic vector field that we set.
      - $`p_t`$ is the actual path we want to learn.
      - If the output of the ODE is similar to the actual path, we may say that $`u_t`$ generates $`p_t`$

<br>

#### Concept) Probability Velocity
- Goal)
  - We want to define Flow Matching in the discrete setting.
  - Thus, we need a velocity concept that corresponds to the velocity field $`u_t`$ in the [continuous flow matching](#concept-continuous-flow-matching).
- Def.)
  - 

<br>

#### Concept) Discrete Flow Matching
