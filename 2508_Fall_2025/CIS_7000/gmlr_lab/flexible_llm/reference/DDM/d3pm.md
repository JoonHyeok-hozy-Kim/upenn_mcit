# Structured Denoising Diffusion Models in Discrete State-Spaces
[@austinStructuredDenoisingDiffusion2023a]

## 2. Background: Diffusion Models
#### Model) Diffusion Models
- Forward Process
  - Desc.)
    - Corrupt the data $`x_0\sim q(x_0)`$ into a sequence of increasingly noisy latent variables $`x_{1:T} = x_1,\cdots,x_T`$.
    - Process)
      - $`q(x_{1:T}\mid x_0) = \displaystyle\prod_{t=1}^T q(x_t\mid x_{t-1})`$
- Reverse Markov Process
  - Desc.)
    - Gradually denoise the latent variables towards the data distribution.
    - Process)
      - $`p_\theta(x_{0:T}) = p(x_T)\displaystyle\prod_{t=1}^T p_\theta(x_{t-1}\mid x_t)`$
- Optimization)
  - Minimize a variational upper bound on the negative log-likelihood of   
    ![](./images/d3pm_001.png)

#### Props.) Diffusion Models
- If $`T\rightarrow\infty`$
  - then the forward and the reverse process share the same functional form.
  - Thus, we may use a learned **reverse process** from the same class of distributions as that of the **forward process**.
- For several choices of the forward process, 
  - $`q(x_t\mid x_0)`$ converges to a stationary distribution $`\pi(x)`$ when $`t\rightarrow\infty`$.
    - i.e.) Independent of $`x_0`$
  - Thus, for the large enough $`T`$, we may choose $`\pi(x)`$ as the prior $`p(x_T)`$.
    - e.g.) Gaussian
  - Also, from the above loss, $`L_T\approx0`$ regardless of the data distribution $`q(x_0)`$
- Desirable properties of the forward process, for the efficient training on $`p_\theta`$
  - Permits efficient sampling of $`x_t`$ from $`q(x_t\mid x_0),\;\forall t`$
    - Then, we may
      - randomly sample $`t`$
      - optimize each $`L_{t-1}`$ term individually with stochastic gradient descent
  - Has a tractable expression for the forward process posterior $`q(x_{t-1}\mid x,_t,x_0)`$
    - Then, we may easily compute the KL divergences in $`L_{t-1}`$ terms.
  - e.g.)
    - $`q(x_t\mid x_{t-1}) = \mathcal{N}(x_t\mid \sqrt{1-\beta_t} x_{t-1}, \beta_t\mathbf{I})`$
    - $`p_\theta(x_{t-1}\mid x_t) = \mathcal{N}(x_{t-1}\mid\mu_\theta(x_t,t), \Sigma_\theta(x_t, t))`$
      - Then
        - $`p(x_t) = \mathcal{N}(x_T\mid\mathbf{0,I})`$

<br><br>

## 3. Diffusion models for discrete state spaces
#### Concept) Diffusion with Categorical Random Variables
- Settings)
  - Scalar discrete random variable $`x_t`$ with $`K`$ categories
    - i.e.) $`x_t\in\{1,\cdots,K\},\; \forall t`$
  - Then, the **forward transition probabilities** can be represented by a matrix
    - $`[\boldsymbol{Q}_t]_{ij} = q(x_t = j\mid x_{t-1}=i)`$   
      - i.e.)    
        $`Q_t = \begin{bmatrix} 
        q(x_t=1\mid x_{t-1}=1) & q(x_t=2\mid x_{t-1}=1) & \cdots & q(x_t=K\mid x_{t-1}=1) \\ 
        q(x_t=1\mid x_{t-1}=2) & q(x_t=2\mid x_{t-1}=2) & \cdots & q(x_t=K\mid x_{t-1}=2) \\ 
        \vdots & \vdots & \ddots & \vdots \\
        q(x_t=1\mid x_{t-1}=K) & q(x_t=2\mid x_{t-1}=K) & \cdots & q(x_t=K\mid x_{t-1}=K) \\
        \end{bmatrix}`$
      - Desc.)
        - The $`i`$-th row denotes the probabilities of $`x_{t-1}=j`$ transitioning into each $`x_t`$ value.
      - Prop.)
        - $`\displaystyle\sum_{j=1}^K q(x_t=j\mid x_{t-1}=i) = 1,\;\forall i=1,\cdots,K`$
        - We may control the data corruption and denoising process by choosing different $`\boldsymbol{Q}_t`$.
