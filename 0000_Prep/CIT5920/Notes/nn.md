- [Back to Main](../main.md)

# 16. Induction 2
### Concept) Strong Induction
- Template)
  - Let
    - $`P(n)`$ be a predicate parametrized by a natural number $`n`$.
    - $`a\le b`$
      - where $`a,b\in \mathbb{R}`$.
  - Then
    - $`P(n)`$ is true for all $`n\ge a`$, if the following two conditions hold:
      1. The Base Case
         - $`P(a), P(a+1), \cdots, P(b-1)`$ are true.
      2. Inductive Step
         - $`\forall k \ge b`$
           - $`P(j) \textrm{ is true for } j \in [a,k) \Rightarrow P(k) \textrm{ is true }`$
  - Alternatively...
    - $`P(a) \wedge P(a+1) \wedge P(k-1) \rightarrow P(k), \forall k\ge b`$

<br>

#### E.g.) Division Theorem
- Theorem)
  - Let $`n`$ be a positive integer.
  - Then for every non negative integer $`m`$, there exist unique integers $`q`$ and $`r`$ such that $`m = nq+r`$ and $`0\le r \lt n`$.
- pf. by induction)
  - 


<br><br>

- [Back to Main](../main.md)