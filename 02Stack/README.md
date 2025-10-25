# L2: Stack & Recursion

## Stack

### Concept

- A kind of **linear structure**
- Insertion and deletion are only allowed to conducted from one end
- Follow **LIFO** rules
![show push and pop operation](./pic/2p1.png)

>**LIFO: Last In First Out
FILO: First in Last out**

### Implementation

- [x] [Sequential Stack](./SqStack.cpp)
- Time complexity for `push` and `pop`: $O(1)$
  ![pop](./pic/2p2.png)
  ![push](./pic/2p3.png)

- Fixed size at initialization: ***Stack Overflow!***
- The **space utilization efficiency** is **low**
- [x] [Linked Stack](./LinkedStack.cpp)
- Time complexity for `push` and `pop`: $O(1)$
  ![push](./pic/2p4.png)
  ![pop](./pic/2p5.png)
- Generally ***no stack overflow***
- The space utilization efficiency is **high**

- [x] Comparison
  ![comp. table](./pic/2p6.png)

### Application

#### Brackets balance check

- 2 rules: **matched** and in **correct order**
- Steps:
  - If is an opening bracket, `{`, `[` or `(`, `push` to stack
  - If is a closed bracket, `}`, `]` or `)`, `pop` from stack
  ![simple process](./pic/2p7.png)

#### Base conversion

- $K=\lfloor N\div d \rfloor\times d+N \mod d$
![example radix-8](./pic/2p8.png)

#### Expression evaluation

Evaluate Postfix expression by stack:

- Scan the postfix expression from left to right
- If is a number, `push`
- If is an operator, `pop` **two numbers and calculate by the operator,
push the result into stack**
- The last remaining element in the stack is the result

## Recursion
