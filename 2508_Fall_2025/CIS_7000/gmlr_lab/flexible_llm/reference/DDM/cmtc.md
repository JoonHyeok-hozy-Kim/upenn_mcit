# A Continuous Time Framework for Discrete Denoising Models
[@campbellContinuousTimeFramework2022]

## 2 Background on Discrete Denoising Models
- Settings)
  - $`x_0\in\mathcal{X}`$ : a discrete data
    - where 
      - $`S=\vert\mathcal{X}\vert`$
        - i.e.) the state space has the finite cardinality $`S`$
  - $`\exists p_{\text{data}}`$ : a discrete data distribution 
    - s.t. $`x_0\sim p_{\text{data}}(x_0)`$
- Forward Noising Process
  - Desc.)
    - Transforms $`p_{\text{data}}`$ to some distribution $`q_K(x_K)`$
      - where $`q_K(x_K) \approx p_{\text{ref}}(x_K)`$ for an easy to sample distribution $`p_{\text{ref}}(x_K)`$.
  - $`q_{k+1\mid k}(x_{k+1}\mid x_k)`$ : a forward kernel
    - e.g.) Uniform
      - $`q_{k+1\mid k}(x_{k+1}\mid x_k) = \delta_{x_{k+1},x_k}(1-\beta) + (1-\delta_{x_{k+1},x_k})\beta/(S-1)`$
        - where
          - $`\delta_{i,j}=\begin{cases} 1&\text{if }i=j\\0&\text{if }i\ne j\\ \end{cases}`$ : a Kronecker delta
          - $`(1-\beta)`$ : the probability that the state is unchanged i.e. $`P(x_{k+1}=x_k)`$
        - Desc.)
          - The corresponding $`p_{\text{ref}}`$ is Uniform over all states with $`\frac{\beta}{S-1}`$
  - Forward Joint Decomposition   
    $`q_{0:K}(x_{0:K}) = p_{\text{data}}(x_0)\displaystyle\prod_{k=0}^{K-1}q_{k+1\mid k}(x_{k+1}\mid x_k)`$
- Reverse Denoising Process
  - Forward Posterior   
    $`q_{0:K}(x_{0:K}) = q_K(x_K)\displaystyle\prod_{k=0}^{K-1}\underbrace{q_{k\mid k+1}(x_k\mid x_{k+1})}_{\text{Reverse order!}}`$
    - where 
      - $`q_{k\mid k+1}(x_k\mid x_{k+1}) = \displaystyle\frac{q_{k+1\mid k}(x_{k+1}\mid x_k) q_k(x_k)}{q_{k+1}(x_{k+1})}\quad\cdots(A)`$
    - Prop.)
      - $`q_{k\mid k+1}(x_k\mid x_{k+1})`$ is intractable!
        - Thus, we may parameterize with $`p^\theta_{k\mid k+1}`$ in the reverse Process
      - $`q_K(x_K)`$ is also intractable
        - Why?)
          - $`q_K(x_K) = \sum_{x_0} q(x_K\mid x_0) p_{\text{data}}(x_0)`$ where $`p_{\text{data}}(x_0)`$ is intractable!
        - Sol.)
          - For large $`K`$, $`q_K(x_K)\approx p_{\text{ref}}(x_K)`$
  - Parametric Denoising Model   
    $`\begin{aligned}
        p^\theta_{k\mid k+1} &\triangleq \sum_{x_0} q_{k\mid k+1, 0}(x_k\mid x_{k+1}, x_0) \; p^\theta_{0\mid k+1}(x_0\mid x_{k+1}) & \text{cf.) Each }x_0\text{ is mutually exclusive. Thus, }\sum_{x_0} \\
        &= \sum_{x_0} \underbrace{\frac{q_{k+1\mid k}(x_{k+1}\mid x_k) q_{k\mid 0}(x_k\mid x_0)}{q_{k+1\mid 0}(x_{k+1}\mid x_0)}}_{\text{from (A)}\times\text{Bayes Rule}} \; p^\theta_{0\mid k+1}(x_0\mid x_{k+1}) \\
        &= q_{k+1\mid k}(x_{k+1}\mid x_k) \sum_{x_0} \frac{q_{k\mid 0}(x_k\mid x_0)}{q_{k+1\mid 0}(x_{k+1}\mid x_0)} \; p^\theta_{0\mid k+1}(x_0\mid x_{k+1})
    \end{aligned}`$
  - Optimization
    - Minimize the negative discrete time ELBO   
      $`\mathbb{E}_{p_{\text{data}}(x_0)} \left[ -\log p_0^\theta(x_0) \right] \le \mathbb{E}_{q_{0:K}(x_{0:K})} \left[ -\log \displaystyle\frac{p_{0:K}^\theta(x_{0:K})}{q_{1:K \mid0}(x_{1:K}\mid x_0)}\right] = \mathcal{L}_{\text{DT}}(\theta)`$
      - Decomposition)   
        $`\begin{aligned}
            \mathcal{L}_{\text{DT}}(\theta) &= \mathbb{E}_{p_{\text{data}}(x_0)} \left[ \text{KL}(q_{K\mid0}(x_K\mid x_0)\;\Vert\; p_{\text{ref}}(x_K))\right] \\
            &- \mathbb{E}_{q_{1\mid0}(x_1\mid x_0)}\left[ \log p_{0\mid1}^\theta(x_0\mid x_1) \right] \\
            &+ \sum_{k=1}^{K-1}\mathbb{E}_{q_{k+1\mid 0}(x_{k+1}\mid x_0)} \left[ \text{KL}(q_{k\mid k+1,0}(x_k\mid x_{k+1},x_0)\;\Vert\; p_{k\mid k+1}^\theta(x_k\mid x_{k+1})) \right]
        \end{aligned}`$
