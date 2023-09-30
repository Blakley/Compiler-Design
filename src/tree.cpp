/*
    Name: Anthony Blakley
    Date: 09/30/2023
    Description:
        Sets up functions for creating, displaying,
        and outputting the binary search tree
*/

// Imports
# include "tree.h"
# include <map>
# include <stack>
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

    // Open the input file
    std::ifstream input_file(filename);
    
    // Check if there is an error with the file
    if (!input_file.is_open()) {
        std::cerr << "[Error] Unable to open input file: " << filename << std::endl;
        exit(EXIT_FAILURE);
    }

    // file content variables
    std::string line;

    // vector to store unique-length words & and all words
    std::vector<std::string> unique_length_words;
    std::vector<std::string> other_words;

     // 1.) add initial strings to array
    while (std::getline(input_file, line)) {

        // extract words from each line
        std::istringstream lines(line);
        std::string word;

        // read words from input file
        while (lines >> word) {
    
            // check if the length of the word is unique in the vector
            int length = word.length();
            bool is_unique = true;
            for (const std::string& unique_word : unique_length_words) {
                if ((int)unique_word.length() == length) {
                    is_unique = false;
                    break;
                }
            }

            // add unique-length words to first vector, and duplicate-length to second vector.
            if (is_unique) 
                unique_length_words.push_back(word);
            else
                other_words.push_back(word); 
        }
    }

    // 2.) create initial nodes from unique_length_words
    for (const std::string& word : unique_length_words)
        insert(root, word);

    // 3.) add remaining words, from other_words, to the created nodes
    for (const std::string& word : other_words)
        insert(root, word);

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
    printer.clear(); 
    printPreorder(root, 0);
    output("preorder");
}


/**
 * -------------------------------------
 *  Preorder print helper function
 * 
 *  @param node   : reference to input node
 *  @param level  : node depth/level
 * -------------------------------------
*/
void BST::printPreorder(node_t* node, int level) {
    if (node == nullptr)
        return;

    // map to group strings by length
    std::map<int, std::vector<std::string>> grouped_data;

    // iterate through the node's data and group by string length
    for (const std::string& value : node->data)
        grouped_data[value.length()].push_back(value);

    // print grouped node data
    for (const auto& entry : grouped_data) {
        const int length = entry.first;
        const std::vector<std::string>& strings = entry.second;

        // calculate asterisk amount
        int asterisk = 0;
        asterisk = level * 2;

        // add asterisk formmating
        std::string chars = "";
        for (int i=0; i<asterisk; i++)
            chars += "*";

        // add additional formatting and node values
        std::string output_line = "";
        std::string str_length = std::to_string(length);
        output_line += chars + str_length + ": ";

        for (size_t i = 0; i < strings.size(); ++i) {
            output_line += strings[i];
            if (i < strings.size() - 1)
                output_line += " ";
        }

        printer.push_back(output_line);
    }

    // recursively print the left and right subtrees with increased level
    printPreorder(node->left, level + 1);
    printPreorder(node->right, level + 1);
}


/**
 * -------------------------------------
 * 
 *  Prints the Binary Search Tree in inorder traversal,
 *  calls the outputs function to save it to a file
 * 
 * -------------------------------------
*/
void BST::printInorder() {;
    printer.clear(); 
    printInorder(root, 0);
    output("inorder");
}


/**
 * -------------------------------------
 *  Inorder print helper function
 * 
 *  @param node   : reference to input node
 *  @param level  : node depth/level
 * -------------------------------------
*/
void BST::printInorder(node_t* node, int level) {
    if (node == nullptr)
        return;

    // recursively print the left subtree with increased level
    printInorder(node->left, level + 1);

    // create a map to group strings by length for the current node
    std::map<int, std::vector<std::string>> grouped_data;

    // iterate through the node's data and group by string length
    for (const std::string& value : node->data)
        grouped_data[value.length()].push_back(value);

    // print grouped node data
    for (const auto& entry : grouped_data) {
        const int length = entry.first;
        const std::vector<std::string>& strings = entry.second;

        // calculate asterisk amount
        int asterisk = level * 2;

        // add asterisk formatting
        std::string chars = "";
        for (int i = 0; i < asterisk; i++)
            chars += "*";

        // add additional formatting and node values
        std::string output_line = "";
        std::string str_length = std::to_string(length);
        output_line += chars + str_length + ": ";

        for (size_t i = 0; i < strings.size(); ++i) {
            output_line += strings[i];
            if (i < strings.size() - 1)
                output_line += " ";
        }

        printer.push_back(output_line);
    }

    // recursively print the right subtree with increased level
    printInorder(node->right, level + 1);
}


/**
 * -------------------------------------
 * 
 *  Prints the Binary Search Tree in postorder traversal, 
 *  calls the outputs function to save it to a file

 * -------------------------------------
*/
void BST::printPostorder() {
    printer.clear(); 
    printPostorder(root, 0);
    output("postorder");
}


/**
 * -------------------------------------
 * 
 *  Postorder print helper function
 * 
 *  @param node   : reference to input node
 *  @param level  : node depth/level
 * -------------------------------------
*/
void BST::printPostorder(node_t* node, int level) {
    if (node == nullptr)
        return;

    // recursively print the left subtree with increased level
    printPostorder(node->left, level + 1);

    // recursively print the right subtree with increased level
    printPostorder(node->right, level + 1);

    // create a map to group strings by length for the current node
    std::map<int, std::vector<std::string>> grouped_data;

    // iterate through the node's data and group by string length
    for (const std::string& value : node->data)
        grouped_data[value.length()].push_back(value);

    // print grouped node data
    for (const auto& entry : grouped_data) {
        const int length = entry.first;
        const std::vector<std::string>& strings = entry.second;

        // calculate asterisk amount
        int asterisk = level * 2;

        // add asterisk formatting
        std::string chars = "";
        for (int i = 0; i < asterisk; i++)
            chars += "*";

        // add additional formatting and node values
        std::string output_line = "";
        std::string str_length = std::to_string(length);
        output_line += chars + str_length + ": ";

        for (size_t i = 0; i < strings.size(); ++i) {
            output_line += strings[i];
            if (i < strings.size() - 1)
                output_line += " ";
        }

        printer.push_back(output_line);
    }
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
void BST::output(std::string traversal) {
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
    else
        output = "out." + traversal; // use:  out."traversal" file name

    // create file
    std::fstream output_file(output, std::ios::out);

        // error opening created file
    if (!output_file.is_open()) { 
        std::cerr << error_file << output << std::endl;
        exit(EXIT_FAILURE);
    }    

    // write tree printer data to file
    for (auto s: printer)
        output_file << s << std::endl;

    // close file
    output_file.close();
}