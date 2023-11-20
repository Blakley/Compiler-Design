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
    else
        previous->children.push_back(newNode);

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
void Tree::new_token(Node* node, const std::string& token) {
    node->tokens.push_back(token);
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
void Tree::traverse(Node* node = nullptr) {
    if (node == nullptr)
        node = root;

    // perform traversal
    std::cout << "\nprint preorder traversal\n";
    std::cout << "Tree size: " << size << " nodes\n";
}