- Sampling Generative Distribution   
  $`p_{0:K}^\theta(x_{0:K}) = p_{\text{ref}}(x_K) \displaystyle\prod_{k=0}^{K-1} p_{k\mid k+1}^\theta(x_k\mid x_{k+1})`$

<br><br>

## 3 Continuous Time Framework
### 3.1 Forward Process and its Time Reversal
![](./images/ctmc_001.png)

#### Concept) Continuous Time Markov Chain (CTMC)
- A Continuous Time Markov Chain (CTMC) can be completely characterized by 
  - Jump Time 
    - i.e.) transitioning between states
    - Let $`r(\tilde{x}\mid x)`$ be the probability distribution of jumping to $`\tilde{x}`$ from $`x`$
      - cf.) **Determined** that we **jump**, what is the probability of jumping specifically to $`\tilde{x}`$
  - Holding Time 
    - i.e.) staying at the same state
    - Put $`\nu(x)`$ as the mean holding time of the state $`x`$
- The time between each **jump** or **holding time** is exponentially distributed with mean $`\nu(x)`$ where $`x`$ is the state in which the process is holding.
  - i.e.) $`t\sim \text{Exponential}\left(t;\frac{1}{\nu(x)}\right) = \frac{1}{\nu(x)}\exp\left(-\frac{t}{\nu(x)}\right) \quad(\because t\ge 0)`$
  - Why exponential distribution?
    - We assumed the Markov Chain framework that depends only on the previous state.
    - The exponential distribution satisfies this property of memorylessness
      - How?)
        - Consider $`P(T\gt t+s\mid T\gt t) = \displaystyle\frac{P(T\gt t+s \;\wedge\; T\gt t)}{P(T\gt t)}`$.
        - We may interpret this as the probability of $`T\gt t+s`$ given the memory of $`T\gt t`$.
        - For exponential distribution with rate $`\lambda`$, the pdf is given by $`p(t)=\lambda e^{-\lambda t}`$
          - cf.) mean of $`1/\lambda`$
        - Thus, we may calculate
          - $`P(T\gt t)=\displaystyle\int_t^\infty \lambda e^{-\lambda T}\text{d}T = e^{-\lambda t}`$
          - $`P(T\gt t+s)=\displaystyle\int_{t+s}^\infty \lambda e^{-\lambda T}\text{d}T = e^{-\lambda (t+s)}`$
        - Also, $`P(T\gt t+s \;\wedge\; T\gt t) = P(T\gt t+s)`$
          - This may hold in other distributions as well.
        - Hence,
          - $`P(T\gt t+s\mid T\gt t) = \displaystyle\frac{P(T\gt t+s)}{P(T\gt t)} = \frac{e^{-\lambda (t+s)}}{e^{-\lambda t}} = e^{-\lambda s} = P(T\gt s)\quad\cdots\text{QED}`$

