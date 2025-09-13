# High Resolution Image Synthesis with Latent Diffusion Models
Rombach et al. 2022

## 1. Introduction
#### Concept) Rate-Distortion Trade-Off
- Concepts)
  - Rate
    - How many bits are used to represent data
      - i.e.) **Compression** efficiency
    - Lower rate = stronger compression
  - Distortion
    - How much information is lost compared to the original
    - Lower distortion = reconstruction is closer to the original
  - Trade-Off
    - High **compression** usually increase **distortion**
- Desc.)
  - Relation with the Diffusion Models
    - In pixel space, diffusion models must handle all pixel-level details.
    - This includes high-frequency noise that is perceptually meaningless.
    - However, most bits in a digital image correspond to imperceptible details.
    - Thus, LDM tries 
      - Perceptual Compression
        - Train an autoencoder to remove redundant high-frequency details while keeping perceptual equivalence
        - This reduces rates significantly.
      - Semantic Compression
        - Train the diffusion model in the latent space of the autoencoder
  - Comparison with other models   
    ![](../images/latent_diffusion/001.png)
    - Desc.)
      - Autoencoder + GAN preserves nearly all pixel details.
      - LDM use 
        - autoencoder for perceptual compression
        - diffusion in the latent space for semantic compression

<br>

## 2. Related Work
### Concept) Diffusion Probabilistic Models (DM)
- Strength)
  - Achieved state-of-the-art results in 
    - density estimation
      - Desc.)
        - Suppose we trained $`p_\theta(x)\approx p(x)`$.
        - Here, we use methods such as MLE or ELBO, which maximize the likelihood in the training dataset.
        - Then, evaluate $`\log p_\theta(x')`$, which is the average test log-likelihood, where $`x'`$ is the test dataset.
        - Compare it with other models to have greater values.
    - sample quality
      - e.g.) IS, FiD scores
  - The generative power of these models stems from a natural fit to the inductive biases of image-like data
    - when implemented with [UNet](./ddpm.md#implementation)
    - when a reweighted objective is used for training.
      - cf.) reweighted objective
        - Recall that [DDPM](./ddpm.md#34-simplified-training-object) used $`t\sim\text{Uniform}[1,T]`$.
        - Instead, 