/*
    Name: Anthony Blakley
    Date: 10/23/2023
    Description: 
        Header file for lexical scanner
*/

# include <map>
# include <vector>
# include <string>

// Lexical scanner class
class Scanner {
    private:
        // input file name
        std::string inputfile;

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

        // token list
        std::vector<token> token_list;

        // list of recognized characters and corresponding token category
        std::map<std::string, std::vector<std::string>> recognized;

        // token builder flags
        bool strings_flag;
        bool comment_flag;
        bool integer_flag;

    public:
        Scanner();  // constructor
        ~Scanner(); // destructor

        void validate(char c, token& t);    // verify token construction

        void arguments(int argc, char** argv);
        void display();                     // output token list    
        void filter();                      // token filter function

        bool tokenize_strings(char c, token& t);    // tokenize identifer & keywords
        bool tokenize_integer(char c, token& t);    // tokenize integers
        bool tokenize_delimiter(char c, token& t);  // tokenize delimiters
        bool tokenize_operator(char c, token& t);   // tokenize operators
        bool tokenize_comment(char c, token& t);    // tokenize comments

        // tokenize handler
        bool tokenizer(char c, token& t);
};