#### Concept) Transition Rate Matrix
- Settings)
  - $`t\in[0,T]`$
  - $`q_0`$ : the initial distribution
- Def.)
  - $`R_t\in\mathbb{R}^{S\times S}`$ : a transition rate matrix
    - where $`R_t(\tilde{x},x) = \displaystyle\lim_{\Delta t\rightarrow0}\frac{q_{t\mid t-\Delta t}(x\mid\tilde{x}) - \delta_{x,\tilde{x}}}{\Delta t} \quad\cdots(B)`$
      - for
        - $`q_{t\mid t-\Delta t}(x\mid\tilde{x})`$ : an infinitesimal transition probability from the state $`\tilde{x}`$ at time $`t-\Delta t`$ transitioning to $`x`$ at time $`t`$
        - $`\delta_{i,j}=\begin{cases} 1&\text{if }i=j\\0&\text{if }i\ne j\\ \end{cases}`$ : a Kronecker delta
- Desc.)
  - $`R_t(\tilde{x},x)`$ is the instantaneous **rate** for the state $`\tilde{x}`$ at time $`t-\Delta t`$ transitioning to $`x`$ at time $`t`$
    - Case 1) $`\tilde{x}\ne x`$
      - These elements $`R_t(\tilde{x},x)`$ denote the rate at which probability mass moves from $`\tilde{x}`$ to $`x`$
      - Consider that   
        $`\begin{aligned}
          \tilde{x}\ne x
          &\Rightarrow\delta_{\tilde{x},x} = 0 \\
          &\Rightarrow R_t(\tilde{x},x) = \displaystyle\lim_{\Delta t\rightarrow0}\frac{q_{t\mid t-\Delta t}(x\mid\tilde{x})}{\Delta t}
        \end{aligned}`$
      - Above is the rate that $`q_{t\mid t-\Delta t}(x\mid\tilde{x})`$ changes
    - Case 2) $`\tilde{x} = x`$ (i.e. the diagonal elements of $`R_t`$)
      - These elements $`R_t(x,x)`$ denote the **exit rate** by our definition.
      - Consider that   
        $`\begin{aligned}
          \tilde{x}=x
          &\Rightarrow\delta_{x,x} =1 \\
          &\Rightarrow R_t(x,x) = \displaystyle\lim_{\Delta t\rightarrow0}\frac{q_{t\mid t-\Delta t}(x\mid\tilde{x}) - 1}{\Delta t}
        \end{aligned}`$
      - Also, if $`\tilde{x}=x`$, then $`\Delta t\rightarrow0 \Rightarrow q_{t\mid t-\Delta t}(x\mid\tilde{x})\rightarrow 1`$
        - i.e.) the probability of staying at $`x`$ is almost 1 in the infinitesimal time step.
      - $`1-q_{t\mid t-\Delta t}(x\mid\tilde{x})`$ denotes the probability of NOT staying at $`x`$
        - i.e.) the exit probability.
      - By **negating** the above probability, we may define the exit rate as a negative value.
      - Thus, we may interpret $`R_t(x,x) = \displaystyle\lim_{\Delta t\rightarrow0}\frac{q_{t\mid t-\Delta t}(x\mid\tilde{x}) - 1}{\Delta t}`$ as the rate that the probability mass moving out of the state $`x`$. 
