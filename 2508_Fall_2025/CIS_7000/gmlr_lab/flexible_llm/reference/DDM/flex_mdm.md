# Any-Order Flexible Length Masked Diffusion
[@kimAnyOrderFlexibleLength2025]

## 2 Preliminaries : CTMC & Mased Diffusion
### Concept) Transport with CTMC
- Def.)
  - Given
    - $`p_1`$ : the target distribution over a finite vocabulary set
    - $`p_0`$ : the reference distribution
  - we define a continuum of distributions $`\{p_t\}_{t\in[0,1]}`$ s.t. $`p_{t=1} = p_1`$.
  - This type of transport can be realized by a CTMC s.t.
    - $`\{X_t\}_{t\in[0,1]}`$ : a stochastic process with $`X_0\sim p_0`$ governed by a time-dependent transition rate matrix $`\{R_t(\cdot,\cdot)\}_{t\in[0,1]}`$ s.t.
      - $`R_t(x,x) = -\displaystyle\sum_{y\ne x}R_t(x,y)`$
      - $`R_t(x,y) \ge 0,\quad x\ne y`$
- Training)
  - Two goals)
    - Specify the probability path $`\{p_t\}_{t\in[0,1]}`$
    - Learn the associated $`\hat{R}_t`$ that satisfies the Komologorov forward equation:
      - $`\partial_t p_t(x) = \displaystyle\sum_{y} p_t(y)R_t(y,x)`$

#### Concept) Infinitesimal Likelihood of Transition
- Def.)
  - $`\mathbb{P}(X_{t+h} = y \mid X_t = x) = \mathbf{1}_{x=y} + h R_t(x,y) + o(h)`$
    - where
      - $`\displaystyle\lim_{h\rightarrow0} \frac{o(h)}{h} = 0`$
- Meaning)
  - Infinitesimal Likelihood of that $`X_t`$ transition to any other state $`y`$

<br>

### 2.1 Masked Diffusion Models
- Settings)
  - $`p_1`$ : the target distribution that assigns probability to length $`L`$ sequences.
  - $`p_0`$ : the base distribution, which is the point mass distribution at the fully masked $`L`$-length sequence $`(\mathbf{m},\cdots,\mathbf{m})`$
    - where $`\mathbf{m}`$ is an auxiliary mask token
- Model)
  - $`\{p_t\}_{t\in[0,1]}`$ : the probability path that bridges between $`p_0`$ and $`p_1`$ is determined by the **stochastic interpolant** $`\{x_t\}_{t\in[0,1]}`$
    - i.e.) $`x_t\sim p_t`$
  - $`x_t`$ should satisfy...
    - the dependence on the smooth and unmasking schedule of $`\alpha_t:[0,1]\rightarrow[0,1]`$ with boundary condition $`(\alpha_0, \alpha_1) = (0,1)`$, and the time derivative denoted by $`\dot{\alpha}_t`$
    - Given 
      - $`x^i`$ : the $`i`$-th element of $`x`$
      - $`T^i`$ : the unmasking time of $`x^i`$ drawn from $`\dot{\alpha}_t`$
        - where $`\dot{\alpha}_t`$ is the time derivative of $`\alpha_t`$
        - i.e.)
          - $`\mathbb{P}(T^i\le t) \le \displaystyle\int_0^t \dot{\alpha}_s\text{d}s = \alpha_t \quad (\because \alpha_0 = 0)`$
    - we may denote $`x_t^i = \begin{cases} \mathbf{m}& t\lt T^i\\ x_1^i & t\ge T^i \end{cases}`$

<br><br>

## 3 Variable Length Masked Diffusions: Training
#### Concept) Joint Interpolant
- Goal)
  - An extension of the stochastic interpolant that augments the process with an auxiliary variable explicitly **tracking token positions**
- Def.)
  - Let 
    - $`\alpha:[0,1]\rightarrow[0,1]`$ : an insertion schedule
      - with boundary condition $`(\alpha_0, \alpha_1)`$
    - $`\beta:[0,1]\rightarrow[0,1]`$ : an unmasking schedule
      - with boundary condition $`(\beta_0, \beta_1)`$
    - 