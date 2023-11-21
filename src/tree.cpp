/*
    Name: Anthony Blakley
    Date: 11/18/2023
    Description: 
        Tree function declarations
*/

# include "headers/tree.h"

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
    delete root;
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

    switch (id) {
        case eof_tk:
            token_string = "eof: " + instance + " : " + line;
            break;
        case space_tk:
            token_string = "space: " + instance + " : " + line;
            break;
        case comment_tk:
            token_string = "comment: " + instance + " : " + line;
            break;
        case integer_tk:
            token_string = "integer: " + instance + " : " + line;
            break;
        case keyword_tk:
            token_string = "keyword: " + instance + " : " + line;
            break;
        case operator_tk:
            token_string = "operator: " + instance + " : " + line;
            break;
        case delimiter_tk:
            token_string = "delimiter: " + instance + " : " + line;
            break;
        case identifier_tk:
            token_string = "identifier: " + instance + " : " + line;
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
 * @param node : current node
 * ------------------------------------------
*/
void Tree::traverse(Node* node) {
    if (node == nullptr) {
        return;
    }

    // Print tokens of the current node
    std::cout << "Node: " << node->label << "\n";


    std::cout << "Root has: " << node->children.size() << "children\n";

    // // Traverse children in preorder
    // for (auto child : node->children) {
    //     traverse(child);
    //     return;
    // }
    
    // std::cout << "Nodes in tree: " << size << "\n";
}