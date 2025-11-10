# Generative Flows on Discrete State-Spaces:  Enabling Multimodal Flows with Applications to Protein Co-Design
[@campbellGenerativeFlowsDiscrete2024]


## 2 Background
- Settings)
  - $`x\in\{1,\cdots,S\}^D`$ : a discrete data sequence with $`D`$-dimension
    - For simplicity, we assume $`D=1`$.
    - Still, all results hold for $`\forall D\gt1`$.

<br>

### 2.1 Continuous Time Markov Chain (CTMC)
[hozy note on CTMC](./cmtc.md)
- Settings)
  - $`x_t`$ : a sequence trajectory over time $`t\in[0,1]`$
    - Desc.)
      - $`x_t`$ alternates between resting in its current state and periodically jumping to another randomly chosen state.   
        ![](./images/dfm_001.png)
  - $`R_t(x_t,j)\in\mathbb{R}^{S\times S}`$ : the transition rate matrix from state $`x_t`$ to $`j`$
    - where
      - $`x_t,j\in\{1,\cdots,S\}`$
      - its off-diagonal elements are non-negative.
  - $`p_{t+\text{d}t}(j\mid x_t)`$ : the probability of jumping from $`x_t`$ to $`j`$ determined by $`R_t(x_t,j)`$
    - Desc.)
      - $`R_t`$ determines the frequency and destination of the jumps by     
        $`\begin{aligned}
            p_{t+\text{d}t}(j\mid x_t) &= \begin{cases} R_t(x_t, j)\text{d}t & \text{for } j\ne x_t \\ 1+R_t(x_t, x_t)\text{d}t & \text{for } j = x_t \\ \end{cases} \\
            &= \delta\{x_t,j\} + R_t(x_t, j)\text{d}t
        \end{aligned}`$
        - where
          - $`\delta(i,j) = \begin{cases} 1 & \text{if }i=j\\0 & \text{otherwise}\end{cases}`$
          - $`R_t(x_t, x_t) := -\displaystyle\sum_{k\ne x}R_t(x_t,k)`$
            - Why?) So that $`R_t(x_t, x_t)+\displaystyle\sum_{k\ne x}R_t(x_t,k)=\sum_{k}R_t(x_t,k)=p_{t+\text{d}t}(\cdot\mid i)=1`$
      - $`p_{t+\text{d}t}`$ is a Categorical distribution 
        - i.e.)
          - $`j\sim p_{t+\text{d}t}(j\mid x_t) \Leftrightarrow j\sim \text{Cat}\bigg(\delta\{x_t,j\} + R_t(x_t,j)\text{d}t\bigg)`$
      - Finite time interval notation
        - $`x_{t+\Delta t} \sim \text{Cat}\bigg(\delta\{x_t,x_{t+\Delta t}\} + R_t(x_t,x_{t+\Delta t})\Delta t\bigg)`$

<br>

### 2.2 Kolmogorov Equation
- Goal)
  - Describe $`p_t(x_t)`$, the marginal distribution at time $`t`$, using the dynamics of the [CTMC above](#21-continuous-time-markov-chain-ctmc).
- Def.)
  - $`\partial_t p_t(x_t) = \displaystyle \underbrace{\sum_{j\ne x_t} R_t(j,x_t)p_t(j)}_{\text{incoming to } x_t} - \underbrace{\sum_{j\ne x_t} R_t(x_t, j)p_t(x_t)}_{\text{outgoing from }x_t} = R_t^\top p_t`$

<br>

#### Concept) Ordinary Differential Equation (ODE) defined on CTMC
- Def.)
  - $`\partial_t p_t = R_t^\top p_t, \quad \forall t\in[0,1]`$
- Prop.)
  - This series of distributions $`p_t`$ satisfies the ODE as a probability flow.

<br><br>

## 3 Discrete Flow Models (DFM)
### 3.1 A Flow Model for Sampling Discrete Data

#### Concept) Generative Flow
- Desc.)
  - A probability flow $`p_t`$ that interpolates from noise to data.
    - $`p_0(x_0) = p_{\text{noise}}(x_0)`$
    - $`p_1(x_1) = p_{\text{data}}(x_1)`$
  - Use conditional flow $`p_{t\mid1}(x_t \mid x_1)`$ instead.
    - Meaning)
      - Assuming that the final destination is $`x_1`$ at $`t=1`$, the probability that the state is at $`x_t`$ at time $`t`$.
    - Why needed?) 
      - $`p_t`$ is complex to consider directly.
    - The conditional flow at noise and data state can be defined as...
      - Noise
        - $`p_{t\mid1}(x_t\mid x_1) = p_{\text{noise}}(x_t)`$
      - Data
        - $`p_{t\mid1}(x_t\mid x_1) = \delta\{x_1,x_t\} = \begin{cases} 1 &\text{if }t=1\\ 0&\text{otherwise}\end{cases}`$
    - **Interpolation** can be defined as...
      - Uniform prior case
        - $`p_{t\mid1}^{\text{unif}}(x_t\mid x_1) = \text{Cat}\bigg( t\;\delta\{x_1,x_t\} + (1-t)\frac{1}{S} \bigg)`$
          - where
            - $`p_{\text{noise}}^{\text{unif}}(x_t)=\displaystyle\frac{1}{S}`$
      - Mask state $`(M)`$ case
        - $`p_{t\mid1}^{\text{mask}}(x_t\mid x_1) = \text{Cat}\bigg( t\;\delta\{x_1,x_t\} + (1-t)\;\delta\{M,x_t\} \bigg)`$
          - where
            - $`p_{\text{noise}}^{\text{mask}}(x_t)=\delta\{M,x_t\}`$
