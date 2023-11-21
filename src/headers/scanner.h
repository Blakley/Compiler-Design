/*
    Name: Anthony Blakley
    Date: 11/18/2023
    Description: 
        Header file for lexical scanner
*/

# ifndef SCANNER_H
# define SCANNER_H

# include <map>
# include <tuple>
# include "token.h"

// A built token and the updated current reference
typedef std::tuple<token, token> _tokens;

// Lexical scanner class
class Scanner {
    private:
        // lookahead character, file name & index & token line
        int fileindex;
        int lineindex;
        char lookahead;
        std::string filename;

        // list of recognized characters and their corresponding token category
        std::map<std::string, std::vector<std::string>> recognized;

        // token builder flags
        bool strings_flag;
        bool comment_flag;
        bool integer_flag;
        bool skip_flag;

    public:
        // reference to current token
        token _token;

        Scanner();  // constructor
        ~Scanner(); // destructor

        void arguments(int argc, char** argv);  // argument handler
        void display(token& t);                 // outputs a token's description

        void validate(char c, token& t);            // verify token construction
        bool tokenize_strings(char c, token& t);    // tokenize identifer & keywords
        bool tokenize_integer(char c, token& t);    // tokenize integers
        bool tokenize_delimiter(char c, token& t);  // tokenize delimiters
        bool tokenize_operator(char c, token& t);   // tokenize operators
        bool tokenize_comment(char c, token& t);    // tokenize comments

        // tokenize handler
        bool tokenizer(char c, token& t);

        void mapping();     // token filter
        void tester();      // scanner test function
        _tokens scanner();  // returns two tokens
};

# endif // SCANNER_H