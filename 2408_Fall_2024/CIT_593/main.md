[Back to Main](../../README.md)

# CIT 593 Introduction to Computer Systems

|Module|Subject|Keywords|
|:-:|:-|:-|
| 1-2|[Bits and Bytes](notes/m01.md)|- Binary, Bits, Bytes, Nibbles, Overflow <br> - 1's Complement, 2's Complement <br> - Sign Extension <br> - Encoding : ASCII and Unicode <br> - Hexadecimal Notation <br> - Fixed-Point, Floating-Point (IEEE 754)|
| 3|[Transistors and Combinational Logic](notes/m03.md)|- Transistor, MOSFET: nMOS, pMOS, cMOS <br> - Logic Gates : Inverter, NAND, NOR, Multiple N-Input Gates, Gate Delay, Truth Tables & Boolean(Logic) Function <br> - PLA <br> - Incrementer : One-Bit Incrementer, N-Bit Incrementer <br> - Adder : Half-Adder, One-Bit Adder (Full-Adder, Ripple Adder) <br> - Subtractor <br> - Multiplexer (MUX) : Control Signal, Adder/Subtractor <br> - Multiplier : Shift and Add Architecture|
| 4|[Sequential Logic to Memory](notes/m04.md)|- Combinational Logic vs Sequential Logic <br>- Basic Storage Elements : R-S Latch, D-Latch, D-Flip-Flop, Clock, Register <br> - Memory Circuit : Single/Multi Port Memory, Address Space, Addressability <br> - Sequential Logic Circuit <br> - Finite State Machine : Counter, State Diagram <br> - SRAM, DRAM, Memory Hierarchy|
| 5|[Making a Basic CPU](notes/m05.md)|- ALU <br> - Register File <br> - Control Signals <br> - Program Counter|
| 6|[ISA - HW/SW Bridge](notes/m06.md)|- LC-4 (Little Computer version 4) <br> - Instruction Set Architecture (ISA) <br> - Processor Status-Word Register (PSR)|
| 7|[Programming with LC-4 Assembly Language](notes/m07.md)|- Machine Language, Assembly, Assembler <br> - Subroutine : `.FALIGN` <br> - Data Memory, Program Memory <br> - Directives : `.DATA`, `.ADDR`, `.FILL IMM16`, `.CODE` |
| 8|[IO and OS](notes/m08.md)|- IO Controller (Status Register, Data Register) <br> - Memory-Mapped I/O : Read, Write, Timer, Video <br> - Directives : `.UNCONST`, `.LC` <br> - `TRAP`/`RTI` : TRAP Vector Table |
| 9|[Introduction to C & The Stack](notes/m09.md)|- C : Stack, Stack Frame, Frame Pointer <br> - Compile : Symbol Table|
|10|[C Variables, Pointers, and Arrays](notes/m10.md)|- Data types : `char`, `int`, `float`, `double` <br> - Data Type Modifier : `long`, `short`, `unsigned` <br> - Type Casting <br> - User Defined Type : `struct`, `typedef` <br> - Pointer: Dereferencing, Pointer Arithmetics <br> - Null Pointer, Segmentation Fault <br> - `void`, Void Pointer <br> - Constants : `const`, `#define`<br> - Storage Classes: Automatic, Static, Global Variable, Heap|
|11|[Strings, Character Arrays, and Points to Strings](notes/m11.md)|- String vs Literal (Pointer to a String) <br> - `strlen` of `string.h` <br> - Arrays of Strings|



- Exam Prep
  - [Mid Term](exam_prep/midterm.md)


[Back to Main](../../README.md)