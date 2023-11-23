/*
    Name: Anthony Blakley
    Date: 11/23/2023
    Description: 
        lexical scanner function declarations
*/

# include "../headers/scanner.h"
# include <iostream>
# include <fstream>


// global input stream reference
std::ifstream inputfile;


/**
 * ------------------------------------------
 *              Constructor
 * ------------------------------------------
*/
Scanner::Scanner() {    
    // initialize member variables
    filename = "";
    fileindex = 0;
    lineindex = 1;

    // token builder flags
    skip_flag = false;
    strings_flag = false;
    comment_flag = false;
    integer_flag = false;
    
    // initialize token properties
    _token.id = eof_tk;
    _token.instance = "";
    _token.line = lineindex;

    // create token mapping
    mapping();
}


/**
 * ------------------------------------------
 *              Destructor
 * ------------------------------------------
*/
Scanner::~Scanner() {
    // close input file stream
    if (inputfile.is_open())
        inputfile.close();

    // remove temporary file
    if (filename == "_temp") {
        if (std::remove(filename.c_str()) != 0)
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
            input_file.close();
            break;
        }
        default:
            // argument count error
            std::cerr << error_args << argc << std::endl;
            exit(EXIT_FAILURE);
    }

    // open file
    inputfile.open(filename);
    if (!inputfile.is_open())
        std::cerr << error_openfile << std::endl;
}


/**
 * ------------------------------------------
 *      Displays the token description
 * 
 * @param t : created token
 * ------------------------------------------
*/
void Scanner::display(token& t) {
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

    // print token description
    if (t.id != eof_tk)
        printf("Token: %-12s Line: %-8d Instance: %-6s\n", m[t.id].c_str(), t.line, t.instance.c_str());
    else
        printf("Token: %-12s\n", m[t.id].c_str());
}


/**
 * ------------------------------------------
 *    Create a token mapping: lookup table
 * ------------------------------------------
*/ 
void Scanner::mapping() {
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
    std::vector<std::string> o = {"=", "<", ">", "~", "+", "-", "*", "/", "%"};

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
}


/**
 * ------------------------------------------
 *  Validates token construction
 * 
 *  @param  c : character
 *  @param  t : current token
 * ------------------------------------------
*/
void Scanner::validate(char c, token& t) {
    // error message
    std::string error_chars_a = "[Error], LEXICAL ERROR. invalid character: ";
    std::string error_chars_b = "[Error], LEXICAL ERROR. invalid token start character: ";

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

    // additional alphabet check
    if (std::isspace(c) || c == '$')
        r = true;

    // comment tokens can contain any character
    if (t.instance[0] == '$')
        r = true;

    // character is not recognized
    if (!r) {
        std::cout << error_chars_a << c << ", line: " << t.line << std::endl;
        exit(EXIT_FAILURE);
    }     

    // check if the character is a valid starting character for a token
    if (c != 'x' && !std::isdigit(c) && t.instance == "" && !std::isspace(c) && c != '$') {
        // check if its a delimiter or operator
        bool invalid = true;

        for (const std::string& value : recognized["delimiters"]) {
            if (value.find(c) != std::string::npos) {
                invalid = false;
                break;
            }
        }

        for (const std::string& value : recognized["operators"]) {
            if (value.find(c) != std::string::npos) {
                invalid = false;
                break;
            }
        }        

        if (invalid) {
            std::cout << error_chars_b << c << ", line: " << t.line << "\n";
            exit(EXIT_FAILURE);            
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
        t.instance = c;
        strings_flag = true;

        // check if token is finished
        if (!std::isalnum(lookahead)) {
            t.id = identifier_tk;
            strings_flag = false;
            return true;   
        }

        return false;
    }

    // continue building token  
    if (strings_flag && std::isalnum(c)) {
        // update token
        t.instance += c;

        // check if token is finished
        if (!std::isalnum(lookahead)) {
            // reset flag
            strings_flag = false;

            // check if token is a keyword
            for(std::string& value : recognized["keywords"]) {
                if (value == t.instance) {
                    t.id = keyword_tk;
                    return true;
                }
            }

            // token is an identifer
            t.id = identifier_tk;
            return true;
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
    // check new integer token
    if (std::isdigit(c) && t.instance == "") {
        t.instance = c;
        t.id = integer_tk;
        integer_flag = true;

        // check if integer token is finished
        if (!std::isdigit(lookahead)) {
            integer_flag = false;
            return true;
        }

        return false;
    }

    // continue building integer token
    if (integer_flag && std::isdigit(c)) {
        t.instance += c;

        // check if integer token is finished
        if (!std::isdigit(lookahead)) {
            integer_flag = false;
            return true;
        }

        return false;
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
            t.instance = c;
            t.id = delimiter_tk;
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
    // check to skip processing operator due to multi-character
    if (skip_flag) {
        skip_flag = false;
        return false;
    }
    
    // check if character is an operator
    for (const std::string& value : recognized["operators"]) {
        if (value.find(c) != std::string::npos) {
            // set token type
            t.id = operator_tk;

            // check for multi-character operator [type 1]
            if (c == '<' && lookahead == '<') {
                t.instance = "<<";
                skip_flag = true;
                return true;
            }

            // check for multi-character operator [type 2]
            if (c == '>' && lookahead == '>') {
                t.instance = ">>";
                skip_flag = true;
                return true;
            }

            // standard operators
            t.instance = c;
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

    return result;
}


/**
 * ------------------------------------------
 *           Creates a token
 * 
 * @return _tokens : a built token and the 
 *                 updated current reference
 * ------------------------------------------
*/ 
_tokens Scanner::scanner() {
    // resulting token
    token result;

    // set next character
    char c;
    inputfile.seekg(fileindex);

    // process next character from file
    while (inputfile.get(c)) {    
        // update lookahead character
        lookahead = inputfile.peek();

        // build token
        bool built = tokenizer(c, _token);

        // increment line number
        if (c == '\n') {
            _token.line ++;
            lineindex ++;
        }

        // output token
        if (built) {
            fileindex ++;
            result = _token;
            _token.instance = "";
            break;
        }

        fileindex ++;
        inputfile.seekg(fileindex);
    }

    // display eof token
    if (lookahead == EOF) {
        _token.id = eof_tk;
        result = _token;
    }
    
    // return _token and result
    return std::make_tuple(_token, result); 
}


/**
 * ------------------------------------------
 *        Tester for scanner class
 * ------------------------------------------
*/ 
void Scanner::tester() {
    // test function for scanner class
    _tokens results = scanner();
    _token = std::get<0>(results);

    // output token value
    display(std::get<1>(results));

    // get tokens from file until EOF
    while (true) {
        if (_token.id == eof_tk)
            break;
        
        results = scanner();
        _token = std::get<0>(results);

        // output token value
        display(std::get<1>(results));
    }
}