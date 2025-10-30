# CIS 7000 Advanced Generative Models

## My Presentation
- Styled-GAN (250908)
  - [GAN Paper Summary](./paper_presentation/250908_style_gan/paper_note_gan.md)
  - [Style-GAN Paper Summary](./paper_presentation/250908_style_gan/paper_note_style_gan.md)
- (Stolen) Auto-Encoder Variational Bayes
  - [VAE Paper Summary](./paper_presentation/250917_vae/paper_note.md)
- Video Models as Zero-Shot Learners
  - [Video Models are Zero-shot Learners and Reasoners](./paper_presentation/250930_video_model_zero_shot/paper_summary.md)
- High-Resolution Image Synthesis with Latent Diffusion Models (250929)
  - [DDPM](./paper_presentation/250931_latent_diffusion/paper_summary/ddpm.md)
  - [Score-Based Model](./paper_presentation/250931_latent_diffusion/paper_summary/score_based_model.md)
  - [Latent Diffusion](./paper_presentation/250931_latent_diffusion/paper_summary/latent_diffusion.md)

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
    - [Rectified Flow](./project/references/basics/rectified_flow.md)
      - Implementation : [minRF](https://github.com/cloneofsimo/minRF/tree/main)
    - [Conditional Flow Matching](./project/references/basics/conditional_flow_matching.md)
    - [Stable Diffusion 3](./project/references/basics/sd3.md)
    - [(WIP) Mean Flow](./project/references/basics/mean_flow.md)
    - [(WIP) Video Diffusion Models](./project/references/basics/video_diffusion_model.md)
  - Methodologies
    - [Video models are zero-shot learners and reasoners]()
      - References)
        - [GPT]()
        - [CoT]()
    - [PAG](./project/references/methodologies/perturbed_attention_guidance.md)
      - Implementation
    - [SEG](./project/references/methodologies/smoothed_energy_guidance.md)
    - [PAG Attention Head Selection](./project/references/methodologies/attention_head_selection.md)
    - [(WIP) Phyworld](./project/references/evaluation_methods/how_far_video_world.md)
      - Main Physics Simulator
    - [ICML 2024: Physics of Language Models](./project/references/methodologies/physics_of_lm.md)
      - Implementing controlled experiments on LMs for
        - Knowledge augmentation
        - Knowledge manipulation
        - Scaling Laws
    - (Yet to be covered) IOI
    - (Yet to be covered) Physics of Language Models
    - (Yet to be covered) Thread: Circuits
    - (Yet to be covered) Sparse Autoencoders
    - (Yet to be covered) [Yan](https://greatx3.github.io/Yan/)
    - (Yet to be covered) [Matrix-Game 2.0](https://matrix-game-v2.github.io/)
    - (Yet to be covered) Feedback Guidance of Diffusion Models
    - [(WIP) Diffusion Forcing: Next-token Prediction Meets Full-Sequence Diffusion]() 
      - [Self Forcing](https://self-forcing.github.io/)
    - Tweeter
      - Rosinality
      - minRF
  - Evaluation Methods
    - [WM-ABench](./project/references/evaluation_methods/wm-abench.md)
      - 2 Stage : Perception, Prediction
      - Human evaluation (X)
    - [PhysBench](./project/references/evaluation_methods/physbench.md)
      - Solving predetermined 4 options questionnaires
      - Applicable only to the VLMs
    - [Physics-IQ](./project/references/evaluation_methods/physicsIQ.md)
      - Given the initial 3 sec., generate the rest 5sec, and compare with the original
      - Quantitative Stats : MSE, IOU
      - Needs original video to compare
      - Generation model must be capable of getting 3 sec video as an input

#### Exploration
- Distill
  - Thread: circuits

<br>

## Presentations in Course
|Date|Subject|Papers|
|:-:|:-:|:-|
|9/10|[AutoRegressive Models](./notes/250910.md)|- Conditional Image Generation with PixelCNN Decoders (van den Oord et al., 2016) <br>- (v) Mamba: Linear-Time Sequence Modeling with Selective State Spaces (Gu & Dao, 2023)  <br>- (v) Autoregressive Model Beats Diffusion: Llama for Scalable Image Generation (Sun et al., 2024) <br>- (v) Learning to (Learn at Test Time): RNNs with Expressive Hidden States (Sun et al., 2024) <br> - (v) Visual Autoregressive Modeling: Scalable Image Generation via Next-Scale Prediction (Tian et al., 2024)|
|9/15|Tokenizers|- |
|9/17|[Normalizing Flow](./notes/250917.md)|- |
|9/22|[Variational Autoencoder](./notes/250922.md)|- |
|9/29|[Energy Based Models](./notes/250929.md)|- |



