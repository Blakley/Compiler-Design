/*
    Name: Anthony Blakley
    Date: 09/28/2023
    Description:
        
*/

// node.h
# pragma once
# include <string>

//
struct node_t {
    std::string data;
    node_t* left;
    node_t* right;
};
