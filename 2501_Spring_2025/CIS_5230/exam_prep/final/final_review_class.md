# 🧾 Final Review Summary – Ethical Algorithm Design & Privacy

## 🔹 Fairness in Machine Learning

### 🧠 Motivation
- Fairness is not a singular concept; multiple conflicting definitions exist.
- Bias can arise without malicious intent—due to data imbalance, poor feature choice, or group difficulty.
- Need to distinguish between:
  - **Statistical (Group) Fairness**
  - **Individual Fairness**
  - **Intersectional Fairness**
  - **Fairness Gerrymandering**

---

### 📊 Group Fairness Metrics

- Based on comparing entries in the confusion matrix across groups.
- Examples:
  - Equal False Positive Rates (FPR)
  - Equal False Negative Rates (FNR)
  - Equalized Odds
  - Demographic Parity
- Example: ProPublica vs. COMPAS (Northpointe) case:
  - COMPAS claimed AUC fairness.
  - ProPublica focused on unequal FPR among Black vs. White defendants.

> 📌 Tradeoff: You often cannot satisfy all fairness definitions at once.
> → Leads to the **Fairness Impossibility Theorem** (can’t satisfy all desirable constraints simultaneously).

---

## 🎲 Bolt-On Fairness (Selective Randomization)

- Designed for a **fixed pretrained model** (no retraining).
- If group A has higher error than group B, deliberately inject randomness into B’s predictions to raise B’s error.
- Result: Equalizes error at the cost of overall accuracy.

### Key Idea:
- Enables "fairness adjustment" post-training.
- Achieves a linear Pareto frontier (trade-off curve) between error and fairness.

> ⚠️ Downside: Decreases model utility.

---

## 📈 Pareto Frontier of Fairness vs. Accuracy

- Defines the set of **undominated** solutions (no point is better in both fairness and error).
- The Bolt-On line is a subset of this.
- Other models (e.g., via retraining) may dominate Bolt-On (lie southwest on the plot).

---

## 🎮 Game-Theoretic Algorithm (Regret-Based Optimization)

- Framework:
  - **Learner** selects models minimizing error.
  - **Regulator** identifies fairness constraint violations.
  - Learner adjusts the objective accordingly.
- Final model: **Uniform mixture over all models** selected during T rounds.

### Video Visualization:
- X-axis: overall error  
- Y-axis: max disparity across subgroups  
- Goal: reach below a fairness threshold (e.g., γ = 0.5) while minimizing error.

---

## ♻️ Generalizing Fairness: Intersectional Subgroup Constraints

- Going beyond disjoint group definitions (e.g., male vs. female).
- Handle exponentially many intersectional subgroups (e.g., Hispanic women over 55 with disabilities).
- Algorithm still efficient using implicit optimization.

---

## 📌 Individual Fairness (Conceptual)

- “Similar individuals should be treated similarly.”
- Examples:
  - Two loan applications are nearly identical; only one is approved → unfair?
- Hard to formalize.
- Recent approaches use implicit constraints on pairs of individuals.

---

## 🧰 Bias Bounties (Model Patching via Crowdsourcing)

- Inspired by software bug bounty programs.
- If a user finds a subgroup + better model on that subgroup:
  - Replace original model with conditional logic:
    - If input ∈ subgroup, use the new model.
    - Else, use original.
- Each addition decreases both subgroup error and overall error.
- Potentially infinite improvement via iterative patching.
- Challenge: Control model complexity and generalizability.

---

## 🔒 Security vs. Privacy

| Concept     | Security                         | Privacy                                        |
|------------|----------------------------------|------------------------------------------------|
| Definition | Control access to data           | Control inference from data                    |
| Method     | Cryptography (encryption, keys)  | Differential Privacy, Anonymization            |
| Goal       | Prevent unauthorized access      | Limit what can be learned about individuals    |

---

## 🕵️‍♂️ Differential Privacy (DP)

### Motivation
- Anonymization is brittle and vulnerable (e.g., triangulation attacks, Netflix Prize, mobility data leaks).
- DP provides **formal mathematical guarantees**.

### Key Idea
- Compare the output of an algorithm on:
  - Dataset \( D \) (with your data)
  - Dataset \( D' \) (without your data)
- If the outputs are **indistinguishable**, your privacy is preserved.

### Definition
A randomized algorithm \( A \) is \( \epsilon \)-DP if for all adjacent datasets \( D, D' \) and all \( S \subseteq O \):

\[
\frac{1}{e^\epsilon} \le \frac{P[A(D) \in S]}{P[A(D') \in S]} \le e^\epsilon
\]

- \( \epsilon = 0 \): perfect privacy
- Larger \( \epsilon \): weaker privacy

---

## 🧪 DP Mechanisms

### 📍 Randomized Response (1965)
- Respond truthfully with 50% chance, else random.
- Provides plausible deniability.
- Satisfies \( \epsilon = \ln(3) \)-DP.

### 📍 Laplace Mechanism
- Add Laplace noise to output of function \( f \).
- Noise ∝ sensitivity of \( f \):
  \[
  \text{Laplace scale } b = \frac{\Delta f}{\epsilon}
  \]
- Works well for low-sensitivity functions (e.g., mean, median).

### 📍 Exponential Mechanism
- Sample output from space \( O \) with probability ∝ \( e^{\epsilon u(x,o)/2\Delta u} \)
- For utility functions \( u(x,o) \) (e.g., model accuracy)
- Used when output is not numerical.

---

## 🧬 DP Properties

### ✅ Post-processing Invariance
- Any function of DP output is still DP.

### ✅ Composition
- Multiple DP computations on same data:
  - Privacy cost adds: \( \epsilon_{total} = \sum \epsilon_i \)

---

## 🧠 Generative AI Concerns

- Bias & Stereotypes
- Privacy leaks & Copyright/IP violations
- Toxicity / Illegal generation
- Hallucination (plausible but false)
- Cheating / Plagiarism (e.g., admissions essays)
- Adversarial Attacks
- Security breaches

### 🔍 Mitigations
- Curation of training data
- Guardrail models (e.g., reinforcement via human feedback)
- Differential Privacy in training
- Watermarking
- Machine Unlearning
- RAG (Retrieval-Augmented Generation)

---

## 🧮 Hallucination Analysis

### Kalai–Vempala Framework
- Facts = word tuples from finite space
- Only small subset of facts appear in training data
- Use Good-Turing estimation:
  \[
  \tau = \frac{\# \text{ facts occurring once}}{\text{training set size}}
  \]
- Hallucination Rate ≥ \( \tau - \text{(model miscalibration)} \)

---

## ✅ Summary Takeaways

- Fairness often involves hard trade-offs—must choose or balance.
- DP gives rigorous privacy but at a cost (noise, utility).
- Generative AI introduces new ethical challenges (hallucination, bias, copyright).
- Simple ideas like bolt-on fairness and bias bounties offer practical solutions, albeit limited.
- Understanding tradeoffs via Pareto Frontiers is crucial for both fairness and privacy design.
