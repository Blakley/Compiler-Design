/*
    Name: Anthony Blakley
    Date: 10/23/2023
    Description: 
        Implementation of a lexical scanner. The scanner is responsible 
        for tokenizing input text, recognizing various types of tokens 
        and organizing them into a list of tokens for further processing.
        *[ Option #2: FSA Implementation ]
*/

#include "P1.h"
#include <iostream>
#include <fstream>


/**
 * ------------------------------------------
 *              Constructor
 * ------------------------------------------
*/
Scanner::Scanner() {
    // initialize member variables
    inputfile = "";
    strings_flag = false;
    comment_flag = false;
    integer_flag = false;
}


/**
 * ------------------------------------------
 *              Destructor
 * ------------------------------------------
*/
Scanner::~Scanner() {
    // remove temporary file
    if (inputfile == "_temp") {
        if (std::remove(inputfile.c_str()) != 0)
            std::cerr << "[Error], Unable to delete tempory file `_temp`" << std::endl;
    }  
}


/**
 * ------------------------------------------
 * Process command line arguments
 * 
 * @param argc  : argument count
 * @param argv  : argument array
 * ------------------------------------------
*/
void Scanner::arguments(int argc, char** argv) {
    // create error messages
    std::string error_args     = "[Error], Invalid number of arguments: ";
    std::string error_openfile = "[Error], Unable to open input file: ";
    std::string error_makefile = "[Error], Unable to create temporary input file: ";

    // name of result file
    std::string filename = "";

    // determine provided arguments
    switch (argc) {
        case 1: {
            // [Read argument from file type 1: redirection file]

            //  create temporary file
            std::fstream temporary("_temp", std::ios::out);

             // error opening created file
            if (!temporary.is_open()) { 
                std::cerr << error_makefile << "_temp" << std::endl;
                exit(EXIT_FAILURE);
            }

            // store the input data in the temporary file
            std::string data;
            while (std::getline(std::cin, data)) {
                temporary << data << std::endl;
            }
            temporary.close();

            filename = "_temp";
            break;
        }
        case 2: {
            // [Read argument from file type 2: file argument]

            // append the required extension
            std::string name = argv[1];
            name += ".f23";
            
            // open file
            std::ifstream input_file(name, std::ios::in);
            
            // check if there is an error with the file
            if (!input_file.is_open()) {
                std::cerr << error_openfile << name << std::endl;
                exit(EXIT_FAILURE);
            }

            filename = name;
            break;
        }
        default:
            // argument count error
            std::cerr << error_args << argc << std::endl;
            exit(EXIT_FAILURE);
    }

    // update inputfile name
    inputfile = filename;
}


/**
 * ------------------------------------------
 *      Displays the tokens list
 * ------------------------------------------
*/
void Scanner::display() {

    // display format variables
    std::string line_a = std::string(35, '=');
    std::string line_b = std::string(35, '-');

    // get token type mapping
    std::map<int, std::string> m = {
        {0, "EOF"},
        {1, "Space"},
        {2, "Comment"},
        {3, "Integer"},
        {4, "Keyword"},
        {5, "Operator"},
        {6, "Delimiter"},
        {7, "Identifer"},
    };

    // print header start
    std::cout << "\n"; 
    std::cout << line_a << std::endl;
    printf("%4s %10s %12s", "Token", "Line", "Instance\n");
    std::cout << line_b << std::endl;

    // print tokens
    for (const auto& token : token_list)
        printf("%-12s %-8d %-6s\n", m[token.id].c_str(), token.line, token.instance.c_str());

    // print header end
    std::cout << line_a << std::endl;
    std::cout << "\n";
}


