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
 *  Initalizes a new binary search tree,
 *  calls data validation function
 * 
 * @param filename  : programs input file
 * -------------------------------------
*/
BST::BST(std::string filename) : root(nullptr), size(0), input_file(filename) {
    // validate input data
    validator();
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
    std::cout << "deleting tree" << std::endl;

    // delete nodes of bst
    while (root) {
        // delete children
        if (root->left) 
            delete root->left;
        
        if (root->right) 
            delete root->right;
        
        // delete current node
        delete root;
        root = nullptr;
    }
}


/**
 * -------------------------------------
 * 
 *  Validates input file data:
 *  Data should be all strings (printable ASCII)
 *  separated with standard WS separators (space, tab, new line)
 *  The strings should contain letters and digits only
 * 
 * -------------------------------------
*/
void BST::validator() {

    std::cout << "validating input data" << std::endl;

    // to check: save file contents to array, iterate array and check
    // all characters to ensure all data in file is valid (error if failed)
    
    // valid example:
    /*
        4aaa susan adam 4aaa
        susan an ann anna
        george x abs ha hahah
    */

    // invalid example:
    /*
        x <- c(“Ekstr\u00f8m”, “J\u00f6reskog”, 
        “bi\u00dfchen Z\u00fcrcher”)x#> [1] 
        “Ekstrøm” “Jöreskog”
        bißchen Zürcher
    */
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
 * -------------------------------------
*/
void BST::buildTree() {
    std::cout << "building tree" << std::endl;

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
 *  Prints the Binary Search Tree in inorder traversal,
 *  calls the outputs function to save it to a file
 * 
 * -------------------------------------
*/
void BST::printInorder() {
    std::vector<std::string> result;

    std::cout << "printing inorder" << std::endl;

    // Implement the method to print the BST in inorder traversal.
    // Outputs a file containing the output as well

    // create output file
    output(result, "inorder");
}


/**
 * -------------------------------------
 * 
 *  Prints the Binary Search Tree in preorder traversal,
 *  calls the outputs function to save it to a file
 * 
 * -------------------------------------
*/
void BST::printPreorder() {
    std::vector<std::string> result;

    std::cout << "printing preorder" << std::endl;

    // Implement the method to print the BST in preorder traversal.
    // Outputs a file containing the output as well


    /*
        printing format:
        a node will print starting with indentaon of 2x depth (root is at depth 0) then the node's length (length of the strings) 
        followed by character ':' and space followed by the list of numbers from the node set separated by spaces (a set, no repetitions and no specific order)    
    */

    // create output file
    output(result, "preorder");
}


/**
 * -------------------------------------
 * 
 *  Prints the Binary Search Tree in postorder traversal, 
 *  calls the outputs function to save it to a file
 * 
 * -------------------------------------
*/
void BST::printPostorder() {
    std::vector<std::string> result;

    std::cout << "printing postorder" << std::endl;

    // Implement the method to print the BST in postorder traversal.
    // Outputs a file containing the output as well

    // create output file
    output(result, "postorder");
}


/**
 * -------------------------------------
 * 
 *  Writes the bst traversed elements
 *  to an output file
 *  
 *  @param data      : a vector of strings containing tree data
 *  @param traversal : traversal method for output file
 * -------------------------------------
*/
void BST::output(const std::vector<std::string>& data, std::string traversal) {
    // output file name
    std::string output = "";
    std::string error_file = "[Error], Unable to create output file";

    // determine file name
    if (input_file != "tempfile") {
        // extract base name of file
        if (input_file.size() >= 4 && input_file.substr(input_file.size() - 4) == ".f23")
            output = input_file.substr(0, input_file.size() - 4); 
        else 
            output = input_file;

        output += "." + traversal;
    }
    else {
        // use:  out."traversal" file name
        output = "out." + traversal;
    }

    std::cout << "Creating output file: " << output << std::endl;

    // create file
    std::fstream output_file(output, std::ios::out);

        // error opening created file
    if (!output_file.is_open()) { 
        std::cerr << error_file << output << std::endl;
        exit(EXIT_FAILURE);
    }    

    // write tree data to file
    for (auto s: data) {
        output_file << s << std::endl;
    }

    // close file
    output_file.close();
}