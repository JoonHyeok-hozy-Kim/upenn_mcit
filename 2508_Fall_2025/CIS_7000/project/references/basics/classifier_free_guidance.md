# Classifier-Free Diffusion Guidance
Ho et al. 2022

## 1. Introduction
### Concept) Classifier Guidance
Dhariwal & Nichol 2021
- Desc.)
  - A technique to boost the sample quality of a diffusion model using an extra trained classifier.
  - Enabled generating low temperature samples from a diffusion model
    - Concept) Low Temperature
      - Desc.)
        - A technique that makes a probability distribution sharper
          - i.e.) $`T\lt 1,\quad \displaystyle p_i = \frac{\exp\left(\displaystyle\frac{z_i}{T}\right)}{\sum_j \exp\left(\displaystyle\frac{z_j}{T}\right)}`$
          - Here, $`T`$ denotes the temperature
      - Prop.)
        - Sharper distribution leads to more deterministic sampling.
        - Thus, the output tends to be more stable and of higher quality, but the diversity decreases.
    - cf.) Similar to BigGAN and low temperature Glow
    - How?)
      - Consider a labeled data set $`\mathcal{D} = (x,y)`$
      - Mix a diffusion model's score estimate with the input gradient of the log probability of a classifier.
        - i.e.) $`\hat{\epsilon_\theta}(x_t, y) = \epsilon_\theta(x_t, t) - s\cdot\sigma_t \underbrace{\nabla_{x_t} \log p(y\mid x_t)}_{\text{the gradient term!}}`$ : the reverse diffusion.
          - where
            - $`\epsilon_\theta`$ : the noise that the DM estimates
            - $`\nabla_{x_t} \log p(y\mid x_t)`$ : the classifier gradient 
              - i.e.) the direction that the probability of $`p(y\mid x)`$ increases.
            - $`s`$ : the guidance strength
      - By varying the strength $`(s)`$ of the classifier gradient, they could trade off Inception score and FID score.
        - Bigger $`s`$ $`\Rightarrow`$ More samples like that classifier $`y`$
          - Then
            - Inception Score increases : more recognizable
            - FID decreases : more realistic
            - Less diversity
- Drawback)
  - Complicates the diffusion model training pipeline.
    - Why?)
      - Requires training an extra classifier in DM
      - Classifier must be trained on noisy data so it is generally not possible to plug in a pre-trained classifier.
  - Mixing $`\nabla_{x_t} \log p(y\mid x_t)`$ can be interpreted as a gradient-based adversarial attack!
    - cf.) Just like in GAN models where the generator's adversarial attack improves the quality of the generation.

<br><br>

## 2. Background
### Concept) Continuous Time Diffusion Model
- Model)
  - $`\mathbf{x}\sim p(\mathbf{x})`$
  - $`\mathbf{z} = \{ \mathbf{z}_\lambda\mid\lambda\in[\lambda_{\min}, \lambda_{\max}] \},\quad \lambda_{\min}\lt \lambda_{\max}\in\mathbb{R}`$
  - $`q(\mathbf{z}\mid\mathbf{x})`$ : the forward process
    - where
      - $`q(\mathbf{z}_\lambda\mid\mathbf{x}) = \mathcal{N}(\alpha_\lambda\mathbf{x}, {\sigma_\lambda}^2 \mathbf{I})`$
        - where
          - $`{\alpha_\lambda}^2 = \displaystyle\frac{1}{1+e^{-\lambda}}`$
          - $`{\sigma_\lambda}^2 = 1-{\alpha_\lambda}^2`$
        - Prop.)
          - $`\lambda = \displaystyle\frac{\log {\alpha_\lambda}^2}{{\sigma_\lambda}^2}`$
      - $`q(\mathbf{z}_\lambda\mid\mathbf{z}_{\lambda'}) = \mathcal{N}\left(\frac{\alpha_\lambda}{\alpha_{\lambda'}}\mathbf{x}, {\sigma_{\lambda\mid\lambda'}}^2 \mathbf{I}\right)`$
        - where
          - $`\lambda\lt\lambda'`$
          - $`{\sigma_{\lambda\mid\lambda'}}^2 = \left( 1-e^{\lambda-\lambda'} \right){\sigma_\lambda}^2`$
  - $`p(\mathbf{z})`$ : the marginal of $`\mathbf{z}`$
    - where
      - $`\mathbf{x}\sim p(\mathbf{x})`$
      - $`\mathbf{z}\sim q(\mathbf{z}\mid\mathbf{x})`$