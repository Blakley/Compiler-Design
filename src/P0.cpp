/*
    Name: Anthony Blakley
    Date: 09/28/2023
    Description:
         Program to practice with trees, traversals, 
         command line arguments, and file IO.
*/

// Imports
# include "node.h"
# include "tree.h"


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
    // assign root node
    node_t* root = nullptr;

    // create & build tree
    BST tree;
    tree.buildTree(root, file);

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

/*
    Function: TODO


    1. Process command line arguments:
        * If no arguments are provided, the program should read data from the keyboard until EOF (End of File).
        * If a single argument is provided, the program should treat it as the input file name. 
          The program should check if the file exists and is readable. 
          If the file is readable, the program should process data from the file.

    2. Handling keyboard input:
        * If the program is reading from the keyboard (stdin), it should temporarily store the input into a temporary file. 
          This is necessary because the program should always process data from a file pointer, even if it's reading from the keyboard. 
          The temporary file allows you to unify the processing logic.

    3. Handling file input:
        * If a valid input file is provided as a command line argument, 
          the program should append the required extension (".f23") to the filename and open it for reading.

    4. Error handling:
        *If the command line arguments are incorrect or the specified input file is not readable for any reason, 
         the program should abort with an appropriate error message.
    
    5. Once the data source (file or temporary file) is set up correctly based on the command line arguments, 
    the function should call the manager function to proceed with building and processing the binary search tree.

    ---------------
     invocation
    --------------
    1.
        P0: When no arguments are provided, the program reads data from the keyboard until it encounters simulated EOF (End of File).
    
    2. 
        P0 < somefile: When the program is invoked with < somefile, it indicates that the program should 
        read input data from the file named "somefile" (without the need for explicit argument). 
        This input redirection is performed by the shell, and the entire filename must 
        include the file extension (e.g., "somefile.f23").

    3. 
        P0 somefile: When a single argument is provided (e.g., "somefile"), the program treats it as the name of the input file. 
        The program appends the required extension (".f23") to the filename and opens it for reading. 
        This allows you to specify the input file explicitly.
*/

}


// Entry function
int main(int argc, char** argv) {
    arguments(argc, argv);
    return 0;
}   