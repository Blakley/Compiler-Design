/*
    Name: Anthony Blakley
    Date: 09/30/2023
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
        node_t* root;                       // pointer to the root node of the BST
        size_t size;                        // number of nodes in BST
        std::string filename;               // name of input file
        std::vector<std::string> printer;   // holds the content needed to print 
        
        void insert(node_t*& node, std::string value);   // inserts nodes
        void output(std::string traversal);              // creates output file

    public:
        BST(const std::string filename);   // constructor
        ~BST();                            // destructor 

        // prints/creates the BST file using inorder method
        void printInorder();
        void printInorder(node_t* node, int level);
        
        // prints/creates the BST file using preorder method
        void printPreorder();  
        void printPreorder(node_t* node, int level);

        // prints/creates the BST file using postorder method
        void printPostorder(); 
        void printPostorder(node_t* node, int level); 

        void buildTree();      // builds the tree
        void validator();      // validates input file data
};
