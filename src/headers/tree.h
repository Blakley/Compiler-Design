/*
    Name: Anthony Blakley
    Date: 11/18/2023
    Description: 
        Tree header file for parse tree output
*/

# ifndef TREE_H
# define TREE_H

# include <iostream>
# include <string>
# include <vector>

struct Node {
    std::string label;                // label for the node, same as nonterminal function name
    std::vector<std::string> tokens;  // store relevant tokens processed in the function
    std::vector<Node*> children;      // children for each nonterminal in a production

    Node(const std::string& l) : label(l) {}

    ~Node() {
        for (auto child : children) {
            delete child;
        }
    }
};

class Tree {
    private:
        Node* root;     // root node
        Node* previous; // keep track of the previous node
        size_t size;    // number of nodes in tree

    public:
        Tree();
        ~Tree();

        void new_node(const std::string& label);              // create a new node
        void new_child(Node* parent, Node* child);            // add a child to a node
        void new_token(Node* node, const std::string& token); // add a token to a node
        void traverse(Node* node);                            // inorder traversal & printing function

        Node* root_node() const;     // getter for root node
        Node* previous_node() const; // getter for previous node
        size_t tree_size() const;    // getter for tree size
};

# endif // TREE_H
