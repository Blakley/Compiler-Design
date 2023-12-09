/*
    Name: Anthony Blakley
    Date: 12/03/2023
    Description: 
        
*/

# include "../headers/scanner.h"
# include "../headers/parser.h"
# include "../headers/semantics.h"
# include "../headers/generator.h"

/**
 * ------------------------------------------
 *              Entry function
 * ------------------------------------------
*/
int main(int argc, char** argv) {
    // get scanner object
    Scanner scanner;

    // handle input arguments
    scanner.arguments(argc, argv);

    // get tree object
    Tree tree;

    // get parser object
    Parser parser(scanner, tree);

    // begin program top-down parsing
    parser.begin();

    // [tree test] output generated tree
    tree.traverse(tree.root_node(), 0);
    std::cout << "\n";

    // get semantic object
    Semantics semantics(tree.root_node());

    // check program semantics
    semantics.static_semantics();
    
    // get generator object
    Generator generator(tree.root_node());

    // output generated .asm code to a file
    generator.output(scanner.filename);

    return 0;
}


/*

<exp> parsing


====================
 Preorder Traversal
====================
<program> 
    <vars> 
        <varList> identifier: "x1" line: 1 integer: 1 line: 1 
            <varList> identifier: "x2" line: 2 integer: 2 line: 2 
                <varList> identifier: "x3" line: 3 integer: 3 line: 3 
    <stats> 
        <stat> 
            <out> 
                <exp> 
                    <M> 
                        <N> 
                            <R> integer: 1 line: 5 
        <mStat> 


For entire program:
    a) store the operators, as they should be executed, from the <exp> node's token
    b) get a list of the values or identifers stored in <R>
    c) create necessary assembly to execute code
    d) make sure to remove the operators from the list that have already been consumed 
    (do same with relational operators)

*/