- Prop.)
  - The higher $`R_t(\tilde{x}, x)`$ is, the less time it will take for this transition to occur
  - $`R_t`$ is analogous to $`q_{k+1\mid k}`$ in the [discrete model case above](#2-background-on-discrete-denoising-models).
    - Refer to the forward process below for more detail.
  - $`R(\tilde{x},x) \ge 0,\quad\forall\tilde{x}\ne x`$
  - $`R(x,x) \le 0`$ 
    - Desc.) Negative value by definition!
  - $`R(x,x) = -\displaystyle\sum_{x'\ne x}R(x,x')`$
    - Desc.) Moving out rate of $`x`$ is identical to the negative of the sum of transitioning rate from $`x`$ to all other states $`x'`$s.
  - [Jump / Holding time](#concept-continuous-time-markov-chain-ctmc) definition using $`R`$
    - Time-Homogeneous Case
      - $`\nu(x) = -\displaystyle\frac{1}{R(x,x)}`$ : mean holding time
        - Why?)
          - Recall that we assumed $`t\sim \text{Exponential}\left(t;\frac{1}{\nu(x)}\right)`$
          - Here, $`\frac{1}{\nu(x)}`$ is the rate that "exit" happens in a unit time step $`t`$.
          - This is equivalent to the exit rate $`-R(x,x)`$
            - cf.) We negated the rate and made $`R(x,x)\le 0`$.
          - Therefore, $`\frac{1}{\nu(x)}=-R(x,x)`$
      - $`r(\tilde{x}\mid x) = \displaystyle(1-\delta_{\tilde{x},x})\frac{R(x,\tilde{x})}{-R(x,x)}`$ : jumping probability from $`x`$ to $`\tilde{x}`$
        - Why?)
          - $`(1-\delta_{\tilde{x},x})`$ is used to fix the case $`x=\tilde{x}`$
            - i.e.) We only consider the case we jump!
            - Thus, we consider all possible states we can jump from $`x`$, and get the conditional probability of jumping specifically to $`\tilde{x}`$
          - Recall that $`R(x,x) = -\displaystyle\sum_{x'\ne x}R(x,x')`$
            - i.e.) Negative value of total exit rate from $`x`$
          - $`R(x,\tilde{x})`$ is the rate of jumping from $`x`$ to $`\tilde{x}`$ specifically.
    - Time-Inhomogeneous Case
      - Desc.)
        - $`\nu(x)`$ and $`r(\tilde{x}\mid x)`$ does not depend on $`t`$.
        - Thus, simple relations like the homogeneous case don't hold.
        - Still, the properties of $`R(x,\tilde{x})`$ still hold!
  - Kolmogorov Equations ($`0\le s\lt t \le 1`$) : Discrete version of Fokker-Planck in Continuous
    - Forward ($`s\rightarrow t`$) : $`\underbrace{\partial_t}_{\text{at }t} q_{t\mid s}(x\mid\tilde{x}) = \displaystyle\sum_y q_{t\mid s}(y\mid\tilde{x})R_t(y,x)`$
      - Desc.)
        - On the perspective of the destination $`x`$ at the time step $`t`$
        - Sum of all possible **inflows** to $`x`$ by passing through $`\tilde{x}\rightarrow y\rightarrow x`$
    - Backward ($`s\leftarrow t`$) : $`\underbrace{\partial_s}_{\text{at }s} q_{t\mid s}(x\mid\tilde{x}) = -\displaystyle\sum_y R_s(\tilde{x},y) q_{t\mid s}(x\mid y)`$
      - Desc.)
        - On the perspective of the starting point $`\tilde{x}`$ at the time step $`s`$
        - Sum of all possible **outflows** from $`\tilde{x}`$ by passing through $`\tilde{x}\rightarrow y\rightarrow x`$
    - Differential Equation at $`t`$ : $`\partial_t q_{t}(x) = \displaystyle\sum_y q_{t}(y)R_t(y,x)`$
      - Desc.)
        - Considers both inflows and outflows of $`x`$ at time step $`t`$.
          - Inflows : $`x\ne y`$
          - Outflows : $`x = y`$


<br>

#### Concept) Forward Process
- Def.) Forward Transitional Probability (Infinitesimal Transition Probability)
  - $`q_{t\mid t-\Delta t} (x\mid\tilde{x}) = \delta_{x,\tilde{x}} + R_t(\tilde{x},x)\Delta t + o(\Delta t)`$
    - where
      - $`o(\Delta t)`$ : terms that tend to zero faster than $`\Delta t`$
        - e.g.) $`\Delta t^2, \Delta t^3`$
    - Derivation)
      - Directly from the definition of the [trainsition rate matrix](#concept-transition-rate-matrix) $`R_t(\tilde{x},x)`$ above
- Prop.)
  - $`R_t`$ plays an analogous role to $`q_{k+1\mid k}`$ (the discrete time forward kernel) 
    - i.e.) It defines the forward process
    - Thus, we may design $`R_t`$ s.t.
      - the forward process mixes quickly towards an easy to sample (stationary) distribution $`p_{\text{ref}}`$
        - e.g.) Uniform
      - capable of analytically obtaining $`q_{t\mid0}(x_t\mid x_0)`$


