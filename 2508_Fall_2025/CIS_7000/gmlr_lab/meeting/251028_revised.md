# Lab Meeting (Oct. 28)
---
Here’s a summary of the project updates discussed today:

*Yonghyun*
Yonghyun is working on two main projects:

* *1. Video Prompting Model*
    * *Goal:* Achieve in-context learning.
    * *Current Status:* Focusing on reasoning and logical puzzles (e.g., Sudoku) using the open-source *Wan2.2* model.
    * *Experiments:* Testing the base model on simple tasks (like rotation, resizing, symmetry) with language prompts and an initial image.
    * *Findings:* Most simple tasks failed, which is likely due to poor prompt alignment in the open model. However, maze-solving tasks were successful.
    * *Next Step:* Transitioning to using *video prompts*.

* *2. Distillation of pi-Flow*
    * *Goal:* Reproduce the pi-Flow paper with a simplified implementation, as the original is overly complex (using *minRF* as a reference).
    * *Challenge:* The Gaussian Mixture Model approach appears to be unstable and less expressive.

---

*Han & YiFan*
* *Project:* World Models for Robots.
* *Goal:* Use a combination of RGB video and a controllable "action map" to represent robot actions.
* *Method:* Leveraging pretrained models to learn this representation.
* *Current Status:* Actively implementing a diffusion model that incorporates the action map.
* *Discussion:* While full video input could enhance robot models, it's computationally expensive. Action maps are being explored as a more efficient alternative.

---

*Andrew*
* *Project:* 3D Motion Generation.
* *Challenge:* High cost of 3D motion data.
* *Proposed Solution:* Using 2D data as a substitute.
* *Explored Methods:*
    * Score Distillation Sampling (SDS) and Reinforcement Learning (but these are costly at test time).
    * Distillation is being considered as a strong candidate approach.
* *Next Steps:* Benchmarking existing papers and creating sequences of prompts to compensate for the lack of 3D data.
* *Note:* Confirmed that the 2D data source is consistent and capable of generating skeleton data.

---

*Joon*
* *Project:* Continuing work on Discrete Diffusion Models.
  * Working on
    * reading insertion based DDM model papers
    * implementing MDLM based on minRF

---

*Yiqian*
* *Project:* Currently implementing an *action denoiser*

---

*Zhiyuan*
* *Project:* Explicit Chain-of-Thought (CoT) mechanism for Text-to-Speech (TTS).
* *Concept:* Exploring a "perpetual CoT cycle." The idea is for the model to generate internal reasoning placeholders even during conversational pauses when no audio is present (similar to how Deepseek uses RL for reasoning).

---

*Guancheng*
* *Project:* Deep Research Agent.
* *Goal:* Building a system similar to GPT for deep research assistance.
* *Context:* This work is part of a *NeurIPS competition* focused on building and optimizing deep research systems.

---

*Mutian*
* *Project 1:* 3D/4D Trajectory-Aware Video Generation.
    * *Experiments:* Testing two different representations: (1) frame-by-frame 3D vs. (2) a more coherent trajectory representation from the first frame. Also experimenting with changing viewpoints.
* *Project 2:* Unified World Model for Robotics (collaborating with Han).
* *Workshop Debrief (from Prof's talk on 3D World Models):*
    * *Key takeaway:* Researchers have differing definitions of "spatial intelligence."
    * *Open Question:* Is an LLM necessary for spatial intelligence? (Perhaps for tasks like Normalizing Flow, but animals demonstrate spatial intelligence without language).

---

*Lulu*
* *Project:* Inference-Time Scaling for Video Generation.
* *Method:* Studying a paper that uses a "tree of frames" approach—calculating scores, using a heap sort, and pruning low-score branches to guide video generation.
* *Current Status:* Learning the experimental setup from this paper.
* *Other Experiments:* Testing different prompting strategies, such as using only yes/no questions.

---

*Yao*
* *Project:* Latent Reinforcement Learning.
* *Workshop Debrief (related to Mutian's):*
    * *Discussion:* Focused on tokens in visual problems and methods for achieving fine-grained control.