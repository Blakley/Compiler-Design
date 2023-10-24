/*
    Name: Anthony Blakley
    Date: 10/24/2023
    Description: 
        scanner header file for lexical scanner
*/

# include <map>
# include "token.h"

// Lexical scanner class
class Scanner {
    private:
        // lookahead character, file name & index & token line
        int fileindex;
        int lineindex;
        char lookahead;
        std::string filename;

        // list of recognized characters and corresponding token category
        std::map<std::string, std::vector<std::string>> recognized;

        // token builder flags
        bool strings_flag;
        bool comment_flag;
        bool integer_flag;
        bool skip_flag;

        // reference to created token
        token _token;

    public:
        Scanner();  // constructor
        ~Scanner(); // destructor

        void arguments(int argc, char** argv);  // argument handler
        void opener();                          // open input file
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
        token scanner();    // returns a token
};