#### Concept) Reverse Process
- Forward Posterior
  - $`q_{t\mid t+\Delta t}(\tilde{x}\mid x) = \delta_{\tilde{x},x} + \hat{R}_t(x,\tilde{x})\Delta t + o(\Delta t)`$
    - where   
      $`\hat{R}_t(x,\tilde{x}) = \begin{cases}
        \displaystyle R_t(\tilde{x},x) \sum_{x_0} \frac{q_{t\mid0}(\tilde{x}\mid x_0)}{q_{t\mid0}(x\mid x_0)} q_{0\mid t}(x_0\mid x) & \text{if } x\ne\tilde{x} \\
        -\displaystyle\sum_{x'\ne x} \hat{R}_t(x,x') & \text{otherwise}
      \end{cases}`$
        - for
          - $`q_{t\mid0}(x\mid x_0)`$ : the conditional marginals of the forward process
          - $`q_{0\mid t}(x_0\mid x) = \displaystyle\frac{q_{t\mid0}(x\mid x_0)}{q_t(x)}`$
          - $`q_t(x)`$ : the marginal of the forward process at time $`t`$
        - Derivation)
          - Refer to [Proposition 1](#proposition-1)
    - Prop.)
      - $`\hat{R}_t`$ is intractable.
        - Why?)
          - $`q_t(x)`$ is intractable $`\Rightarrow`$ $`q_{0\mid t}(x_0\mid x)`$ is intractable
        - Sol.)
          - Approximate using $`\hat{R}_t^\theta`$ with $`p_{0\mid t}^\theta (x_0\mid x)`$
- Parametric Denoising Model   
  - Def.)
    - $`p_{0\mid t}^\theta (x_0\mid x)`$
  - Prop.)
    - We may approximate the backward transition rate as   
      $`\hat{R}_t^\theta(x,\tilde{x}) = \begin{cases}
        \displaystyle R_t(\tilde{x},x) \sum_{x_0} \frac{q_{t\mid0}(\tilde{x}\mid x_0)}{q_{t\mid0}(x\mid x_0)} \; \underbrace{p_{0\mid t}^\theta (x_0\mid x)}_{\text{Param'd Model}} & \text{if } x\ne\tilde{x} \\
        -\displaystyle\sum_{x'\ne x} \hat{R}_t^\theta(x,x') & \text{otherwise}
      \end{cases}`$

<br>

#### Proposition 1
- Theorem)
  - Let
    - $`\{ x_t \}_{t\in[0,T]}`$ : a forward in time CTMC
      - with
        - $`R_t`$ : a rate matrix at $`t`$
        - $`p_{\text{data}}(x_0)`$ : the initial distribution
        - $`q_T(x_T)`$ : the terminal distribution
  - Then, there exists a CTMC
    - with
      - initial distribution $`q_T(x_T)`$
      - terminal distribution $`p_{\text{data}}(x_0)`$
      - $`\hat{R}_t`$ : a backward transition rate matrix
  - And, it is almost everywhere equivalent to the time **reversal of the forward CTMC** : $`\{ x_t \}_{t\in[T,0]}`$
  - Further, $`\hat{R}_t`$ is given by   
    $`\hat{R}_t(x,\tilde{x}) = \begin{cases}
      \displaystyle R_t(\tilde{x},x) \sum_{x_0} \frac{q_{t\mid0}(\tilde{x}\mid x_0)}{q_{t\mid0}(x\mid x_0)} q_{0\mid t}(x_0\mid x) & \text{if } x\ne\tilde{x} \\
      -\displaystyle\sum_{x'\ne x} \hat{R}_t(x,x') & \text{otherwise}
    \end{cases}`$
      - where
        - $`q_{t\mid0}(x\mid x_0)`$ : the conditional marginals of the forward process
        - $`q_{0\mid t}(x_0\mid x) = \displaystyle\frac{q_{t\mid0}(x\mid x_0)}{q_t(x)}`$
        - $`q_t(x)`$ : the marginal of the forward process at time $`t`$

<details>
<summary>Pf (Click to see)</summary>

