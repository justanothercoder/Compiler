Rainbow 
========

The aim of this project is to build a compiler for the Rainbow language and produce correct x86 Linux assembly code for it.

###Algorithms and techological side.

There are several phases of compilation such as:

1. Lexical analysis.
2. Syntax analysis and IR generation.
3. Semantic analysis.
4. TAC generation. 
5. Optimization.
6. Assembly generation.

On the first stage some kind of finite automaton is used. Lexical analysis is needed to divide the text of a program into tokens - minimal units of syntax.

Next stage is parsing. On this stage the backtracking *LL(k)* parser is the main idea.
It is used to make sure that program is syntactically correct and transform it to the internal representation.
This representation has form of abstract syntax tree (AST). 

On the next stage of compilation semantic analysis is performed. We take the several in-order traversals through the AST to make sure that all needed symbols are defined, all function calls can be performed, and to check the types.

The next stage is Three-Address Code (TAC) generation which is primarily used to perform optimization. Optimization currently consists of constant propagation and elimination of unused variables and temporaries.

The last stage is code generation. This is performed by mapping TAC instructions to assembly instructions. The output of this phase is the final assembly code, that can be turned into object code with **_nasm_** and then linked with **_ld_**, for example.

###User guide

All you need is to compile the source into the executable. For this you need **_make_** and **_clang_** that supports the _C++14_.

Then you need to execute in shell such command as
```bash
$ [executable_name] [program_name]
```

The language is quite simple, there are several examples.

* Comments

```
// This is one-line commnet

/*
  This is multi-line comment
*/
```

* Function definition

```
def f(int x, int y) : int {
  //code
}

template <class T, class U>
def f(U x) : T {
  //code
}
```

* Struct definition

```
struct A {
  //functions and variables and structures
}

template <class T, class U> // template metaprogramming like in C++
struct B {
  //functions and variables and structures
}
```

* Control structures

```
if ( cond ) {
  //code if cond
}
else { //optional
  //code if not cond
}

while ( cond ) { 
  //code
}

for ( init; cond; step ) { //all three are optional, but here semicolons are mandatory
  //code;
}
```

* Import

```
import libname
from libname import something
```

* New

```

//Like in Java and C#

struct A 
{
  def A() {
  
  }
}

new A
new A() 

```

##Standart Library

1. _ASCII_ strings
2. Arrays
3. Heap allocation
4. Buffered IO

More containers are in plans.
As for language core, this will soon be ready: first-class functions, lambda expressions, improved import system, JIT compilation.


###Developer Guide
  
There are several main classes that do most of the work.
* Lexer (divides input in tokens)
* Parser (does all the parsing)
* AST (base class for all AST nodes)
* ExprNode (base class for all expressions and child of AST)
* ExpandTemplatesVisitor (expands templates)
* DefineVisitor (defines symbols)
* CheckVisitor (perform semantic analysis)
* GenSSAVisitor (generates TAC)
* ThreeAddressCode (code)
* Optimizer (performs optimization)

If you want to add something new you should change one of this classes and possibly add a new node as a child of AST class.

You can also expand the standart library of the Rainbow, adding new classes and functions to stdlib/. 
However, there can be very platform-specific things, that need to be coded in assembly language. Then you should write it and recompile the library.


###References

Books:

* Language Implementation Patterns, Terence Parr
* Let's build a compiler, Jack Crensaw
* Basics of compiler design, Torben Mogensen
* Compilers: Principles, Techniques, and Tools, Alfred Aho, Jeffrey Ullman 

Reference:

* http://en.cppreference.com/

Inspired by C++, D and Python