- Forward Process)
  - $`q(\boldsymbol{x}_t\mid \boldsymbol{x}_{t-1}) = \text{Cat}(\boldsymbol{x}_t;\; \boldsymbol{p} = \boldsymbol{x}_{t-1}\boldsymbol{Q}_t)`$
    - where
      - $`\boldsymbol{x}_{t-1}, \boldsymbol{x}_t \in \left\{ [1\;0\;\cdots\;0], \cdots, [0\;0\;\cdots\;1] \right\}\subseteq \{0,1\}^K`$ : one-hot row vector
      - $`\boldsymbol{p}\in[0,1]^K`$ : row probability vector
        - cf.)   
          $`\boldsymbol{p} = \begin{bmatrix} q(x_t=1\mid x_{t-1}=i) & q(x_t=2\mid x_{t-1}=i) & \cdots & q(x_t=K\mid x_{t-1}=i) \end{bmatrix}`$
      - $`\text{Cat}(\boldsymbol{x};\boldsymbol{p})`$ : a categorical distribution over  $`\boldsymbol{x}`$ and $`\boldsymbol{p}`$ 
  - Then, we may get
    - $`q(\boldsymbol{x}_t\mid\boldsymbol{x}_0) = \text{Cat}(\boldsymbol{x}_t; \boldsymbol{p} = \boldsymbol{x}_0 \overline{\boldsymbol{Q}}_t)`$ : the $`t`$-step marginal
      - where $`\overline{\boldsymbol{Q}}_t = \boldsymbol{Q}_1\boldsymbol{Q}_2\cdots\boldsymbol{Q}_t`$ 
        - Prop.)
          - The rows of $`\overline{\boldsymbol{Q}}_t`$ must converge to a known stationary distribution when $`t`$ becomes large
    - $`\begin{aligned}
        q(\boldsymbol{x}_{t-1}\mid\boldsymbol{x}_t, \boldsymbol{x}_0) 
        &= \frac{q(\boldsymbol{x}_t\mid\boldsymbol{x}_{t-1},\boldsymbol{x}_0) q(\boldsymbol{x}_{t-1}\mid\boldsymbol{x}_0)}{q(\boldsymbol{x}_t\mid\boldsymbol{x}_0)} \\
        &= \text{Cat} \left(\boldsymbol{x}_t; \boldsymbol{p} = \frac{\boldsymbol{x}_t \boldsymbol{Q}_t^\top \odot \boldsymbol{x}_0 \overline{\boldsymbol{Q}}_t}{\boldsymbol{x}_0 \overline{\boldsymbol{Q}}_t x_t^\top}  \right)
      \end{aligned}`$ : the posterior at $`t-1`$
      - cf.)
        - $`q(\boldsymbol{x}_{t-1}\mid\boldsymbol{x}_t, \boldsymbol{x}_0) = q(\boldsymbol{x}_{t-1}\mid\boldsymbol{x}_t) \quad (\because\text{Markov Process})`$
- Reverse Process)
  - Assume that $`p_\theta(x_t\mid x_{t-1})`$ is also factorized as conditionally independent over the image or sequence elements.
    - Then, the KL divergence between $`q`$ and $`p_\theta`$ can be computed by simply summing over all possible values of each random variable.

<br>

### 3.1 Choice of Markov transition matrices for the forward process
- Advantage)
  - We may control the data corruption and denoising process by choosing different $`\boldsymbol{Q}_t`$.
- Caution)
  - $`\boldsymbol{Q}_t`$ should gradually add noise to the previous state.
    - Why?)
      - If not, the original image will turn into pure noise in few steps.
    - Below examples of $`\boldsymbol{Q}_t`$ are the ones that guarantee that the information is slowly and gradually destroyed in a controlled manner.
