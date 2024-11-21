# K-Means Clustering
- Desc.)
  - $`K`$ number of clusters
    - hyperparameter
- Algorithm)
  - Randomly initialize $`K`$ initial center points.
  - Repeat the followings
    - Color your data by closest center
      - Classification!
    - Update each cluster center to the average point in its cluster.
  - Terminate when nothing changes
- Prop.)
  - Converges real fast!
  - Very robust.
    - Choose a proper random initial cluster center.
- Drawback
  - If the cluster is in ellipse shape the distance is hard to calculate.
- Diserderta)
  - How to Tune
    - How choose the hyperparameter $`K`$
      - For a point $`x_i`$, define $`c(x_i)`$ to return the cluster center.
        - $`D = \frac{1}{n}\sum_{i=1}^n\Vert x_i -c(x_i)\Vert_2^2`$
      - $`K=n`$ will be the best but meaningless model. 


<br><br>

# Transformers
Deep Learning in general

- Transforming text into numbers that we can setup a model and make predictions
  - Ver1) Bag of Words.
    - In each bag count the number of occurrence.
    - Convert sentences into vectors using those counts.
    - Problem)
      - each words are orthogonal to each other
      - Does not consider the semantics in human language.

### Self Trained model
- Don't know the label at first
- After training we get the label
- And make predictions based on them

