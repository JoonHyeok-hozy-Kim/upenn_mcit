# Levenshtein Distillation on Edit Flow

### Ideation
- [Edit Flow](../reference/DDM/edit_flow.md)
  - Edit Flow implicitly learns the shortest probability path from state $`x_0`$ to $`x_1`$.
  - However, the paper only observes that phenomena but does not provide theoretical description for it.
- [Levenshtein Transformer](../reference/NAR/levt.md)
  - It utilizes the oracle (teacher) model of Levenshtein distance as a metric of the shortest path.
- hozy idea
  - Argument
    - Can we distillate Edit Flow using the Levenshtein distance?
  - Possible Outcome
    - We may explicitly teach the shortest (optimal) transport between to states.
      - Faster sampling?
  - Possible Hardship
    - Edit flow uses MSE on the continuous framework.
    - Levenshtein distance is a discrete metric.
    - Can these two be incorporated into a shared learning procedure?