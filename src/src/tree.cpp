/*
    Name: Anthony Blakley
    Date: 11/18/2023
    Description: 
        Tree function declarations
*/

# include "../headers/tree.h"

/**
 * ------------------------------------------
 *              Constructor
 * ------------------------------------------
*/
Tree::Tree() : root(nullptr), previous(nullptr) {
    // set tree size
    size = 0;
}


/**
 * ------------------------------------------
 *              Destructor
 * ------------------------------------------
*/
Tree::~Tree() {
    // remove all children of root
    if (root != nullptr)
        clean(root);
}


/**
 * ------------------------------------------
 *          Destructor Helper
 * 
 * @param node : current node
 * ------------------------------------------
*/
void Tree::clean(Node* node) {
    if (node == nullptr)
        return;

    // delete children
    for (auto child : node->children)
        clean(child);

    // delete the current node
    delete node;
}


/**
 * ------------------------------------------
 *  Creates a new node with the given label    
 * 
 *  @param label : the label for the new node
 * ------------------------------------------
*/
void Tree::new_node(const std::string& label) {
    Node* newNode = new Node(label);
    
    if (root == nullptr)
        root = newNode;

    previous = newNode;
    size ++;
}


/**
 * ------------------------------------------
 *              Add Child Node
 * 
 *  @param parent : the parent node
 *  @param child  : the child node to be added     
 * ------------------------------------------
*/
void Tree::new_child(Node* parent, Node* child) {
    parent->children.push_back(child);
}


/**
 * ------------------------------------------
 *          Adds a token to a node
 * 
 *  @param node  :  the node to which the token will be added
 *  @param token :  the token to be added.          
 * ------------------------------------------
*/
void Tree::new_token(Node* node, token* t) {
    // get the token values
    int id = t->id;
    std::string instance = t->instance;
    std::string line = std::to_string(t->line);
    
    // create the token string
    std::string token_string = "";

    // handle 
    switch (id) {
        case integer_tk:
            token_string = "integer: " + instance + " line: " + line;
            break;
        case identifier_tk:
            token_string = "identifier: \"" + instance + "\" line: " + line;
            break;
        default:
            // handle unknown token type
            break;
    }

    // add token to node
    node->tokens.push_back(token_string);
}


/**
 * ------------------------------------------
 *       Returns the root of the tree
 *     
 *  @return : the root node of the tree
 * ------------------------------------------
*/
Node* Tree::root_node() const {
    return root;
}

/**
 * ------------------------------------------
 *       Returns the previous of the tree
 *     
 *  @return : the previous node of the tree
 * ------------------------------------------
*/
Node* Tree::previous_node() const {
    return previous;
}


/**
 * ------------------------------------------
 *       Returns the amount of nodes 
 * 
 * @param : the size of the tree
 * ------------------------------------------
*/
size_t Tree::tree_size() const {
    return size;
}


/**
 * ------------------------------------------
 *         Preorder tree traversal
 * 
 * @param node        : current node
 * @param indentation : indentation amount
 * ------------------------------------------
*/
void Tree::traverse(Node* node, int indentation) {
    if (node == nullptr)
        return;

    // print header
    if (node == root) {
        std::string line(20, '=');
        std::cout << line << "\n";
        std::cout << " Preorder Traversal\n"; 
        std::cout << line << "\n";
    }

    // print identation
    for (int i = 0; i < indentation; ++i)
        std::cout << "    ";
    
    // print both the label and tokens of the current node on the same line
    std::cout << node->label << " ";
    for (const auto& token : node->tokens) {
        std::cout << token << " ";
    }
    std::cout << "\n";

    // traverse children in preorder with increased indentation
    for (auto child : node->children)
        traverse(child, indentation + 1);
}

    /*
            ===================
            Project 3 --> TODO:
            ===================

            Goal, implemenet static semantics: proper definition and use of variables
            
            * A variable has to be defined anywhere in the program to be used (must satisfy syntax), before or after use

                1. variable definition: any identifier listed in any <varList> is definition

                2. variable use: identifier showing up in any statement
                
                * rules:
                    no multiple variables with same name and a variable used must be defined

                    There can be two kinds of errors: 
                        1. multiple variables with the same name in the same scope 
                        2. undefined variable
        

            * Variable name can only be defined once in a scope but can be reused in another scope
    
            * There is only one scope, the global scope, regardless of where a variable is defined
    
            * In main.cpp, call tree.semantics(tree.root_node)
                It just does a tree traversal (like the traverse function) (might need to perform 2 traversals)
                
            * implementation option:
                - have a file: semantics.cpp, where the Semantics object takes in the root node of the tree 
                - create necessary semantic functions in semantics.cpp
                - create a semantics.h file
                - in main, call semantics.static_semantics();

    */