/*
    Name: Anthony Blakley
    Date: 09/30/2023
    Description:
        Header, contains the definition for a node structure
        
*/

# pragma once
# include <set>
# include <string>

// node structure
struct node_t {
    std::set<std::string> data; // strings of same length
    node_t* left;
    node_t* right;
};
