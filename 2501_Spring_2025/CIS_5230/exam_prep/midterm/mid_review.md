# 1.
- Required 2 readings
  - try to mitigate more than just observing and regulating


# 2.
- Framework stuffs
  - $`\langle x, y\rangle\sim P`$
  - Fundamental thm of machine learning
    - enough data relative to the complexity of the model case


# 3.
- Propublica vs Northpointe
  - PP : got their hands on COMPAS where people were getting interest in ML
  - COMPAS : 
    - complete survey. so many sides were considered.
    - Input : data even the ones that are directly relevant to recidivism
    - Output
      - recidivism
      - other outputs but PP ignored
  - PP got the data from B county FL that only discloses the criminal records.
    - Joining this with COMPAS data, figured out wether they recidivated or not.
    - Binary classification with threshold 8, high/low risk
    - Compute the error rate
      - FPR : PP says its more harmful. Falsely being tagged as the recidivism
        - FP was high on Black
        - FN was high on Whites
  - Northpointe
    - Never used recidivism score for actual judgement
    - It cared the demographic fairness
      - Using AUROC : different measure of fairness than FPR
        - Different curves for Black and White
        - They lied on top of each other
      - If PP used another threshold, they could have gotten the same FP between black and white
      - Different threshold must have been used.
    - Not looking at sensitive features cannot mitigate the problem.
      - Why?)
        - Other features reveal sensitive features
          - Zipcode, SAT score
            - Race, wealth
          - Not looking at these features cannot 

# 4. Bias Mitigation
- Paper 1)
  - Individually good condition of fairness cannot be simultaneously satisfied.
  - More constraint, less solution
    - e.g.) equalizing FP
- Paper 2)
  - read
- Google Pass)
- Paper 3) Bolt on method
  - But not from paper, we used Pareto / H-frontier
    - red : our algorithm
    - blue : applying intersection : marginal


### Questions)
1. ROC touching the top left corner : Overfitting?
2. Red / Blue Pareto frontier
   1. Which model corresponds to what
      1. Game theoretic approach the the regulator uses pairwise error rate 
         1. chooses two subgroups for the fairness
      2. Individual error rates for each subgroup
         1. Is this the model that mitigates the Fairness Gerrymandering?
            1. Then is it the blue curve?
               1. broader features
            2. Red Curve
               1. combinatorial constraints