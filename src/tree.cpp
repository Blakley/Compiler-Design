/*
    Name: Anthony Blakley
    Date: 09/28/2023
    Description:
        Sets up functions for creating, displaying,
        and outputting the binary search tree
*/

// Imports
# include "tree.h"
# include <iostream>
# include <fstream>


/**
 * -------------------------------------
 * 
 *  BST constructor:
 *  Initalizes a new binary search tree
 * 
 * -------------------------------------
*/
BST::BST() {
    // initialize root and tree size
    size = 0; 
    root = nullptr; 
}


/**
 * -------------------------------------
 *  
 *  BST destructor:
 *  Cleans up the binary search tree
 * 
 * -------------------------------------
*/
BST::~BST() {
    // todo
}


/**
 * -------------------------------------
 * 
 * Inserts a new node with a given value
 * into the tree
 * 
 * @param node  : reference to new node
 * @param value : node value
 * -------------------------------------
*/
void BST::insert(node_t*& node, int value) {
    // Implement the insertion logic here.
}


/**
 * -------------------------------------
 * 
 *  Builds the binary search tree using
 *  the data from the input file
 * 
 * @param root  : reference to root node
 * @param file  : name of input file
 * 
 * -------------------------------------
*/
void BST::buildTree(node_t*& root, const std::string file) {
    /*
        The program will read the data left to right and put them into a tree, which is a binary search tree (BST). 
        The data is all strings made up of letters and digits only (you may assume only good data, +10 for validaon and error). 
        The BST is constructed with respect length of a string - strings of the same length fall into the same node, those shorter end up in the left subtree, etc. 
        Tree is never balanced nor restructured other than growing new nodes as needed. 
        A node should know the digit it collects the number for, and all the numbers falling into the node except that duplicates are not retained.
    */

    // call isnert method for inserting new nodes.
}


/**
 * -------------------------------------
 * 
 *  Prints the Binary Search Tree in 
 *  inorder traversal, outputs the result 
 *  to the console and a file
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
 *  Prints the Binary Search Tree in 
 *  preorder traversal, outputs the result 
 *  to the console and a file
 * 
 * -------------------------------------
*/
void BST::printPreorder() {
    // Implement the method to print the BST in preorder traversal.
    // Outputs a file containing the output as well


    /*
        printing format:
        a node will print starting with indentaon of 2x depth (root is at depth 0) then the node's length (length of the strings) 
        followed by character ':' and space followed by the list of numbers from the node set separated by spaces (a set, no repetitions and no specific order)    
    */
}


/**
 * -------------------------------------
 * 
 *  Prints the Binary Search Tree in 
 *  postorder traversal, outputs the result 
 *  to the console and a file
 * 
 * -------------------------------------
*/
void BST::printPostorder() {
    // Implement the method to print the BST in postorder traversal.
    // Outputs a file containing the output as well
}



/**
 * -------------------------------------
 * 
 *  Writes the bst to an output file
 *  
 *  @param data : a vector of strings containing tree data
 *  @param name : the name of the output file to be created
 * -------------------------------------
*/
void BST::output(const std::vector<std::string>& data, const std::string name) {
    // create the output file
    std::ofstream new_file(name);

    // check if file opens
    if (new_file.is_open()) {
        // write tree data to file
        for (auto s: data) {
            new_file << s << std::endl;
        }
        
        // close file
        new_file.close();
    }
    else {
        // error opening file
        std::cerr << "Error: unable to create output file: " << name << std::endl;
    }

    /*
    Output:
        If the file name is given, then the output file name should be of the form:
            file_name.preorder 
            file_name.postorder 
            file_name.inorder

        If the file name is not given, then the output file name should be of the form:
            out.preorder 
            out.postorder 
            out.inorder
    */
}