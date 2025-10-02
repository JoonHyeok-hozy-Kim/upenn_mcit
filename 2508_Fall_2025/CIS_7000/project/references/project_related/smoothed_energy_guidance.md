# Smoothed Energy Guidance: Guiding Diffusion Models with Reduced Energy Curvature of Attention
[@hongSmoothedEnergyGuidance2024]

## 2. Preliminaries
### 2.1 Diffusion Models
- Settings) Refer to [Score-Based Model](../../../paper_presentation/250931_latent_diffusion/paper_summary/score_based_model.md) for more detail
  - $`\mathbf{x}`$ : an image
  - $`t\in[0,1]`$ : the time
  - $`\text{d}\mathbf{x} = \mathbf{f}(\mathbf{x},t)\text{d}t + g(t)\text{d}\mathbf{w}`$ : the SDE that governs the forward process
    - where
      - $`\mathbf{f},g`$ : predefined functions that determine the manner in which the noise is added.
      - $`\text{d}\mathbf{w}`$ : a standard Wiener process
  - $`\text{d}\mathbf{x} = \left[ \mathbf{f}(\mathbf{x},t) - g(t)^2\nabla_\mathbf{x}\log p_t(\mathbf{x}) \right]\text{d}t + g(t)\text{d}\bar{\mathbf{w}}`$ : the reverse SDE
    - where
      - $`\nabla_\mathbf{x}\log p_t(\mathbf{x})`$ : the score of the noisy data distribution
      - $`\text{d}\bar{\mathbf{w}}`$ : the standard Wiener process for the reverse time
- Model)    
  - Let
    - $`s_\theta(\mathbf{x},t) \approx \nabla_\mathbf{x}\log p_t(\mathbf{x})`$ : a neural network to approximate the score function
      - cf.) Conditional Case
        - $`s_\theta(\mathbf{x},t,c) \approx \nabla_\mathbf{x}\log p_t(\mathbf{x}\mid c)`$
          - where $`c`$ denotes a condition
  - Additionally, consider the [CFG](../basics/classifier_free_guidance.md) that enhances the quality and the stability of the sample s.t.
    - $`\gamma_{\text{cfg}}`$ : the parameter that controls the strength of the guidance
  - Then, we may get the reverse process models as
    - $`\text{d}\mathbf{x} = \left[ \mathbf{f}(\mathbf{x},t) - g(t)^2 \left( \gamma_{\text{cfg}} \cdot \underbrace{s_\theta(\mathbf{x},t,c)}_{\text{conditional}} - (\gamma_{\text{cfg}}-1)\cdot \underbrace{s_\theta(\mathbf{x},t)}_{\text{unconditional}} \right) \right]\text{d}t + g(t)\text{d}\bar{\mathbf{w}}`$

<br>

### 2.2 Energy-based view of attention mechanism
#### Concept) Hopfield Network
- Def.)  
  - A Hopfield Network is a type of neural network that acts like an **associative memory**.
- Purpose)
  - Take an incomplete or noisy piece of information (an input pattern) 
  - Retrieve the most similar, complete version from its memory (a stable pattern).
- How)
  - Define the energy function $`E(\xi) = -\underbrace{\text{lse}(\mathbf{X}\xi^\top)}_{\text{similarity}} + \underbrace{\frac{1}{2}\xi\xi^\top}_{\text{regularizer}}`$
    - where
      - $`\xi\in\mathbb{R}^{1\times d}`$ : incomplete input pattern
      - $`\mathbf{X}\in\mathbb{R}^{N\times d}`$ : the prior data of the stable patterns
      - $`\text{lse}(\mathbf{v}) = \log\left(\sum_{i=1}^N e^{v_i}\right)`$ : the log-sum-exp function
    - Meaning)
      - Energy is a mathematical value that represents the magnitude of instability.
  - Optimization)
      - $`\displaystyle \xi^* = \arg\min_{\xi} E(\xi)`$
        - i.e.) 
          - The network's goal is to move from a high-energy state to a low-energy state.
        - How?) 
          - Gradient Descent
            - $`\xi_{t+1} = \xi_{t}-\eta\nabla_\xi E(\xi)`$
        - Prop.)
          - $`\xi^* \in\mathbf{X}`$ : the stable data point nearest to the $`\xi_0`$ input.

