# Do Vision-Language Models Have Internal World Models? Towards an Atomic Evaluation
[@gaoVisionLanguageModelsHave2025]

### Concept) World Modeling
- Desc.)
  - Provide internal representations of the external world
  - Simulate how the current state transforms to the next

<br>

### Concept) Dual-Stage Conceptual Framework for World Models
- Two stages)
  - [Perception Stage](#concept-perception-stage)
  - [Prediction Stage](#concept-prediction-stage)
- Desc.)
  - This frame work explains...
    - how raw sensory signals are converted into compact world representations
    - how these representations then guide forward simulations

#### Concept) Perception Stage
- Desc.)
  - Agents form internal representations of the current state by sensing and encoding environmental stimuli.
- Sub-Dimension Definition)
  - Space and Time
  - Motion
  - Quantity
  - Vision


#### Concept) Prediction Stage
- Desc.)
  - Agents use [perceptions](#concept-perception-stage) to extrapolate future states
- Sub-Dimension Definition)
  - Mechanic Simulation
    - Understanding the causality of intuitive physical dynamics
  - Transitive Inference
    - Long horizon planning for the multi-step forecast
  - Compositional Inference
    - Merging partial pre-conditions into an overall post condition

<br>

### Concept) WM-ABench 
- Methods
  - Controlled Experiment and Causal Analysis
    - When evaluating a dimension, keep all other dimensions fixed.
  - Fighting Shortcuts and Spurious Correlations
    - Generate hard negative options.
      - e.g.) Counter factual states


<br><br>

## E. Evaluation and Reproducibility
### E.1 Human Evaluation
- Hire people from Amazon Mechanical Turk
### E.2 VLM Evaluation
- Give Prompt
  - You are a helpful assistant. You will be given a question to answer. If it is a multichoice question, return the index of your choice 1,2,3,4 or A,B,C,D depending on the question, and then followed by any explanation necessary. If it is a yes/no question, clearly answer “yes” or “no” at first, and then follow with your explanation if needed. If you are asked to choose the images, please note that the last four images of all the given images are your choices. And your answer should be 1 or 2 or 3 or 4 pointing to these last four images.