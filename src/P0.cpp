/*
    Name: Anthony Blakley
    Date: 09/28/2023
    Description:
         Program to practice with trees, traversals, command line arguments, and file IO.
*/

// Imports
# include "node.h"
# include "tree.h"

using namespace std;

/**
 * -------------------------------------
 * 
 * 
 * 
 * -------------------------------------
*/
static void manager(const char* file) {
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
 * 
 * 
 * -------------------------------------
*/
static char* arguments(int argc, char** argv) {
    
   /*
                                            Todo:
                                                
       Process command arguments, set up data to work regardless of source, check if file readable, set the basename for the output file, etc. 

        - Processing keyboard or file input: [PO is executable name]
             1. 
                If keyboard input ("P0" or "P0 < file", the program doesnt know which but the file there was no argument), 
                read the input into a temporary file, close and reopen to read Otherwise you have the file, just append the 
                extension and open it to read Afterwards the rest of the program always processes file input 
                
             2. 
                 If keyboard input, set file pointer to stdin otherwise set file pointer to the actual file 
                 (after adding extension and opening), then process always from the file pointer


            // process command line arguments first and error if improper arguments 
            // if filename given, make sure file is readable, error otherwise 
            // set file pointers or read to temporary file so that below this point there is only one v ersion of the code


            Assume you do not know the size of the input file. Assume the input data is all strings (printable ASCII) separated with standard WS separators 
            (space, tab, new line). If the input 
            file is not readable for whatever reason, or command line arguments are not correct, the program must abort with an appropriate message

   */


    // manager(file);
}


// Entry function
int main(int argc, char** argv) {
    arguments(argc, argv);
    return 0;
}

/*
                        TODO:
- Output:
    The program will subsequently output 3 files corresponding to 3 traversals, named file.preorder, file.inorder and file.postorder. 
    Note that file is the base name of the input file if given, and it is out.preorder, etc. if that is not not given.
*/