/**
 * ------------------------------------------
 *  Checks if a character is a recognized
 *  Checks for invalid token combinations
 *  Checks if a integer token is finished
 *  Ensures to handle tokens without spaces
 * 
 *  @param  c : character
 *  @param  t : current token
 * ------------------------------------------
*/
void Scanner::validate(char c, token& t) {
    // error message
    std::string error_chars_a = "[Error], LEXICAL ERROR. invalid character: ";
    std::string error_chars_b = "[Error], LEXICAL ERROR. invalid token start character: ";
    std::string error_chars_c = "[Error], LEXICAL ERROR. invalid character(s): ";

    // check if character is part of our lexical alphabet
    bool r = false;
    for (const auto& entry : recognized) {        
        for (size_t i = 0; i < entry.second.size(); ++i) {
            if (std::string(1, c) == entry.second[i]) {
                r = true;
                break;
            }
        }
    }   

    if (std::isspace(c) || c == '$')
        r = true;

    // character is not recognized
    if (!r) {
        std::cout << error_chars_a << c << ", line: " << t.line << std::endl;
        exit(EXIT_FAILURE);
    } 

    // check if the character is a valid starting character for a token
    if (!std::isdigit(c) && c != 'x' && t.instance == "" && !std::isspace(c) && c != '$') {
        // check if its a delimiter or operator
        bool v = true;

        for (const std::string& value : recognized["delimiters"]) {
            if (value.find(c) != std::string::npos) {
                v = false;
                break;
            }
        }

        for (const std::string& value : recognized["operators"]) {
            if (value.find(c) != std::string::npos) {
                v = false;
                break;
            }
        }        

        // invalid starting character
        if (v) {
            // xlooplovely should error
            // xloop is a keyword and lovely isn't a valid identififer

            std::cout << error_chars_b << c << ", line: " << t.line << "\n";
            exit(EXIT_FAILURE);            
        }
    }

    // check for invalid token combinations
    if (t.instance != "") {

        // check if current character is a letter
        for (const std::string& value : recognized["letters"]) {
            if (value.find(c) != std::string::npos) {
    
                // check if letter != 'x' (only valid starting letter)
                if (c != 'x') {
                    // get previous token instance
                    char previous = t.instance[t.instance.length() - 1];

                    // check if previous character is a integer
                    if (std::isdigit(previous)) {
                        // error
                        // example: 2open, 3e
                        std::cout << error_chars_c << previous << c << ", line: " << t.line << std::endl;
                        exit(EXIT_FAILURE);
                    }
                }

                break;    
            }
        }
    }    

    // check if integer token is finished (since identifer token can be combined with integers)
    if (t.instance != "" && !std::isdigit(c)) {
        // handles: 123xyz (ensures both are processed seperately)

        // check if all characters are digits
        bool digits = true;
        std::string previous = t.instance;
        
        for (size_t i = 0; i < previous.length(); i++) {
            if (!std::isdigit(previous[i])) {
                digits = false;
                break;
            }
        }

        // check & finish previous digit token
        if (digits) {
            t.id = integer_tk;
            token_list.push_back(t);
            integer_flag = false;

            // reset token for the next character
            t.id = eof_tk;
            t.instance = "";
        }        
    }    
}


/**
 * ------------------------------------------
 *    Tokenize identifier and keywords
 * 
 *  @param  c : current character
 *  @param  t : current token instance
 *  @return   : Flag if the token was built
 * ------------------------------------------
*/ 
bool Scanner::tokenize_strings(char c, token& t) {
    // start building identifer/keyword token
    if (c == 'x' && t.instance == "") {
        t.instance += c;
        strings_flag = true;
        return false;
    }

    // continue building token  
    if (strings_flag && t.instance != "") {
        // update token
        if (std::isalnum(c)) {
            t.instance += c;

            // check if token is a keyword
            for(std::string& value : recognized["keywords"]) {
                if (value == t.instance) {
                    t.id = keyword_tk;
                    token_list.push_back(t);

                    // reset token for next character
                    t.id = eof_tk;
                    t.instance = "";
                    strings_flag = false;
                    return true;
                }
            }
        }
        else {
            // token is an identifer
            t.id = identifier_tk;
            token_list.push_back(t);
            
            // reset token for next character
            t.id = eof_tk;
            t.instance = "";
            strings_flag = false;
        }
    }

    return false;
}


/**
 * ------------------------------------------
 *           Tokenize integers
 * 
 *  @param  c : current character
 *  @param  t : current token instance
 *  @return   : Flag if the token was built
 * ------------------------------------------
*/ 
bool Scanner::tokenize_integer(char c, token& t) {
    // check for integer tokens
    if (std::isdigit(c) && t.instance == "") {
        t.instance += c;
        integer_flag = true;
        return false;
    }

    // continue building integer token
    if (integer_flag && std::isdigit(c)) {
        t.instance += c;
        return false;
    }

    // check if integer token is complete
    if (integer_flag && (!std::isdigit(c))) {
        t.id = integer_tk;
        token_list.push_back(t);
        integer_flag = false;
        
        // reset token for the next character
        t.id = eof_tk;
        t.instance = "";
    }

    return false;
}


/**
 * ------------------------------------------
 *           Tokenize delimiters
 * 
 *  @param  c : current character
 *  @param  t : current token instance
 *  @return   : Flag if the token was built
 * ------------------------------------------
*/ 
bool Scanner::tokenize_delimiter(char c, token& t) { 
    // check if character is a delimiter
    for (const std::string& value : recognized["delimiters"]) {
        if (value.find(c) != std::string::npos) {
            // make token instance, add to token list
            t.instance = c;
            t.id = delimiter_tk;
            token_list.push_back(t);
            return true;
        }
    }

    return false;
}


