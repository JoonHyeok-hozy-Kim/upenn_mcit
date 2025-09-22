# CIS 7000 Advanced Generative Models

## My Presentation
- Styled-GAN (250908)
  - [GAN Paper Summary](./paper_presentation/250908_style_gan/paper_note_gan.md)
  - [Style-GAN Paper Summary](./paper_presentation/250908_style_gan/paper_note_style_gan.md)
- (Stolen) Auto-Encoder Variational Bayes
  - [VAE Paper Summary](./paper_presentation/250917_vae/paper_note.md)
- High-Resolution Image Synthesis with Latent Diffusion Models (250929)
  - [DDPM](./paper_presentation/250924_latent_diffusion/paper_summary/ddpm.md)
  - [Score-Based Model](./paper_presentation/250924_latent_diffusion/paper_summary/score_based_model.md)
  - [Latent Diffusion](./paper_presentation/250924_latent_diffusion/paper_summary/latent_diffusion.md)

<br>

| Concept| [DDPM](./paper_presentation/250924_latent_diffusion/paper_summary/ddpm.md) (Ho 2020)| [SDE](./paper_presentation/250924_latent_diffusion/paper_summary/score_based_model.md) (Song 2021)| [CFG](./project/references/basics/classifier_free_guidance.md) (Ho 2022)|
|--------------------|------------------------------|------------------------------------------------|----------------------------------------------------|
| time variable| discrete step $`t`$| continuous time $`t \in [0,1]`$| log-SNR $`\lambda`$|
| noisy sample| $`x_t`$| $`x_t`$| $`z_\lambda`$|
| cumulative factor| $`\bar{\alpha}_t`$| $`\exp\!\left(-\int \beta(s)\, ds\right)`$| $`\alpha_\lambda^2 = \frac{1}{1+e^{-\lambda}}`$     |
| variance| $`1-\bar{\alpha}_t`$| depends on SDE type| $`\sigma_\lambda^2 = 1-\alpha_\lambda^2`$|
| model target| $`\epsilon_\theta(x_t, t)`$| score $`s_\theta(x_t, t)`$| $`\epsilon_\theta(z_\lambda, \lambda)`$|


<br>

## Project Game Plan
- Reference
  - Basics
    - [Improved DDPM](./project/references/basics/improved_ddpm.md)
      - Interpolated reverse process variance
      - Non-Linear noise schedule
      - Non-Uniform step distribution during training
    - [Classifier-Free Guidance](./project/references/basics/classifier_free_guidance.md) 
      - Train conditional $`D_1`$ and unconditional $`D_0`$ simultaneously
    - [Autoguidance](./project/references/basics/autoguidance.md)
      - Train original $`D_1`$ and penalized $`D_0`$ simultaneously under CFG setting.
    - [(WIP) Rectified Flow](./project/references/basics/rectified_flow.md)
      - Implementation : [minRF](https://github.com/cloneofsimo/minRF/tree/main)
    - Conditional Flow Matching
    - Stable Diffusion 3
  - Project Related
    - Self-Rectifying Diffusion Sampling with Perturbed-Attention Guidance
      - Implementation
    - Smoothed Energy Guidance: Guiding Diffusion Models with Reduced Energy Curvature of Attention
    - Fine-Grained Perturbation Guidance via Attention Head Selection
    - [Yan](https://greatx3.github.io/Yan/)
    - [Matrix-Game 2.0](https://matrix-game-v2.github.io/)
    - Feedback Guidance of Diffusion Models
    - Tweeter
      - Rosinality
      - minRF

#### Exploration
- Distill
  - Thread: circuits

<br>

## Presentations in Course
|Date|Subject|Papers|
|:-:|:-:|:-|
|2025-9-10|[AR1](./notes/250910.md)|- Conditional Image Generation with PixelCNN Decoders (van den Oord et al., 2016) <br>- (v) Mamba: Linear-Time Sequence Modeling with Selective State Spaces (Gu & Dao, 2023)  <br>- (v) Autoregressive Model Beats Diffusion: Llama for Scalable Image Generation (Sun et al., 2024) <br>- (v) Learning to (Learn at Test Time): RNNs with Expressive Hidden States (Sun et al., 2024) <br> - (v) Visual Autoregressive Modeling: Scalable Image Generation via Next-Scale Prediction (Tian et al., 2024)|
| 2|[]()|- |
| 3|[]()|- |
| 4|[]()|- |
| 5|[]()|- |
| 6|[]()|- |
| 7|[]()|- |
| 8|[]()|- |
| 9|[]()|- |
|10|[]()|- |



