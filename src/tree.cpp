/*
    Name: Anthony Blakley
    Date: 09/28/2023
    Description:
        
*/

// Imports
# include "tree.h"
# include <iostream>
# include <fstream>


/**
 * -------------------------------------
 * 
 * 
 * 
 * -------------------------------------
*/
BST::BST() {
    root = nullptr; // Initialize the root pointer in the constructor.
    size = 0; // Initialize size to 0.
}


/**
 * -------------------------------------
 * 
 * 
 * 
 * -------------------------------------
*/
BST::~BST() {
    // Define the destructor if needed.
}


/**
 * -------------------------------------
 * 
 * 
 * 
 * -------------------------------------
*/
void BST::insert(node_t*& node, int value) {
    // Implement the insertion logic here.
}


/**
 * -------------------------------------
 * 
 * 
 * 
 * -------------------------------------
*/
void BST::buildTree(node_t*& root, const char* filename) {
    // Implement the logic to build the tree from a file.
    // You may use the insert method for inserting nodes.
}


/**
 * -------------------------------------
 * 
 * 
 * 
 * -------------------------------------
*/
void BST::printInorder() {
    // Implement the method to print the BST in inorder traversal.
    // Outputs a file containing the output as well
}


/**
 * -------------------------------------
 * 
 * 
 * 
 * -------------------------------------
*/
void BST::printPreorder() {
    // Implement the method to print the BST in preorder traversal.
    // Outputs a file containing the output as well
}


/**
 * -------------------------------------
 * 
 * 
 * 
 * -------------------------------------
*/
void BST::printPostorder() {
    // Implement the method to print the BST in postorder traversal.
    // Outputs a file containing the output as well
}




/*
                                                    Todo:

    1. Build the tree (tree.cpp)
         functions for #2 and #3 should be in separate source(s) . Any node types should be defined in a separate header file (node.h)


    2. Traverse the tree three different ways generating outputs (tree.cpp)
         functions for #2 and #3 should be in separate source(s) . Any node types should be defined in a separate header file (node.h)

        - printing format:
            a node will print starng with indentaon of 2x depth (root is at depth 0) then the node's length (length of the strings) 
            followed by character ':' and space followed by the list of numbers from the node set separated by spaces (a set, no repeons and no specific order)


    The program will read the data le to right and put them into a tree, which is a binary search tree (BST). 
    The data is all strings made up of leers and digits only (you may assume only good data, +10 for validaon and error). 
    The BST is constructed with respect length of a string - strings of the same length fall into the same node, those shorter end up in the le subtree, etc. 
    Tree is never balanced nor restructured other than growing new nodes as needed. A node should know the digit it collects th
*/

