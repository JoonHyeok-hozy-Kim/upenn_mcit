[Back to Main](../../README.md)

# CIS 520 Machine Learning

|No|Subject|Keywords|
|:-:|:-|:-|
| 1|[Supervised Learning and Error](notes/01.md)|- Model, Training <br> - Supervised Learning : Classification, Regression <br> - Training Error, Loss, Mean Squared Error, Empirical Risk Minimization <br> - Generalization, True Risk, Generalization Gap|
| 2|[k Nearest Neighbors (k-NN)](notes/02.md)|- Minkowski Distance, Normalized Compression Distance (NCD) <br> - Curse of Dimensionality|
| 3|[Linear Models and Perceptron](notes/03.md)|- Linear Predictors <br> - Perceptron : Separability, Convergence Theorem|
| 4-5|[Gradient Descent](notes/04.md)|- Taylor Approximation, Learning Rate <br> - ADAGRAD <br> - Newton's Method <br> - Convexity, L-Smoothness <br> - Convergence, Convergence Rate <br> - $`\mu`$-Strongly Convex|
| 6|[Linear Regression](notes/06.md)|- Steps of Learning <br> - Linear Regression <br> - Overfitting <br> - Regularization : $`L_2`$, $`L_1`$, Elastic Net <br> - Ridge Regression, Lasso|
| 7|[Logistic Regression](notes/07.md)|- Squashing Function : Sigmoid Function|
| 8|[Support Vector Machine (SVM)](notes/08.md)|- Margin <br> - Hard Margin SVM <br> - Soft Margin SVM|
|9-10|[Kernels Part 1 & 2](notes/09.md)|- Feature Map $`\phi(\mathbf{x})`$ <br> - Kernel Trick <br> - The Radial Basis Function (RBF) Kernel (Gaussian Kernel) <br> - Kernel Constructions|
|11|[Gaussian Process](notes/11.md)|- Multivariate Gaussian Distribution, Covariance, Correlation <br> - Periodic Kernel <br> - Gaussian Process Regression|
|12|[Neural Networks](notes/12.md)|- Activation Function, Hidden Layers, Hidden Representation <br> - Neural Network : Training, Regularization <br> - Stochastic Gradient Descent (SGD)|
|13|[Decision Trees](notes/13.md)|- Split, Purity <br> - Loss Functions : Entropy, Gini Index|
|14|[Random Forest](notes/14.md)|- Bagging <br> - Random Forest: Subsampling Features, Out of Bag (OOB) Estimation|
|15-16|[Boosting](notes/15.md)|- Weak Learner : Decision Stump <br> - Gradient Boosted Regression Trees (GBRT) <br> - AdaBoost : Exponential Loss, Categorical Distribution (Multinomial Distribution)|
|17-18|[Bias, Variance, and Model Performance](notes/17.md)|- Bias-Variance Decomposition, Noise|
|19|[Performance Measures](notes/19.md)|- Class Probability Estimation (CPE) model <br> - 0-1 Loss, Cost-Sensitive Loss(Asymmetric Classification Cost), Weighted Surrogate Loss Minimization <br> - Confusion Matrix, True Positive Rate (TPR), True Negative Rate (TNR), Arithmetic Mean (AM), Geometric Mean (GM), Recall and Precision<br> - Receiver Operating Characteristic (ROC) Curve, AUROC|
|20|[Dimensionality Reduction](notes/20.md)|- Principal Component Analysis : (Maximum Variance Detection) vs (Minimizing Reconstruction Error) <br>- Fractional Reconstruction Accuracy|
|21|[K-Means Clustering](notes/21.md)|- |
|22|[Transformer](notes/22.md)|- |




<br>

- PyTorch HWs
  |Note|Concepts|
  |:-:|:-|
  |[HW0](hw/00/CIS5200%20HW0%20Revisited.ipynb)|- Tensor (matrix) multiplication : `unsqueeze`, `repeat` <br> - Stat functions : `mean`, `var` <br> - Plotting with `matplotlib`|
  |[HW1](hw/01/CIS5200%20HW1%20Revisited.ipynb)|- Perceptron, Plotting with `matplotlib` <br> - k-Nearest Neighbors (MNIST data)|
  |[HW2](hw/02/CIS5200_HW2Revisited.ipynb)|- Log Regression (Scikit Learn : Wine quality data) <br> - Ridge Regression <br> - Soft SVM (Scikit Learn : Breast cancer data)|
  |[HW3](hw/03/CIS5200_HW3_Revisited.ipynb)|- Decision Tree, Bagging (Bootstrap Aggregating), Boosting (Scikit Learn : Breast cancer data) <br> - `torch.full`|
- Exam Prep
  - [Mid Term](exam_prep/mid_term.md)



[Back to Main](../../README.md)