# Mean Flows for One-step Generative Modeling
Geng et al. 2025

## 3. Background: Flow Matching
- Settings)
  - $`z_t = a_t x + b_t \epsilon`$ : a flow path    
    - where
      - $`x\sim p_{\text{data}}`$ : given data
      - $`\epsilon\sim p_{\text{prior}}`$ : a prior
      - $`t`$ : time
      - $`a_t, b_t`$ : predefined time dependent schedules
  - $`v_t = z_t' = a_t' x + b_t' \epsilon`$ : the velocity