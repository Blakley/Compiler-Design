/*
    Name: Anthony Blakley
    Date: 09/29/2023
    Description:
         Program to practice with trees, traversals, 
         command line arguments, and file IO.
*/

// Imports
# include "node.h"
# include "tree.h"
# include <fstream>
# include <iostream>


/**
 * -------------------------------------
 * 
 *  Setups up the binary search tree,
 *  and prints the traveral results
 * 
 *  @param file : name of input file
 * -------------------------------------
*/
static void manager(std::string file) {
    
    std::cout << "Manager: filename: " << file << std::endl;
    
    // create & build tree
    BST tree(file);
    tree.buildTree();

    // print tree traverals
    tree.printInorder();
    tree.printInorder(); 
    tree.printPreorder(); 
    tree.printPostorder();
}

/**
 * -------------------------------------
 * 
 * Parses command line arguments and 
 * checks the validity of the input data
 * 
 * @param argc  : argument count
 * @param argv  : argument array
 * -------------------------------------
*/
static void arguments(int argc, char** argv) {
    // create error messages
    std::string error_args = "[Error], Invalid number of arguments: ";
    std::string error_openfile = "[Error], Unable to open input file: ";
    std::string error_makefile = "[Error], Unable to create temporary input file: ";

    // name of result file
    std::string result = "";

    // determine provided arguments
    switch (argc) {
        case 1: {
            // [no arguments provided : read from keyboard until EOF or redirection file

            std::fstream temporary("tempfile", std::ios::out);

             // error opening created file
            if (!temporary.is_open()) { 
                std::cerr << error_makefile << "tempfile" << std::endl;
                exit(EXIT_FAILURE);
            }

            // store the input data in the temporary file
            std::string data;
            while (std::getline(std::cin, data)) {
                temporary << data << std::endl;
            }
            temporary.close();

            result = "tempfile";
            break;
        }
        case 2: {
            // [one argument provided : treat as input file]

            // append the required extension
            std::string name = argv[1];
            name += ".f23";
            
            // open file
            std::ifstream input_file(name, std::ios::in);
            
            // check if there is an error with the file
            if (!input_file.is_open()) {
                std::cerr << error_openfile << name << std::endl;
                exit(EXIT_FAILURE);
            }

            result = name;
            break;
        }
        default:
            // argument count error
            std::cerr << error_args << argc << std::endl;
            exit(EXIT_FAILURE);
    }

    // create the BST
    manager(result);
}


// Entry function
int main(int argc, char** argv) {
    arguments(argc, argv);
    return 0;
}   