/**
 * ------------------------------------------
 *           Tokenize operators
 * 
 *  @param  c : current character
 *  @param  t : current token instance
 *  @return   : Flag if the token was built
 * ------------------------------------------
*/ 
bool Scanner::tokenize_operator(char c, token& t) {
    // check if character is an operator
    for (const std::string& value : recognized["operators"]) {
        if (value.find(c) != std::string::npos) {
            
            // check for multi-character operators
            if (!token_list.empty()) {
                // get previous token
                token& previous = token_list.back();
            
                if (c == '<' && previous.instance == "<") {
                    previous.instance += c;
                    return true;
                }

                if (c == '>' && previous.instance == ">") {
                    previous.instance += c;
                    return true;
                }
            }            

            // make token instance, add to token list
            t.instance = c;
            t.id = operator_tk;
            token_list.push_back(t);
            return true;
        }
    }

    return false;
}


/**
 * ------------------------------------------
 *           Tokenize comments
 *
 *  @param  c : current character
 *  @param  t : current token instance
 *  @return   : Flag if the token was built
 * ------------------------------------------
*/ 
bool Scanner::tokenize_comment(char c, token& t) {
    // beginning of comment token
    if (!comment_flag && c == '$') {
        t.instance = c;
        comment_flag = true;
        return false;
    }

    // continue building comment token
    if (comment_flag && c != '$') {
        t.instance += c;
        return false;
    }

    // end of comment token
    if (comment_flag && c == '$') {
        t.instance += c;
        t.id = comment_tk;
        token_list.push_back(t);
        comment_flag = false;
        return true;
    }

    return false;
}


/**
 * ------------------------------------------
 *  Create a token, represent it based on
 *  one of the token types.
 * 
 *  @param  c : current character
 *  @param  t : current token instance
 *  @return   : Flag if the token was built
 * ------------------------------------------
*/
bool Scanner::tokenizer(char c, token& t) {
    // token check
    bool result = false;

    // validate character & token construction
    validate(c, t);

    // handle keyword & identifer tokens
    result = tokenize_strings(c, t);
    if (result)
        return true;

    // handle integer tokens
    result = tokenize_integer(c, t);
    if (result)
        return true;

    // handle delimiter tokens
    result = tokenize_delimiter(c, t);
    if (result)
        return true;

    // handle operator tokens
    result = tokenize_operator(c, t);
    if (result)
        return true;

    // handle comment tokens
    result = tokenize_comment(c, t);
    if (result)
        return true;

    return false;
}


/**
 * ------------------------------------------
 *    Filter tokens from an input file
 * ------------------------------------------
*/ 
void Scanner::filter() {
    /*
        ==========
        open file
        ==========
    */
    
    // error messages
    std::string error_openfile = "[Error], Unable to open input file: ";

    // open file
    std::ifstream openfile(inputfile, std::ios::in);
    
    // check if there is an error opening the file
    if (!openfile.is_open()) {
        std::cerr << error_openfile << inputfile << std::endl;
        exit(EXIT_FAILURE);
    }

    /*
        =====================
        build lexer alhphabet
        =====================
    */

    // get a list of recognized letters
    std::vector<std::string> l = {}; 

    for (char c = '0'; c <= 'z'; c++) {
        if (std::isalpha(c)) {
            std::string letter = std::string(1, c);
            l.push_back(letter);
        }
    }

    // get a list of recognized numbers
    std::vector<std::string> n = {}; 

    for (char c = '0'; c <= '9'; c++) {
        std::string number = std::string(1, c);
        n.push_back(number);
    }

    // get a list of delimiters & operator characters
    std::vector<std::string> d = {".", ",", ":", ";", "(", ")", "{", "}", "[", "]"};
    std::vector<std::string> o = {"=", "<<", ">>", ">", "<", "~", "+", "-", "*", "/", "%"};

    // get a list of keywords
    std::vector<std::string> k = {
        "xopen", "xclose", "xloop", "xdata", "xexit", 
        "xin", "xout", "xcond", "xthen", "xlet", "xfunc"
    };

    // store values in recognition map
    recognized["numbers"]    = n; 
    recognized["letters"]    = l;
    recognized["operators"]  = o; 
    recognized["delimiters"] = d;
    recognized["keywords"]   = k; 

    /*
        =============
        create tokens
        =============
    */

    // first token reference
    token current;
    current.line = 1;
    current.id = eof_tk;
    current.instance = "";

    // process characters from the file
    char c;
    while (openfile.get(c)) {
        // build token
        bool built = tokenizer(c, current);

        // increment line number
        if (c == '\n')
            current.line ++;
             
        // reset current token
        if (built) {
            current.id = eof_tk;
            current.instance = "";
        }
    }

    // append EOF token to list;
    token_list.push_back(current);

    // close input file
    openfile.close();
}


/**
 * ------------------------------------------
 *              Entry function
 * ------------------------------------------
*/
int main(int argc, char** argv) {
    // get scanner object
    Scanner scanner;
  
    // handle input arguments
    scanner.arguments(argc, argv);

    // filter tokens
    scanner.filter();

    // output tokens
    scanner.display();

    return 0;
}
