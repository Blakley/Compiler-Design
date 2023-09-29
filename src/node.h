/*
    Name: Anthony Blakley
    Date: 09/28/2023
    Description:
        Header, contains the definition for a node structure
        
*/

# pragma once
# include <string>

// node structure
struct node_t {
    std::string data;
    node_t* left;
    node_t* right;
};
