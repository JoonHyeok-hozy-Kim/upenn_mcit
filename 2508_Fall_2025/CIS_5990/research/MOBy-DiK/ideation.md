# Multi-Objective Bayesian Optimization with Diverse Solutions from K-clusters (MOBy-DiK)

### Goal)
Search diverse solutions along the Pareto Front

### Similar Works)
- DGEMO
  - Pros)
    - Provides diverse solutions along the Pareto Front
  - Cons)
    - Not effective on high dimensional feature space $`(N\gt6)`$
    - Local optimization 
      - The surrogate model (GP) is treated to be deterministic and $`L_2`$ optimization is performed on it.
      - 1st-order approximation on the surrogate model (GP).
- MORBO
  - Pros)
    - Effective on high dimensional feature space
    - Asynchronous local optimization allows faster search.
  - Cons)
    - Does not focus on providing diverse solutions
- ROBOT
  - Similarity)
    - Provides diverse candidate solutions in a single objective problem setting
  - Difference)
    - Does not support MOBO
  - Prop.)
    - User defined distance metric is used.

### Idea
- Use TuRBO's TR strategy for searching the high-dimensional feature space.
  - $`M`$ TRs that perform local optimizations.
- Find heuristics to suggest diverse datapoints by combining the existing best points from diverse TRs.
  - How?)
    - Clustering and Mixing
      - Cluster the existing best points from each TRs into $`K`$ clusters.
        - K-Means
        - DBSCAN
      - Create new TRs that encompasses the combinations of the points from each cluster.
        - For each of these new TRs, we may take advantage of the asynchronous optimization.
    - User defined distance metric