<br>

#### Concept) Energy Function for Self-Attention
- Idea)
  - Recall 
    - the [Hopfield Network](#concept-hopfield-network) above
    - $`\text{Attention}^{(i)}\left(Q^{(i)},K^{(i)},V^{(i)}\right) = \displaystyle \text{Softmax}\left(\frac{Q^{(i)}{K^{(i)}}^\top}{\sqrt{d}}\right)\cdot V^{(i)}`$
  - What if we treat
    - $`Q`$ to be the $`\xi`$
    - $`K`$ to be the $`\mathbf{X}`$
  - Then, we may approximate our network to find the key value that is nearest to the query.
- Def.)
  - Let
    - $`\mathbf{Q}\in\mathbb{R}^{(HW)\times d}`$ : a matrix of query vectors
    - $`\mathbf{K}\in\mathbb{R}^{(HW)\times d}`$ : a matrix of key vectors
    - $`\mathbf{A} = \mathbf{QK}^\top \in\mathbb{R}^{(HW)\times(HW)}`$ : the attention score matrix
  - Then, we may define the energy function w.r.t. entire self-attention weights in diffusion models as
    - $`E(\mathbf{A}) = \displaystyle\sum_{i=1}^H\sum_{j=1}^W E'(\mathbf{a}_{(i,j)})`$
      - where
        - $`\mathbf{a}_{(i,j)}\in\mathbb{R}^{1\times(HW)}`$ : the $`(i,j)`$-th flatten vector element of $`\mathbf{A}`$
        - $`E'(\mathbf{a}) = -\text{lse}(\mathbf{a}) = \displaystyle -\log\left(\sum_{k=1}^H\sum_{l=1}^W e^{a_{(k,l)}}\right)`$
        - $`a_{(k,l)}\in\mathbb{R}`$ : the $`(k,l)`$-th pixel element of $`\mathbf{a}`$
- Intuition)
  - The **attention** operation can be seen as a minimization steps on the energy landscape.
    - Why?)
      - The first derivative of the energy function has the form of the **softmax** operation.

<br><br>

## 3. Methods
### 3.1 Gaussian blur to attention weights
#### Concept) 2D Gaussian Filter
- Def.)
  - Let
    - $`G(x,y)=\displaystyle\frac{1}{2\pi\sigma^2}\exp\left(-\frac{(x-\mu_x)^2+(y-\mu_y)^2}{2\sigma^2}\right)`$ : the 2D Gaussian function
      - $`(\mu_x,\mu_y) := (0,0)`$
      - $`\sigma`$ : the standard deviation
        - Prop.)
          - Bigger $`\sigma`$, more blurry outcome.
  - Then, the 2D Gaussian Filtering can be defined as
    - $`\displaystyle\tilde{a}_{(i,j)} = \sum_{m=-k}^k\sum_{n=-k}^k G(m,n)\cdot a_{(i+m, j+n)}`$
      - where 
        - $`k`$ : the filter size
        - $`G(m,n)`$ : the Gaussian Filter value at position $`(m,n)`$

<br>

