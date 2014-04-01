Rainbow 
========

The aim of this project is to build a compiler for the Rainbow language and produce correct x86 Linux assembly code for it.

###Algorithms and techological side.

There are several phases of compilation such as:

1. Lexical analysis.
2. Syntax analysis.
3. Semantic analysis.
4. Code generation.

On the first stage some kind of finite automaton is used. Lexical analysis is needed to divide the text of a program into tokens - minimal unit of syntax.

Next stage is parsing. On this stage the backtracking *LL(k)* parser is the main idea.
It is used to make sure that program is syntactically correct and transform it to the internal representation.
This representation has form of abstract syntax tree (AST). 

On the next stage of compilation semantic analysis is performed. We take the several in-order traversals through the AST to make sure that all needed symbols are defined, all function calls can be performed, and to check the types.

The last stage is code generation. Again this is performed by traversal through the tree. The output of this phase is the final assembly code, that can be turned into object code with **_nasm_** and then linked with **_ld_**, for example.

###User guide

All you need is to compile the source into the executable. For this you need **_make_** and **_clang_** that supports the _C++11_.

Then you need to execute in shell such command as
```bash
$ [executable_name] [program_name]
```

The language is quite simple, there are several examples.

* Function definition

```
def f(int x, int y) : int {
  return x + y * 2;
}
```

* Struct definition

```
struct A {
  def A(int _x) {
    x = _x;
  }
  
  def operator()() : int {
    return x + x;
  }
  
  int x;
}
```
* Templates

```
template <class T>
struct A {
  T x;
  
  def A(T _x) { x = _x; }
  
  def operator()(int k) {
    x(k);
  }
}
```

Unfortunately, template functions like in _C++_ are not supported yet.


Well, you can see that it's some sort of mixed _C++_ and _Python_, where all _C_ compatibility is cut off. As the time goes, new syntax constructs will appear to reach the _Python_ simplicity and _C++_ power.


###Developer Guide
  
There are several main classes that do most of the work.
* Lexer (divides input in tokens)
* Parser (does all the parsing)
* AST (base class for all AST nodes)
* ExprNode (base class for all expressions and child of AST)
* CallHelper (provides functions to resolve viable overload and get _CallInfo_)
* CodeGen (provides functions for code generation)

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

Inspired by:
  C++ and Python