- Def.)
  - $`p_t(x_t) =\mathbb{E}_{p_{\text{data}}(x_1)} \left[ \underbrace{p_{t\mid1}(x_t\mid x_1)}_{\text{tractable!}} \right]`$
    - How?)   
      - Consider that   
        $`\begin{aligned}
            \mathbb{E}_{p_{\text{data}}(x_1)} \left[ p_{t\mid1}(x_t\mid x_1) \right] 
            &= \sum_{x_1} p_{t\mid1}(x_t\mid x_1) \; p_{\text{data}}(x_1)  \\
            &= \sum_{x_1} p(x_t, x_1) \\
            &= p_t(x_t)
        \end{aligned}`$

<br>

#### Concept) Conditional Transition Matrix
- Desc.)
  - [Recall](#generative-flows-on-discrete-state-spaces--enabling-multimodal-flows-with-applications-to-protein-co-design) that we decided to use the conditional probability flow $`p_{t\mid1}(x_t \mid x_1)`$.
  - Thus, we should define $`R_t(x_t, j\mid x_1)`$ that generates $`p_{t\mid1}(x_t \mid x_1)`$.
- Def.)         
  - $`R_t(x_t, j\mid x_1)`$ 
    - s.t.   
        $`\begin{aligned}
            p_{t}(x_t \mid x_1) &= \begin{cases} R_t(x_t, j\mid x_1)\text{d}t & \text{for } j\ne x_t \\ 1+R_t(x_t, x_t\mid x_1)\text{d}t & \text{for } j = x_t \\ \end{cases}
        \end{aligned}`$

<br>

#### Concept) How to get the Marginal Transition Rate Matrix
- Goal)
  - To generate the marginal probability flow, $`p_t(x_t)`$, we need to define $`R_t(x_t,j)`$.
  - However, since $`p_t(x_t)`$ is tractable, we instead have...
    - $`p_{t}(x_t \mid x_1)`$
    - $`R_t(x_t, j\mid x_1)`$ 
  - We want to derive using what we have.
- Desc.)
  - The Prop. below shows that we may derive $`R_t(x_t,j)`$ using $`R_t(x_t, j\mid x_1)`$ 
    - $`R_t(x_t, j) := \mathbb{E}_{p_{1\mid t}(x_1\mid x_t)} \bigg[ R_t(x_t,j\mid x_1) \bigg]`$
      - pf.)   
        ![](./images/dfm_002.png)
  - To take the expectation, we may get
    - $`p_{1\mid t}(x_1\mid x_t) = \displaystyle \frac{p_{t\mid1}(x_t\mid x_1) \; p_{\text{data}}(x_1)}{p_t(x_t)}`$
      - where $`p_t(x_t)`$ is still intractable.
  - Thus, we [approximate using neural network](#312-training).
    - i.e.) $`p_{1\mid t}^\theta(x_1\mid x_t) = p_{1\mid t}(x_1\mid x_t)`$

<br>

#### 3.1.1 Sampling
- Algorithm)   
  ![](./images/dfm_003.png)

<br>

#### 3.1.2 Training
- Desc.)
  - Recall that we approximated as
    - $`p_{1\mid t}^\theta(x_1\mid x_t) = p_{1\mid t}(x_1\mid x_t)`$
  - We may train this model by making it approximate the true denoising distribution.
    - i.e.) Learn to predict the clean data point $`x_1`$ given the noisy data $`x_t \sim p_{t\mid 1} (x_t\mid x_1)`$.
  - Using the cross entropy loss, we may get
    - $`\mathcal{L}_{\text{ce}} = \mathbb{E}_{p_{\text{data}}(x_1)\;\mathcal{U}(t;0,1)\;p_{t\mid1}(x_t\mid x_1)} \bigg[ \log p_{1\mid t}^\theta(x_1\mid x_t) \bigg]`$
      - cf.)   
        $`\begin{aligned}
            \mathcal{L}_{\text{ce}} &= -\sum_{x_1'} y(x_1') \log p^\theta(x_1'\mid x_t) & (y(x_1') \text{ is a one-hot vector}) \\
            &= -\sum_{x_1'} \log p^\theta(x_1'\mid x_t) & (\because \text{all } x_1\ne x_1' \text{zeroes out!}) \\
        \end{aligned}`$
      - This loss relates to ELBO.