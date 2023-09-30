/*
    Name: Anthony Blakley
    Date: 09/29/2023
    Description:
        Header, contains the definition for a node structure
        
*/

# pragma once
# include <set>
# include <string>

// node structure
struct node_t {
    std::set<std::string> data; // set of strings
    node_t* left;
    node_t* right;
};
