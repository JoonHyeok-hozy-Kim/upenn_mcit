### Binary & Hexadecimal
<details>
  <summary>Calculate addition between 2C binary #s: 11 + 01010101</summary>
  <p>
  
  $`\begin{aligned}
    11111111& \\
    01010101& \\
    \_\_\_\_\_\_\_ \\
    01010100& = 84
  \end{aligned}`$
  
  </p>
</details>

<details>
  <summary>Describe what conditions indicate overflow has occurred when two 2's complement numbers are added. </summary>
  <p>
  
  Both operands are positive and the result is negative or vice versa.
  
  </p>
</details>




### Floating Point
- [Reference](../notes/m01.md#concept-floating-point)

<details>
  <summary>How many digits?</summary>
  <p>
  
  32-bit
  
  </p>
</details>

<details>
  <summary>Components?</summary>
  <p>
  
  ![](../images/m0102/014.png)
  
  </p>
</details>

<details>
  <summary>Convert the decimal -55 23/64 to Floating Point</summary>
  <p>
  
  - 1 10000100 10111010111000000000000
  
  </p>
</details>

<details>
  <summary>Convert the floating point # 1 10000011 00010000000000000000000 to decimal</summary>
  <p>
  
  - $`-17`$
  
  </p>
</details>

<details>
  <summary>Convert the floating point # 0 111111110 0000000000000000000000 to decimal</summary>
  <p>
  
  - +infinity
    |Exponent|Mantisa|Value|
    |:-:|:-:|:-:|
    |255|0|infinity|
    |255|~0|NaN|
  
  </p>
</details>

<details>
  <summary>Maximum/Minimum value of exponent</summary>
  <p>
  
  - $`-126 \le e \le 127`$
    - Why?)
      - Recall $`\text{Exp}`$ is an 8-digit unsigned binary number.
        - Thus, $`0 \le \text{Exp} \le 2^8-1 = 255`$.
      - Also, $`00000000`$ and $`11111111`$ are special cases that should be excluded.
        - Hence $`1 \le \text{Exp} \le 2^8-2 = 254`$.
      - Putting $`e = \text{Exp}-127`$
        - $`-126 \le e \le 2^8-2 - (2^7-1) = 2^7-1 = 127`$
  
  </p>
</details>

<details>
  <summary>Let xF0 be the hexadecimal representations of 2's complement binary numbers. Convert it to decimal numbers.</summary>
  <p>
  
  - $`\mathbf{x}F0 = 1111 0000 = -128 +64 +32 +16 = -16`$
  
  </p>
</details>

