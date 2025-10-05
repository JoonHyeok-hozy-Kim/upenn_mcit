# Do generative video models understand physical principles?
[@motamedGenerativeVideoModels2025]

### Concept) Physical IQ
- Methodology
  - Get an 8 seconds length video.
  - Split it into starting 3 seconds and the remaining 5 seconds.
  - Input the former into the generative model, and generate video for the 5 seconds after the last frame.
  - Compare 5 seconds from the dataset and the generation
    - How?)
      - Frame by frame comparison
      - Comparison statistics
        - MSE : how action happens
          - Lower the better
        - Spatiotemporal IoU : where and when action happens
        - Weighted Spatial IoU : where and how much action happens
        - Spatial IoU : where action happens
      - Sum the above four scores to Physics-IQ score
      - Additionally,
        - MLLM score
          - Ask LLM to identify the generated video among the real and the generated one.