1. Existence of the reversal of the forward CTMC
   - Since $`\{ x_t \}_{t\in[0,T]}`$ is a Markov Process, it satisfies...
     - Right-continuity   
       ![](./images/ctmc_001.png)
     - Markov properties
       - $`\{q_{t\mid s}\mid s,t\in[0,T], s\lt t\}`$ : a transition probability system
       - $`R_t(\tilde{x},x)`$ : a transition matrix
       - Kolmogorov **Backward** Equation
         - $`\underbrace{\partial_s}_{\text{at }s} q_{t\mid s}(x\mid \tilde{x}) = -\displaystyle\sum_{y\in\mathcal{X}} R_s(\tilde{x},y) q_{t\mid s}(x\mid y) \quad\cdots(\text{A-1})`$
   - Put 
     - $`\tau_n`$ : the jump times of the forward process $`\{ x_t \}_{t\in[0,T]}`$
     - $`\{y_t\}_{t\in[0,T]}`$ s.t. $`y_t = \begin{cases}
        x_{T-t} & \text{if } t\ne T-\tau_n \\
        x_{\tau_n}^- = \displaystyle\lim_{t\le\tau_n, t\rightarrow\tau_n} x_t & \text{if } t= T-\tau_n \\
       \end{cases}`$
       - i.e.) Artificially making $`\{y_t\}`$ satisfy right-continuity by...
         - Filling holes (state before jump in $`x_t`$)
         - Punching holes (state after jump in $`x_t`$)   
           ![](./images/ctmc_001_1.png)
   - Then $`\{y_t\}_{t\in[0,T]}`$ is a CTMC.
