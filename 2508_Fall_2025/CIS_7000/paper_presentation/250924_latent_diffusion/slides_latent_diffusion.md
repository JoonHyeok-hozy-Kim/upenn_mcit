---
marp: true
style: |
  section {
    margin-top: 5px;
    line-height: 1;
  }
  .left { text-align: left; }
  .center { text-align: center; }
  .right { text-align: right; }

  .flex-container {
    display: flex;
    align-items: center;
    justify-content: space-between;
  }

  .right-img {
    margin-left: 5px;
  }

---

# High Resolution Image Synthesis with Latent Diffusion Models (**LDM**)
Rombach et al. 2022

<br><br><br><br><br>
<div class="right">Reviewed & Presented by Joon Hyeok Kim</div>

---

## Text Image aligned sample

<div class="flex-container">
  <div>

- Blabla

  </div>
  <img class="right-img" src="./images/ddpm/001.png" width="400px">
  <div> 
  
  </div>
</div>

---

# Contents
### 1. Weaknesses of **DDPM**
### 2. Suggestion : **Latent** Diffusion Model (**L**DM) $=$ DDPM $\times$ **VAE** 
### 3. Upgrade : **Conditional** LDM $=$ DDPM $\times$ VAE $\times$ **(Cross) Attention**
### 4. Pros, Cons, and **Updates** of LDM : **Stable Diffusion** v1 $\rightarrow$ v2 $\rightarrow$ v3

---

# Questions

<br><br><br><br><br>

Thank you