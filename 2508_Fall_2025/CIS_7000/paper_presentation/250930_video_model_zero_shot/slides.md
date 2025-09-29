---
marp: true
style: |
  section {
    margin-top: 5px;
    line-height: 1;
  }
  .left { 
    margin-left: 40px;
    text-align: left; 
  }
  .center { text-align: center; }
  .right { text-align: right; }

  .flex-container {
    display: flex;
    align-items: center;
    justify-content: space-between;
  }

  .flex-container-left {
    display: flex;
    align-items: center;
    justify-content: flex-start;
  }

  .right-img {
    margin-left: 5px;
  }

---

# Video models are **zero-shot** learners and reasoners
Google Deepmind 2025

<br><br><br><br><br>
<div class="right">Reviewed & Presented by Joon Hyeok Kim</div>

---

# Contents
### 1. Key Idea 1 : Analogy with **LLMs** as a Generalist Model
### 2. Key Idea 2 : **Hierarchical Categorizations** of Visual Capabilities
### 3. Methods, Evaluations, & Experiments
### 4. Limits & Future Outlook

---
### Recall the history of the language models.
#### The Pre-LLM Era was like the 群雄割据(군웅할거) of ...

<div class="flex-container">
  <div>

## Task-Specific Bespoke Models
- Translation
- Summarization
- Domain specific QnA
- ...

  </div>
  <div> 
  <img class="right-img" src="./images/001.webp" width="500px">
  </div>
</div>


---
## **LLM** unified and started to work as a Generalist

<div class="flex-container">
  <div>

- Translation
- Summarization
- Domain specific QnA
- And, now even capable of
  - Coding
  - Math
  - Creative writing
  - Deep research (oh my...)
  - and so on...

  </div>
  <div> 
  <img class="right-img" src="./images/002.png" width="500px">
  </div>
</div>


---
## Paradigm Shift : **Chain-of-Thoughts (CoT)** + Computing Power

<div class="flex-container">
  <div> 
  <img class="right-img" src="./images/004.png" width="800px">
  <br>Chain-of-Thought Prompting Elicits Reasoning in Large Language Models
  <br>Google Research, Brain Team 2022
  </div>
  <div> 
  <img class="right-img" src="./images/003.webp" width="200px">
  </div>
</div>

---
# Analogy in Video Models
### Current task-specific video models that outperform general video models...
<br>
<div class="flex-container">
  <div> 
  Segmentation Specialist
  <br><br>
  <img class="right-img" src="./images/007.png" width="600px">
  </div>
  <div> 
  Object Detection Specialist (YOLO variants)
  <br><br> 
  <img class="right-img" src="./images/008.png" width="600px">
  </div>
</div>



---
## Can **video models** become **Generalists** just like **LLM** did?

<div class="flex-container">
  <div> 
  <img class="right-img" src="./images/009.webp" width="600px">
  </div>
  <div> 
  <img class="right-img" src="./images/010.webp" width="600px">
  </div>
</div>

---
Maybe yes with...
## Chain-of-Frames (**CoF**) + Future Developments
- Applying changes across **dimensions** of the real world frame-by-frame
  - Dimensions : Time & Space
  - Similar to Step-by-step strategy in CoT

![w:1200px](./images/005.png)

#### Still, conceptual and no analytic mechanism studied yet...
---
# 2. Hierarchical Categorizations of Visual Capabilities
![w:1200px](./images/006.png)
#### Provides a **framework** to assess various abilities of Video Models

---
# Stacking up! 
![2:1200px](./images/011_5.png)

[Examples](https://video-zero-shot.github.io/)

---

# 3. Methods, Evaluation, & Results
![](./images/012.png)   
### e.g.   
  ![h:240px](./images/021.png)   



---
# Evaluation Methodologies
## **Qualitative** Evaluation
#### Concept) Success Rate 
- Def.)
  - The fraction of generated videos that solved the task
