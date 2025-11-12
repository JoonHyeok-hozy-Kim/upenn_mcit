# Discrete Flow Matching
[@gatDiscreteFlowMatching2024]

## 2 Discrete Flow Matching
### 2.1 Setup and notations
- $`x = \left( x^1, x^2, \cdots, x^N \right)`$ : a sequence as an array of $`N`$ elements
  - where
    - each element (token) $`x^i`$ is selected from a vocabulary of size $`d`$
      - i.e.) $`x^i \in [d] = \{1,\cdots,d\}`$
- $`\mathcal{D} = [d]^N`$ : the entire set of possible sequences
  - where 
    - $`[d] = \{1,\cdots,d\}`$
- $`X`$ : a random variable taking values $`\mathcal{D}`$
  - where
    - $`P(X=x)`$ is the corresponding probability mass function 
      - Notation) $`p(x)`$ denotes the PMF as well.
- $`p(x^i) = \displaystyle\sum_{x^{\bar{i}}} p(x)`$ : the $`x^i`$ marginal of $`p`$
  - where
    - $`x^{\bar{i}} = (\cdots, x^{i-1}, x^{i+1}, \cdots) \in [d]^{N-1}`$
      - i.e.) $`x^i`$ is excluded from $`x`$
  - Desc.)
    - By marginalizing out $`x^{\bar{i}}`$, we focus only on a single token $`x^i`$
- $`\delta_y(x) = \displaystyle\prod_{i=1}^N \delta_{y^i}(x^i)`$ : the delta PMF function
  - where
    - $`x,y\in\mathcal{D}`$
    - $`\delta_{y^i}(x^i) = \begin{cases} 1 & x^i = y^i \\ 0 & x^i \ne   y^i \end{cases}`$
  - Prop.)
    - $`\delta_y(x) = 1 \Leftrightarrow x^i = y^i,\quad \forall i=1,\cdots,N`$
  - Notation)
    - $`\delta_y(x^{i}) = \delta_{y^{i}}(x^{i})`$
    - $`\delta_y(x^{\bar{i}}) = \delta_{y^{\bar{i}}}(x^{\bar{i}}) = \displaystyle\prod_{j\ne i}\delta_{y^{j}}(x^{j}) = \displaystyle\prod_{j\in\{1,\cdots,N\}\backslash i}\delta_{y^{j}}(x^{j})`$