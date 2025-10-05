# Physics of Language Models
[@AllenZhu-icml2024-tutorial]

## Studying LM
![](./images/physics_lm_001.png)
- Controlled experiment on LLMs!

<br>

## Knowledge Augmentation
![](./images/physics_lm_002.png)
- Only Knowledge Augmented Pretraining leads to the knowledge extraction.
  - cf.) Pretraining and fine tuning the knowledge leads to poor inference performance
  - Why?)
    - Knowledge augmentation changes the way the model learns the data   
      ![](./images/physics_lm_003.png)
      - Shown by probing the attention
- Plentiful data can help learning the sparse data.
  - Consider that the celebrities have rich data, while minorities have less data.
  - The former helped extracting knowledge from the latter   
    ![](./images/physics_lm_004.png)

<br>

## Knowledge Manipulation
- Using COT during training enhances the knowledge extraction.
  - e.g.) COT during the fine tuning stage   
    ![](./images/physics_lm_005.png)
- Reversing knowledge in fine tuning is impossible   
  ![](./images/physics_lm_006.png)

<br>

## Scaling Laws
![](./images/physics_lm_007.png)
- LLMs 2bit per parameter in storing knowledge that are exposed a lot
- 1bit for less exposed (rare) data   
  ![](./images/physics_lm_008.png)
- Exposure to the junk data significantly harms the knowledge capacity on good data   
  ![](./images/physics_lm_009.png)
  - Just by providing the domain address, LLMs automatically learns which source is good or bad.