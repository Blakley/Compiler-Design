/*
    Name: Anthony Blakley
    Date: 11/16/2023
    Description: 
        Outlines a node structure
*/

# ifndef NODE_H
# define NODE_H

# include <string>
# include <vector>

// node structure for parse tree
struct node {
    std::string label;                // label for the node, same as nonterminal function name
    std::vector<std::string> tokens;  // store relevant tokens processed in the function
    std::vector<node*> children;      // children for each nonterminal in a production
};

// node function definitions
node* create(const std::string& label);
void new_child(node* parent, node* child);
void new_token(node* node, const std::string& token);

# endif // NODE_H