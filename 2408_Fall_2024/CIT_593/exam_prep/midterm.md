## Binary & Hexadecimal
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
<br>

## Floating Point
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
  <summary>Convert the floating point # 0 11111111 00000000000000000000000 to decimal</summary>
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
<br>

## Transistors and Combinational Logic
<details>
  <summary>Draw transistor level Inverter, NAND, NOR gate</summary>
  <p>
  
  |||
  |:-|:-|
  |![](../images/m03/013.png)|![](../images/m03/017.png)|
  
  </p>
</details>

<details>
  <summary>Draw transistor level mux and decoder</summary>
  <p>
  
  <img src="../images/m03/043.png"><br>
  <img src="../images/m03/038.png">  
  
  </p>
</details>

<details>
  <summary>Draw logic gate level half adder and add/subtractor</summary>
  <p>
  
  <img src="../images/m03/032.png"><br>
  <img src="../images/m03/041.png">  
  
  </p>
</details>
<br>

## Sequential Logic to Memory
<details>
  <summary>Difference between Combinational and Sequential Logic</summary>
  <p>

  |Logic Type|Desc.|
  |:-:|:-|
  |[Combinational Logic](m03.md#3-transistors-and-combinational-logic)|- Always gives the same output for a given set of inputs <br> - Stateless|
  |Sequential Logic|- Its output depends on its inputs & its last output! <br> - Forms the basis for “state” or “memory” for a computer|
  
  </p>
</details>

<details>
  <summary>Draw logic gate level R/S Latch, D-Latch, D-Flip-Flop</summary>
  <p>
  
  <img src="../images/m04/001.png"><br>
  <img src="../images/m04/003.png">  
  
  </p>
</details>

<details>
  <summary>What is the clock period in nano second of a clock with 2.5GHz clock frequency?</summary>
  <p>
  
  $`(2.5\times{10}^9)^{-1} = \frac{1}{2.5\times{10}^9} = \frac{4}{10}\text{ns} = 0.4\text{ns}.`$
  
  </p>
</details>

<details>
  <summary>Explain the Memory Hierarchy</summary>
  <p>
  
  <img src="../images/m04/023.png">  
  
  </p>
</details>

<br>

## Making a Basic CPU

<details>
  <summary>Draw Von Neumann Model</summary>
  <p>
  
  <img src="../images/m05/011.png" width="600px">  
  
  </p>
</details>



<details>
  <summary>Explain the Von Neumann Loop</summary>
  <p>

- Von Neumann Loop)
  1. Fetch instruction at PC
  1. Decode instruction (Convert to Control Signals!)
  1. Execute instruction
  1. Update PC
  
  </p>
</details>



<details>
  <summary>Explain the Memory Hierarchy</summary>
  <p>
  
  <img src="../images/m04/023.png">  
  
  </p>
</details>

<details>
  <summary>What is the word length?</summary>
  <p>
  
  The size of the quantities normally processed by the ALU is often referred to 
  as the word length of the computer, and each element is referred to as a word.
  
  </p>
</details>

<br>

## ISA

<details>
  <summary> ISA vs Microarchitecture </summary>
  <p>
  
  - ISA
    - the specification of the interface between programs and hardwares
  - Microarchitecture
    - a more detailed implementation of the ISA
  
  </p>
</details>

<details>
  <summary> 3 components specified in ISA </summary>
  <p>
  
  1. The set of instructions
     1. the type of operation
     2. what data is needed
  2. Data types : acceptable representation for operands
  3. Addressing Modes : the mechanism that the computer can use to figure out where the operands are located at.
  
  </p>
</details>

<details>
  <summary> 2 components of an instruction </summary>
  <p>
  
  1. Opcode : what the instruction do
  2. Operand : the target of the instruction
  
  </p>
</details>

