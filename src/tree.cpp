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
            token_string = "eof: " + instance + " line: " + line;
            break;
        case space_tk:
            token_string = "space: " + instance + " line: " + line;
            break;
        case comment_tk:
            token_string = "comment: " + instance + " line: " + line;
            break;
        case integer_tk:
            token_string = "integer: " + instance + " line: " + line;
            break;
        case keyword_tk:
            token_string = "keyword: " + instance + " line: " + line;
            break;
        case operator_tk:
            token_string = "operator: " + instance + " line: " + line;
            break;
        case delimiter_tk:
            token_string = "delimiter: " + instance + " line: " + line;
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

    // print tokens of the current node with indentation
    for (int i = 0; i < indentation; ++i)
        std::cout << "    ";
    
    // Print both the label and tokens of the current node on the same line
    std::cout << node->label << " ";
    for (const auto& token : node->tokens) {
        std::cout << token << " ";
    }
    std::cout << "\n";

    // Traverse children in preorder with increased indentation
    for (auto child : node->children)
        traverse(child, indentation + 1);
}