#### Lemma 3.1)
  - Spatially applying a [2D Gaussian blur](#concept-2d-gaussian-filter) to the attention weights $`\mathbf{a}:=\mathbf{Qk}^\top`$ **preserves the average** $`\mathbb{E}_{i,j}[a_{(i,j)}]`$

<br>

#### Lemma 3.2)
- Applying a [2D Gaussian blur](#concept-2d-gaussian-filter) to attention weights $`\mathbf{a}:=\mathbf{Qk}^\top`$ increases the $`\text{lse}`$ term
- The maximum $`\text{lse}`$ is achieved when the attention is Uniform.
  - i.e.) $`a_{(i,j)} = a_{(k,l)},\;\forall i,j,k,l \quad\Leftrightarrow\quad \sigma\rightarrow\infty`$
  - cf.)
    - Recall that [PAG](./perturbed_attention_guidance.md) used [PSA](./perturbed_attention_guidance.md#concept-perturbed-self-attention-psa) as $`QK^\top = I`$.
    - In SEG, $`\sigma\rightarrow\infty`$ corresponds to 
      - $`QK^\top \rightarrow \begin{bmatrix} c&c&\cdots&c\\c&c&\cdots&c\\\vdots&\vdots&\ddots&\vdots\\c&c&\cdots&c\\ \end{bmatrix}`$ where $`c`$ is the average value.
    - i.e.) SEG considers the context while PAG doesn't!

<br>

### 3.2 Analysis of the energy landscape
#### Theorem 3.1)
- Thm.)
  - Let the attention weights be defined as $`\mathbf{a} = \mathbf{Qk}^\top`$.
  - Applying a [Gaussian blur](#concept-2d-gaussian-filter) to the attention weights $`\mathbf{a}`$ before the softmax operation results in the **attenuation (smoothing out)** of the underlying energy function's Gaussian curvature where gradient descent is performed.
    - Analogy)
      - Just like Lipschitz-continuity and convexity improve the performance of the Gradient Descent
      - Gaussian blur smooths out the underlying energy function so that GD can better perform.

<br>

### 3.3 Smoothed energy guidance for diffusion models

#### Prop. 3.1.)
- Thm.)
  - $`G*(\mathbf{QK}^\top) = (G*\mathbf{Q})\mathbf{K}^\top`$
    - i.e.) Blurring the attention weights with $`G`$ is equivalent to blurring the query matrix $`\mathbf{Q}`$ with $`G`$ and then computing the attention weights
- Why needed?)
  - More efficient computation
    - Let $`N=HW`$ be the token size.
    - The LHS applies Gaussian blur to $`\mathbf{QK}^\top\in\mathbb{R}^{N^2}\Rightarrow O(N^2)`$ operation.
    - The RHS applies Gaussian blur to $`\mathbf{Q}\in\mathbb{R}^{N\times d}\Rightarrow O(N\times d)`$ operation.
    - Since $`N\gg d`$, the RHS can apply less Gaussian blur operations.
    - Thus, the RHS is faster.

<br>

#### Concept) Smoothed Energy Guidance (SEG)
- Def.)
  - Let
    - $`s_\theta(\mathbf{x},t)`$ : unconditional score prediction
    - $`\tilde{s}_\theta(\mathbf{x},t)`$ : unconditional score prediction with [reduced energy curvature](#theorem-31)
      - i.e.) [Gaussian blurred](#concept-2d-gaussian-filter)
    - $`(\mathbf{QK}^\top)_{\text{seg}} = G * (\mathbf{QK}^\top)`$ : 2D Gaussian Blurring
      - where
        - $`G`$ : a 2D Gaussian filter with std.dev. $`\sigma`$
        - $`*`$ : 2D convolutional operator
  - Then, we define SEG as
    - $`\text{d}\mathbf{x} = \left[ \mathbf{f}(\mathbf{x},t) - g(t)^2 \left( \gamma_{\text{seg}} \cdot s_\theta(\mathbf{x},t,c) - (\gamma_{\text{seg}}-1)\cdot \underbrace{\tilde{s}_\theta(\mathbf{x},t)}_{\text{Gaussian Blurred}} \right) \right]\text{d}t + g(t)\text{d}\bar{\mathbf{w}}`$
- Prop.)
  - It can be applied to unconditional problems as well.
    - cf.) [CFG](../basics/classifier_free_guidance.md) could only be applied to the conditional problem settings.


<br>