2. Derivation of $\hat{R}_t(x,\tilde{x})$
   - Let $`x,\tilde{x}\in\mathcal{X}`$ s.t. $`x\ne\tilde{x}`$ be different states.
   - Since $`\{y_t\}_{t\in[0,T]}`$ is a CTMC, we may define 
     - $`\hat{R}_t(\tilde{x},x)`$ : a transition matrix 
     - $`\{p_{t\mid s}\mid s,t\in[0,T], s\lt t\}`$ : a transition probability system
   - Thus, $`\{y_t\}`$'s Kolmogorov **Forward** equations are given by
      - $`\underbrace{\partial_t}_{\text{at }t} p_{t\mid s}(\tilde{x}\mid x) = \displaystyle\sum_{y\in\mathcal{X}} p_{t\mid s}(y\mid x)\hat{R}_t(y,\tilde{x}) \quad\cdots(\text{A-2})`$
   - Then, we might rewrite the transition probability system as   
     $`\begin{array}{lll}
      p_{t\mid s}(\tilde{x}=j\mid x=i)
      &= \mathbb{P}(y_t = j\mid y_s = i) \\
      &= \mathbb{P}(x_{T-t} = j\mid x_{T-s} = i) \\
      &= \mathbb{P}(x_{T-s} = i\mid x_{T-t} = j) \displaystyle\frac{\mathbb{P}(x_{T-t} = j)}{\mathbb{P}(x_{T-s} = i)} &\because\text{Bayes' Rule} \\
      &= q_{T-s\mid T-t}(x=i\mid \tilde{x}=j) \displaystyle\frac{q_{T-t}(\tilde{x} = j)}{q_{T-s} (x= i)} & \cdots(\text{A-3}) \\
     \end{array}`$
   - Consider that if $`s\lt t`$, then $`T-s \gt T-t`$
     - Thus, we may use (relabel) $`\{x_t\}`$'s Kolmogorov Backward $`\text{(A-1)}`$ as 
        - $`\partial_{T-t} q_{T-s\mid T-t}(x\mid \tilde{x}) = -\displaystyle\sum_{y\in\mathcal{X}} R_{T-t}(\tilde{x},y) q_{T-s\mid T-t}(x\mid y)`$
          - i.e.) Relabel $`\begin{cases} s \rightarrow T-t \\ t\rightarrow T-s \end{cases}`$
          - Why?) The equation holds for $`\forall s,t`$ s.t. $`s\lt t`$
      - Using the chain rule, we may get   
        $`\begin{aligned}
          \underbrace{\partial_t}_{\text{what we want!}} q_{T-s\mid T-t}(x\mid \tilde{x})
          &= \left(\frac{\partial_t}{\partial_{T-t}}\right) \left(\partial_{T-t} q_{T-s\mid T-t}(x\mid \tilde{x})\right) \\
          &= (-1) \left(-\sum_{y\in\mathcal{X}} R_{T-t}(\tilde{x},y) q_{T-s\mid T-t}(x\mid y)\right) \\
          &= \sum_{y\in\mathcal{X}} R_{T-t}(\tilde{x},y) q_{T-s\mid T-t}(x\mid y) &\cdots\text{(A-4)}
        \end{aligned}`$
   - Also consider that 
     - $`\begin{cases} \displaystyle\lim_{s\rightarrow t}q_{T-s\mid T-t}(x\mid \tilde{x}) = 0 \\ \displaystyle\lim_{s\rightarrow t}q_{T-s\mid T-t}(x\mid x) = 1 \end{cases}\quad\cdots\text{(A-5)}`$
       - Why?)
         - $`q_{T-s\mid T-t}(x\mid \tilde{x})`$ is the probability that $`x`$ jumps to $`\tilde{x}`$.
           - Thus, as $`s\rightarrow t (\Leftrightarrow\Delta t\rightarrow0)`$, the probability converges to 0.
         - $`q_{T-s\mid T-t}(x\mid x)`$ is the probability that $`x`$ stays.
           - Thus, as $`s\rightarrow t (\Leftrightarrow\Delta t\rightarrow0)`$, the probability converges to 1.
       - cf.) Rate $`R_t`$ may not!
   - Since $`x\ne\tilde{x}`$, by definition   
     $`\begin{array}{lll}
       \hat{R}_t(x,\tilde{x}) &= \displaystyle\lim_{s\rightarrow t}\frac{p_{t\mid s}(\tilde{x}\mid x)}{t-s} & \left(\Leftrightarrow \displaystyle\lim_{\Delta t\rightarrow0}\frac{q_{t\mid t-\Delta t}(\tilde{x}\mid x)}{\Delta t}\right) \\
       &= \displaystyle\lim_{s\rightarrow t} \partial_t p_{t\mid s}(\tilde{x}\mid x) \\
       &= \displaystyle\lim_{s\rightarrow t} \partial_t \left(q_{T-s\mid T-t}(x\mid \tilde{x}) \frac{q_{T-t}(\tilde{x})}{q_{T-s} (x)}\right) & \because \text{Plugging in (A-3)} \\
       &= \displaystyle\lim_{s\rightarrow t} \left[ \partial_t q_{T-s\mid T-t}(x\mid \tilde{x}) \frac{q_{T-t}(\tilde{x})}{q_{T-s} (x)} + \underbrace{q_{T-s\mid T-t}(x\mid \tilde{x})}_{\rightarrow0} \frac{\partial_t q_{T-t}(\tilde{x})}{q_{T-s} (x)}\right] \\
       &= \displaystyle\lim_{s\rightarrow t} \partial_t q_{T-s\mid T-t}(x\mid \tilde{x}) \frac{q_{T-t}(\tilde{x})}{q_{T-s} (x)} & \because\text{(A-5)} \\
       &= \displaystyle\lim_{s\rightarrow t} \underbrace{\sum_{y\in\mathcal{X}} R_{T-t}(\tilde{x},y) q_{T-s\mid T-t}(x\mid y)}_{\text{(A-4)}} \frac{q_{T-t}(\tilde{x})}{q_{T-s} (x)} &\because\text{Plugging in (A-4)} \\
       &= \displaystyle\lim_{s\rightarrow t} \left(\sum_{y \ne x} R_{T-t}(\tilde{x},y) \underbrace{q_{T-s\mid T-t}(x\mid y)}_{\rightarrow 0} + R_{T-t}(\tilde{x},x) \underbrace{q_{T-s\mid T-t}(x\mid x)}_{\rightarrow1} \right) \frac{q_{T-t}(\tilde{x})}{q_{T-s} (x)}  \\
       &= \displaystyle R_{T-t}(\tilde{x},x) \frac{q_{T-t}(\tilde{x})}{q_{T-t} (x)}
     \end{array}`$
     - Or, equivalently 
       - $`\hat{R}_{T-t}(x,\tilde{x}) = \displaystyle R_{t}(\tilde{x},x) \frac{q_{t}(\tilde{x})}{q_{t} (x)}`$

</details>