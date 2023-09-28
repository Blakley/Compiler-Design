/*
    Name: Anthony Blakley
    Date: 09/28/2023
    Description:
        
*/

// tree.h
# pragma once

# include "node.h"
# include <cstddef>

//
class BST {
    private:
        node_t* root; // Pointer to the root node of the BST.
        size_t size;  // keep track of number of nodes in BST.

        void insert(node_t*& node, int value);  // Helper method for buildTree

    public:
        BST();  // Constructor (if needed)
        ~BST(); // Destructor (if needed)

        void printInorder();   // Define this method.
        void printPreorder();  // Define this method.
        void printPostorder(); // Define this method.

        void buildTree(node_t*& root, const char* filename); // Define this method.
};
