/*
    Name: Anthony Blakley
    Date: 09/28/2023
    Description:
        Header, contains the definition for the binary search tree object
*/


# pragma once
# include "node.h"
# include <string>
# include <vector>

// BST object
class BST {
    private:
        node_t* root; // pointer to the root node of the BST
        size_t size;  // number of nodes in BST
        
        // inserts nodes
        void insert(node_t*& node, int value);  
        
        // creates output file
        void output(const std::vector<std::string>& data, const std::string name);

    public:
        BST();   // constructor
        ~BST();  // destructor 

        void printInorder();   // prints the BST using inorder method
        void printPreorder();  // prints the BST using preorder method
        void printPostorder(); // prints the BST using postorder method

        void buildTree(node_t*& root, const std::string file); // builds the tree
};