- Props.)
  - Determined by **humans**
  - $\gt0$ : the model possesses the ability to solve the task
  - $\approx1$ : the model reliably solves the problem irrespective of the random seed

---

|||
|:-:|:-:|
|Success|![](./images/016_1.png)|
|Failure|![](./images/016_2.png)|

---
## **Quantitative** Evaluation
### Problem specific scores are used.
- No unified measure nor statistics
### Consider both the **best frame** and the **last frame**
- Best Frame : Best performance but not deterministic
- Last Frame : Deterministic but no guarantee on performance

---
##### e.g.) Segmentation Problem **(Perception)** :  Intersection over Union (IoU)
![](./images/017.png)

---
## e.g.) Maze Solving Problem **(Reasoning)**
#### Compare...
<div class="flex-container">
  <div> 
  <img class="right-img" src="./images/014.png" height="150px">
  </div>
  <div> 
  <img class="right-img" src="./images/009.webp" height="150px">
  </div>
  <div> 
  <img class="right-img" src="./images/014.webp" height="150px">
  </div>
  <div> 
  <img class="right-img" src="./images/014_2.webp" height="150px">
  </div>
</div>

---
# For Video Models... (Veo 2 & Veo 3)

#### 1. Provide a maze  image to the **prompt** as the first frame
![](./images/013_1.png)
- They tried various maze datasets.
  - `maze-dataset 0.3.4`
  - Hand drawn irregular mazes (flipped/rotated) to get 40 unique samples

---

#### 2. Prompt text as below
![w:1200px](./images/013_2.png)

---

#### For Other Reference Models
- Nano Banana (Image Specific Generative Model)   
  ![](./images/015_1.png)
- Gemini 2.5 Pro (Language Model)   
  ![](./images/015_2.png)

---

# Evaluation
#### Define Illegal Moves
- Jumping over walls
- Clipping through boundaries
- Alteration of the goal's position
- ...

#### Success Rate (Quantitative)
- The fraction of $k$ attempts where the agent successfully reaches the goal without illegal moves through out the generated video.

---

# Result

<div class="flex-container">
  <div>

- Veo2 vs Veo3
- $k\uparrow\Rightarrow \text{pass}\uparrow$
- Complex
  - Beats LLM
- Simple, Irregular 
  - Beats Image

  </div>
  <div> 
  <img class="right-img" src="./images/018.png" width="1000px">
  </div>
</div>

---

## Limit : Insufficient **Reasoning** Capability
#### Significant portion of the **reasoning** experiments scored low success rates.
![h:250px](./images/019_1.png)   
![h:250px](./images/019_2.png)   

---

#### Failures on complex tasks
![](./images/020_1.png)   
![](./images/020_2.png)   

---
# More Limits...
### No analytic relation proven between **CoF** and **Zero-shot learning**
- Does CoF guarantees the correct reasoning path?
- Or, does "correctness" even matter if we have the answer?
### No unified evaluation method : Human and problem dependent
### Video generation is still too computationally expensive
- Emergent abilities only at scale! (Veo 3's performance improvement)
### High Dependency on Prompt Engineering
- Nevertheless, CoT did change the world...
### Jack of many trades, master of few
- Fine-tuned models dominate in specific tasks. Will they last forever?

---

# Future Outlook
### Video Models Show Great Potential for Zero-Shot Learning
- Distinguish a model's task performance and its underlying ability to solve it.
- Early LLMs underperformed against fine-tuned models.
  - But look who dominates now, huh?
### We are at the very beginning of the Video Model development
- Improvement from Veo 2 to Veo 3 indicates further developments.
- Alternative approach on Prompt Engineering?
  - Current : first frame image + text description
  - Just like CoT changed the game with the prompt engineering in LLMs.
- Cost will fall : LLM's ongoing decrease in cost may support this.

### Leveraging Scaling Laws and Optimization 
- Performance can be further boosted by applying inference-time scaling and standard optimization toolkits, which were not used in this study

---

# Questions

<br><br><br><br><br>

Thank you