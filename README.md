This project implements a top-down parser for a specific language, 
generating an Abstract Syntax Tree (AST) from the input source code. 
Additionally, it performs static semantics checks on the AST, ensuring 
proper variable definition and use according to predefined rules. 
The project consists of a Scanner, Parser, Tree, and Semantics modules, 
working together to analyze and validate the input code.

## Features

- **Token Handling:** The parser recognizes and handles different token types, such as identifiers and integers.

- **Non-Terminal Parsing:** The code includes functions for parsing non-terminals, creating corresponding nodes in the syntax tree.

- **Tree Traversal:** The syntax tree is traversed in a pre-order fashion, facilitating a structured visualization of the parsed code.

- **Static Semantics Check:** Static semantics checks to ensure proper variable definition and use within the code. It performs checks for multiple definitions of variables within the same scope, as well as verifies the usage of variables to avoid undefined references.
