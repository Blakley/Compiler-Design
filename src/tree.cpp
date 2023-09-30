/*
    Name: Anthony Blakley
    Date: 09/29/2023
    Description:
        Sets up functions for creating, displaying,
        and outputting the binary search tree
*/

// Imports
# include "tree.h"
# include <fstream>
# include <sstream>
# include <iostream>


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
BST::BST(std::string filename) : root(nullptr), size(0), filename(filename) {
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
 *  The strings should contain letters, digits, 
 *  and WS seperators only
 * 
 * -------------------------------------
*/
void BST::validator() {

    std::cout << "validating input data" << std::endl;

    // create error messages
    std::string error_line = "[Error], Input file (line ";
    std::string error_empty = "[Error], Input file is empty: ";
    std::string error_file = "[Error], Unable to open input file: ";

    // open file
    std::ifstream input_file(filename, std::ios::in);
    
    // check if there is an error with the file
    if (!input_file.is_open()) {
        std::cerr << error_file << filename << std::endl;
        exit(EXIT_FAILURE);
    }

    // check if the file is empty
    if (input_file >> std::ws && input_file.eof()) {
        std::cerr << "Error: Input file is empty." << std::endl;
        exit(EXIT_FAILURE);
    }

    // file content variables
    std::string line;
    int index = 0;

    // check file contents
    while (std::getline(input_file, line)) {
        ++index;

        // split the line into words
        std::istringstream lines(line);
        std::string word;

        // check that each string contains only letters, digits, and spaces
        while (lines >> word) {
            
            bool valid = true; 
            for (char c : word) {
                if (!std::isalnum(c) && !std::isspace(c)) {
                    valid = false; 
                    break;
                }
            }

            // file contains invalid characters
            if (!valid) {
                std::cerr << error_line << index << "): Invalid string - " << word 
                        << " (contains unsupported characters)" << std::endl;
                exit(EXIT_FAILURE);
            }
        }
    }

    input_file.close();
}


/**
 * -------------------------------------
 * 
 * Inserts nodes into the tree with a given value
 * 
 * @param node   : reference to new node
 * @param value  : node value
 * -------------------------------------
*/
void BST::insert(node_t*& node, std::string value) {
    std::cout << "insert a value into tree" << std::endl;

    // check current node
    if (node == nullptr) {
        // create a new node
        node = new node_t;
        node->data.insert(value);
        node->left = nullptr;
        node->right = nullptr;
    }
    else {
        // traverse left or right based on string length
        int length = value.length();
        int node_length = node->data.begin()->length();

        if (length == node_length) 
            node->data.insert(value);   // add value to current node
        else if (length < node_length)
            insert(node->left, value);  // traverse left subtree
        else
            insert(node->right, value); // traverse right subtree
    }
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

    // Open the input file
    std::ifstream input_file(filename);
    
    // Check if there is an error with the file
    if (!input_file.is_open()) {
        std::cerr << "[Error] Unable to open input file: " << filename << std::endl;
        exit(EXIT_FAILURE);
    }

    // file content variables
    std::string line;

    // create nodes from file content
    while (std::getline(input_file, line)) {

        // extract words from each line
        std::istringstream lines(line);
        std::string word;

        // process each word
        while (lines >> word) {
            // check if word is root
            if (root == nullptr) {
                // set root node
                root = new node_t;
                root->data.insert(word);
                root->left = nullptr;
                root->right = nullptr;
            }
            else
                insert(root, word);
        }
    }

    // close file
    input_file.close();
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

    // create output file
    output(result, "preorder");
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
   
   
    /*
        printing format:
        a node will print starting with indentaon of 2x depth (root is at depth 0) then the node's length (length of the strings) 
        followed by character ':' and space followed by the list of numbers from the node set separated by spaces (a set, no repetitions and no specific order)    
    */

   
   
    // output file name
    std::string output = "";
    std::string error_file = "[Error], Unable to create output file";

    // determine file name
    if (filename != "tempfile") {
        // extract base name of file
        if (filename.size() >= 4 && filename.substr(filename.size() - 4) == ".f23")
            output = filename.substr(0, filename.size() - 4); 
        else 
            output = filename;

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