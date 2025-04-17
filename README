# SYNTAXIA - A C++ Compiler

Welcome to **Syntaxia**, a compiler designed to interpret and compile a subset of the C++ programming language into a RISC-V based assembly format inspired by MIPS R2000.
---

## Team Members

This project was developed collaboratively by:

- Yateen  
- Kashish  
- Yash  
- Sunny  
- Nidhish  
- Krishna  

---

## Supported Features

**Syntaxia** supports:

- **Expression Evaluation**
- **Scope Management** including:
  - Function Declarations  
  - Local Variables  
  - Argument Support  
  - Control Flow Statements: `if`, `while`, `goto`, etc.
- **Semantic and Logical Validation**
- **Code Generation** to assembly format

---

## Register Usage

Syntaxia translates your C code using the following register conventions:

- **Local Variables**:  
  `{ x9, x18, x19, x20, x21, x22, x23, x24, x25, x26, x27 }`

- **Arithmetic Instructions**:  
  `{ add, sub, div, mod, mul, or, xor, and, sll, srl, not }`

- **Branch Instructions**:  
  `{ ld, bne, ble, blt, bge, bgt, beq }`

- **Function Arguments**:  
  `{ x10, x11, x12, x13, x14, x15, x16, x17 }`

- **Global Variables**:  
  `{ g8, g9, g18, g19, g20, g21, g22, g23, g24, g25, g26, g27 }`

> The final compiled output is a RISC-V assembly language file that closely follows MIPS R2000 conventions.

---

## Code Structure

| File                | Responsibility                       |
|---------------------|--------------------------------------|
| `scanner.cpp`, `scanner.h` | Lexical Analysis – All tokens are stored in `tokens.txt`. |
| `parser.cpp`, `parser.h`   | Syntax Analysis and Semantic Helpers |
| `semantics.cpp`, `semantics.h` | Semantic Evaluation, Scope Checking, and **Code Generation** into `compile.txt`. |

---

## Frontend UI

Syntaxia includes a user-friendly frontend to interact with the compiler.

### Launch Instructions

1. Open your terminal and navigate to the project directory.
2. Run the following command:
   ```bash
   python server.py
3. The web UI will launch. Enter your C code into the provided editor.
4. Click the 'Compile' button to generate the assembly output.
