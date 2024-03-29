/*
    Name: Anthony Blakley
    Date: 12/03/2023
    Description: 
        Header file for token structure
*/

# ifndef TOKEN_H
# define TOKEN_H

# include <vector>
# include <string>

// token ids
enum ids {
    eof_tk,
    space_tk,
    comment_tk,
    integer_tk, 
    keyword_tk,
    operator_tk,    
    delimiter_tk,   
    identifier_tk  
};

// token structure
struct token {
    int line;             // line number of token
    ids id;               // token id
    std::string instance; // token value
};

# endif // TOKEN_H