- Types)
  - [Uniform Transition Matrix](#concept-uniform-transition-matrix)
  - [Absorbing State Transition Matrix](#concept-absorbing-state-transition-matrix)
  - [Discretized Gaussian Transition Matrix](#concept-discretized-gaussian-transition-matrix)
  - [Token Embedding Distance Transition Matrix](#concept-token-embedding-distance-transition-matrix)

<br>

#### Concept) Uniform Transition Matrix
- Def.)
  - $`\boldsymbol{Q}_t = (1-\beta_t)\mathbf{I} + \beta_t/K \; \boldsymbol{1}\boldsymbol{1}^\top`$ with $`\beta_t\in[0,1]`$
    - Desc.)
      - With the probability
        - $`1-\beta_t`$, the next state stays in the same category
        - $`\beta_t`$, the next state changes the category
- Prop.)
  - Doubly stochastic with strictly positive entries.
    - Thus, the stationary distribution is uniform.
  - Transition probability to any other state is uniform
- Noise Schedule)
  - [Cosine schedule](../../../../project/references/basics/improved_ddpm.md#32-improving-the-noise-schedule) as in [Improved DDPM](../../../../project/references/basics/improved_ddpm.md).

<br>

#### Concept) Absorbing State Transition Matrix
- Def.)   
  - For the absorbing state $`m`$,   
    $`[\boldsymbol{Q}_t]_{ij} = \begin{cases}
      1 & \text{if } i=j=m \\
      1-\beta_t & \text{if } i=j \ne m \\
      \beta_t & \text{if } j=m, i\ne m \\
    \end{cases}`$
    - Or, equivalently $`Q_t = (1-\beta_t)\mathbf{I} + \beta_t \boldsymbol{1} e_m^\top`$ where $`e_m`$ is a vector with a one on the absorbing state and zeros elsewhere.
  - Prop.)
    - Higher $`\beta_t`$, faster absorbing
- Desc.)
  - [MASK] : an absorbing state
    - s.t. each token either stays the same or transitions to [MASK] with some probability $`\beta_t`$
  - Motivated by BERT
  - No particular relationships between categories
- Noise Schedule)
  - Interpolate the mutual information between $`\boldsymbol{x}_t`$ and $`\boldsymbol{x}_0`$
    - i.e.) $`I(\boldsymbol{x}_t;\boldsymbol{x}_0) \approx \left(1-\frac{t}{T}\right) H(\boldsymbol{x}_0)`$
    - This schedule reduces to exactly the $`(T-t+1)^{-1}`$

<br>

#### Concept) Discretized Gaussian Transition Matrix
- Desc.)
  - Imitate a continuous space diffusion model by using a discretized, truncated Gaussian distribution.
    - Higher probability to the closer categories
  - Doubly stochastic normalization, leading to a uniform stationary distribution.
- Noise Schedule)
  - Linearly increasing variance of the Gaussian before discretizing it.
    - cf.) Linear schedule for $`\boldsymbol{Q}_t`$ leads to a nonlinear amount of cumulative noise in $`\overline{\boldsymbol{Q}}_t`$

<br>

#### Concept) Token Embedding Distance Transition Matrix
- Desc.)
  - Specifically for the textural data.
  - Higher probability to the closer embeddings
- Noise Schedule)
  - Interpolate the mutual information between $`\boldsymbol{x}_t`$ and $`\boldsymbol{x}_0`$
    - i.e.) $`I(\boldsymbol{x}_t;\boldsymbol{x}_0) \approx \left(1-\frac{t}{T}\right) H(\boldsymbol{x}_0)`$

<br><br>

### 3.3 Parameterization of the Reverse Process
- Model)
  - $`p_\theta(\boldsymbol{x}_{t-1}\mid \boldsymbol{x}_t) \varpropto \displaystyle\prod_{\tilde{\boldsymbol{x}_0}} q(\boldsymbol{x}_{t-1}, \boldsymbol{x}_t \mid \tilde{\boldsymbol{x}_0}) \; \tilde{p}_\theta(\tilde{\boldsymbol{x}_0}\mid\boldsymbol{x}_t)`$


$`\begin{aligned}
    \log p_\theta(\mathbf{x}_{0})
    &= \log \int q(\mathbf{x}_{1:T}\mid \mathbf{x}_0) \frac{p_\theta(\mathbf{x}_{0:T})}{q(\mathbf{x}_{1:T}\mid \mathbf{x}_0)} \text{d}\mathbf{x}_{1:T} \\
    &\ge \mathbb{E}_{q(\mathbf{x}_{1:T}\mid \mathbf{x}_0)} \left[ \log\frac{p_\theta(\mathbf{x}_{0:T})}{q(\mathbf{x}_{1:T}\mid \mathbf{x}_0)} \right] & \because (\text{Jensen Inequality})
\end{aligned}`$