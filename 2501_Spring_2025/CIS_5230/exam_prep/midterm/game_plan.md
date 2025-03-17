# Midterm Game Plan

## 1. Notes

|nO|Notes|Readings Summary|Keywords|
|:-:|:-|:-|:-|
|1|[Foundations of ML](../../notes/01.md)||- |
|2|[Bias in Machine Learning: COMPAS and ProPublica](../../notes/02.md)||- |
|3|[Science of Fair ML: Models and Algorithms](../../notes/03.md)|||
|4|[ML Fairness via "Bias Bounties"](../../notes/04.md)||- |

- Cause of bias
  1. Imbalanced training data
  2. Feature selection bias
  3. Data quality issue
  4. Inherent predictability of some subgroups
  5. Loss minimization training methodology issue
  6. Feedback loop

## 2. Papers

|No|Note|Required|
|:-:|:-|:-|
|1-1|[Responsible AI in the generative era](https://www.amazon.science/blog/responsible-ai-in-the-generative-era)|Required|
|1-2|[Responsible AI in the wild: Lessons learned at AWS](https://www.amazon.science/blog/responsible-ai-in-the-wild-lessons-learned-at-aws)|Required|
|3-1|[Inherent Trade-Offs in the Fair Determination of Risk Scores](../../readings/03/01.md)|First 8 pages|
|3-2|[The Frontiers of Fairness in Machine Learning](../../readings/03/02.md)|Required|
|3-3|[(Google) Attacking discrimination with smarter machine learning](../../readings/03/03.md)|Required|
|3-4|[Equality of Opportunity in Supervised Learning](../../readings/03/04.md)|Intro|
|3-5|[A Reductions Approach to Fair Classification](../../readings/03/05.md)|Intro|
|3-6|[An Empirical Study of Rich Subgroup Fairness for Machine Learning](../../readings/03/06.md)|Intro|
|3-7|[An Algorithmic Framework for Bias Bounties](../../readings/03/07.md)|Intro, Section 5 and 6|
|3-8|[](../../readings/03/08.md)|Skim|

<br>

#### Paper 3-1) Inherent Trade-Offs in the Fair Determination of Risk Scores
- Three Fairness Conditions
  1. Calibration within Groups
  2. Balance for the Negative Class
  3. Balance for the Positive Class
- Impossible to satisfy all three fairness conditions simultaneously except for
  - Perfect Prediction
  - Equal Base Rates


<br>

#### Paper 3-2) The Frontiers of Fairness in Machine Learning
- Three Causes of Unfairness
  1. Bias Encoded in Data
  2. Minimizing Average Error Fits Majority Populations
  3. Since we cannot observe the prediction's performance until it happens, we should take actions we believe to be sub-optimal in order to gather more data.

<br>

#### Paper 3-3) (Google) Attacking discrimination with smarter machine learning
- Max Profit
  - Goal)
    - Just maximize the total profit.
  - Result)
    - Maximum profit.
    - The blue group is held to a higher standard than the orange one.
      - Unfair for the blue group.
- Group Unaware
  - Goal)
    - Hold all groups to the same standard
  - Result)
    - Poorest profit.
    - Unfairness
- Demographic Parity
  - Goal)
    - Let two groups receive the same number of loans.
      - i.e.) Fix **positive rate** for both blue and orange group to 37%.
  - Result)
    - Fewer qualified people in the blue group being given loans than in the orange group
    - Doesn't care about the default!
- Equal Opportunity (Hardt, Price, and Srebro)
  - Goal)
    - Fix TPR for each group to 68%.
  - Result)
    - It's possible to efficiently find thresholds that meet any of these criteria.

<br>

#### Paper 3-4) Equality of Opportunity in Supervised Learning
- Unfair Methods
  - Fairness through Unawareness
    - i.e.) Dropping sensitive features
  - Demographic Parity
    - i.e.) Assigning the positive class at the same rate across different demographic subgroups
    - $`P(h(X) = 1 \vert A = a) = P(h(X) = 1 \vert A = b)`$
- Proposal)
  - Equal Opportunity
    - i.e.) Fixing TPR across different demographic groups
    - $`P(h(X) = 1 \vert Y=1, A = a) = P(h(X) = 1 \vert Y=1, A = b)`$
- How?)
  - Bolt-on Method)
    - i.e.) Find an equalized odds or equal opportunity predictor $`\tilde{Y}`$ derived from an existing binary predictor $`\hat{Y}`$ or a existing score $`R`$.

<br>

#### Paper 3-5) Reductions Approach to Fair Classification
- Key Idea)
  - Reduce fair classification to a sequence of cost-sensitive classification problems
    - whose solutions yield a **randomized** classifier with the lowest (empirical) error subject to the desired constraints
  - A black-box access to a cost-sensitive classification algorithm.

<br>

#### Paper 3-6) An Empirical Study of Rich Subgroup Fairness for Machine Learning
- Fairness Gerrymandering
  - A model appears to be fair when evaluated on the broad subgroups exhibits unfairness at the intersections of the subgroups or at the individuals.
- Proposal)
  - Assumption)
    - Access to the black-box classifier (oracle)
  - Assertion)
    - As long as the class of functions defining these subgroups has bounded VC dimension
    - finding the best (distribution over) classifiers in $`\mathcal{H}`$ 
      - subject to the constraint of...
        - e.g.) equalizing the positive classification rate, the false positive rate, or the false negative rate 
      - over every subgroup defined over $`\mathcal{G}`$ 
      - is solvable whenever the **dataset size is sufficiently large** relative to the VC dimension of $`\mathcal{G}`$ and $`\mathcal{H}`$.

<br>

#### Paper 3-7) An Algorithmic Framework for Bias Bounties
- Bias Bounty Approach
  - Meaning)
    - External parties are invited to find biases in a model and reward doing so.