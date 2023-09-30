/*
    Name: Anthony Blakley
    Date: 09/29/2023
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
        node_t* root;         // pointer to the root node of the BST
        size_t size;          // number of nodes in BST
        std::string filename; // name of input file

        // inserts nodes
        void insert(node_t*& node, std::string value);  
        
        // creates output file
        void output(const std::vector<std::string>& data, std::string traversal);

    public:
        BST(const std::string filename);   // constructor
        ~BST();                            // destructor 

        void printInorder();   // prints the BST using inorder method
        void printPreorder();  // prints the BST using preorder method
        void printPostorder(); // prints the BST using postorder method

        void buildTree();      // builds the tree
        void validator();      // validates input file data
};
