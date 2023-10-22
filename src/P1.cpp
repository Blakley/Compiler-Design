/*
    Name: Anthony Blakley
    Date: 10/21/2023
    Description: 
        Lexical Analyzer
         
*/

// Imports
# include <map>
# include <string>
# include <vector>
# include <fstream>
# include <iostream>


// == Globals ==

// character lookup table alias
using recognized = std::map<std::string, std::vector<std::string>>;

// token types
enum token_id {
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
    int line;   
    token_id id;
    std::string instance;
};

// token flags
bool strings_flag = false; // keyword or identifer
bool comment_flag = false; // comment
bool integer_flag = false; // integer


/**
 * ------------------------------------------
 * Prints the tokens list
 * 
 * @param tokens : list of tokens
 * ------------------------------------------
*/
void display(std::vector<token> tokens) {
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
    for (const auto& token : tokens)
        printf("%-12s %-8d %-6s\n", m[token.id].c_str(), token.line, token.instance.c_str());

    // print header end
    std::cout << line_a << std::endl;
    std::cout << "\n"; 
}



/**
 * ------------------------------------------
 *  Checks if a character is a recognized
 *  character in our alphabet
 * 
 *  @param  c : character
 *  @param  t : current token
 *  @param  r : recognized characters map
 * ------------------------------------------
*/
void validate(char c, token t, recognized r) {
    // error message
    std::string error_chars = "[Error], LEXICAL ERROR. invalid character: ";

    // iterate map values
    for (const auto& entry : r) {        
        for (size_t i = 0; i < entry.second.size(); ++i) {
            if (std::string(1, c) == entry.second[i])
                return;
        }
    }   

    // final check, white space and comment char
    if (std::isspace(c) || c == '$')
        return;
    else {
        std::cout << error_chars << c << ", line: " << t.line << std::endl;
        exit(EXIT_FAILURE);
    }
}


/**
 * ------------------------------------------
 *  Create a token, represent it based on
 *  one of the token types in `token_id` enum
 * 
 *  @param  c : current character
 *  @param  t : current token instance
 *  @param  l : tokens list
 *  @param  r : recognized characters map
 *  @return   : Flag if the token was built
 * ------------------------------------------
*/
bool tokenize(char c, token& t, std::vector<token>& l, recognized& r) {
    // check if the character is valid
    validate(c, t, r);

    // handle token line
    if (c == '\n')
        t.line ++;

    /*
        ===========================
        keyword & identifier tokens
        ===========================
    */ 
    
    if (c == 'x' && t.instance == "") {
        // start of identifer or keyword token
        t.instance += c;
        strings_flag = true;
        return false;
    }

    // continue building token
    if (strings_flag && t.instance != "") {
        if (std::isalnum(c)) {
            t.instance += c;
        }
        else {
            // check if the token finished: 
            bool f = false;
            
            // check if delimiter is encountered
            for(std::string& value : r["delimiters"])
                if (value.find(c) != std::string::npos)
                    f = true;

            // check if delimiter is encountered
            for(std::string& value : r["operators"])
                if (value.find(c) != std::string::npos)
                    f = true;

            // check if whitespace is encountered
            if (std::isspace(c))
                f = true;

            // check if token is finished
            if (f) {
                // check if keyword
                bool keyword = false;

                for(std::string& value : r["keywords"]) {
                    if (value == t.instance) {
                        t.id = keyword_tk;
                        keyword = true;
                        break;
                    }
                }

                // token is an identifier
                if (!keyword) 
                    t.id = identifier_tk;

                l.push_back(t);

                // reset token for next character
                t.id = eof_tk;
                t.instance = "";
                strings_flag = false;
            }
        }
    }

    /*
        ===============
        integer tokens
        ===============
    */ 
   
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
    if (integer_flag && (!std::isdigit(c) || c == '\n')) {
        t.id = integer_tk;
        l.push_back(t);
        integer_flag = false;
        
        // reset token for the next character
        t.id = eof_tk;
        t.instance = "";
    }

    /*
        ================
        delimiter tokens
        ================
    */ 

    for (const std::string& value : r["delimiters"]) {
        if (value.find(c) != std::string::npos) {
            // make token instance, add to token list
            t.instance = c;
            t.id = delimiter_tk;
            l.push_back(t);
            return true;
        }
    }

    /*
        ===============
        operator tokens
        ===============
    */ 

    for (const std::string& value : r["operators"]) {
        if (value.find(c) != std::string::npos) {
            // ceck if the current character can be appended to the last token
            if (!l.empty()) {
                token& previous = l.back();
                if (previous.instance == "<" || previous.instance == ">") {
                    previous.instance += c;
                    return true;
                }
            }            

            // make token instance, add to token list
            t.instance = c;
            t.id = operator_tk;
            l.push_back(t);
            return true;
        }
    }

    /*
        ===============
        comment tokens
        ===============
    */ 

    // beginning of comment
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

    // end of comment
    if (comment_flag && c == '$') {
        t.instance += c;
        t.id = comment_tk;
        l.push_back(t);
        comment_flag = false;
        return true;
    }

    return false;
}


/**
 * ------------------------------------------
 * Recognize the syntax & structure of tokens 
 * Identify and categorizes them
 * 
 * @param file  : input file name
 * ------------------------------------------
*/
void analyze(std::string file) {
    /*
        ==========
        open file
        ==========
    */
    
    // error messages
    std::string error_openfile = "[Error], Unable to open input file: ";

    // open file
    std::ifstream input_file(file, std::ios::in);
    
    // check if there is an error with the file
    if (!input_file.is_open()) {
        std::cerr << error_openfile << file << std::endl;
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

    // get a list of keyword stirngs
    std::vector<std::string> k = {
        "xopen", "xclose", "xloop", "xdata", "xexit", 
        "xin", "xout", "xcond", "xthen", "xlet", "xfunc"
    };

    /*
        ============================
        build character lookup table
        ============================
    */

    // lookup map
    recognized lookup;
    lookup["numbers"]    = n; 
    lookup["letters"]    = l;
    lookup["operators"]  = o; 
    lookup["delimiters"] = d;
    lookup["keywords"]   = k; 

    /*
        =============
        create tokens
        =============
    */

    // create a token list
    std::vector<token> tokens;

    // initialize first token
    token current;
    current.line = 1;
    current.id = eof_tk;
    current.instance = "";

    // file reading variables
    char c;

    // process characters from the file
    while (input_file.get(c)) {
        // build token
        bool built = tokenize(c, current, tokens, lookup);

        // reset current token
        if (built) {
            // excluding line number
            current.id = eof_tk;
            current.instance = "";
        }
    }

    /*
        =============
        handle output
        =============
    */

    // append EOF token;
    tokens.push_back(current);
    
    // print tokens
    display(tokens);

    // close input file
    input_file.close();

    // remove temporary file
    if (file == "_temp") {
        if (std::remove(file.c_str()) != 0)
            std::cerr << "[Error], Unable to delete tempory file `_temp`" << std::endl;
    }   
}


/**
 * ------------------------------------------
 * Parses command line arguments and 
 * checks the validity of the input file
 * 
 * @param argc  : argument count
 * @param argv  : argument array
 * ------------------------------------------
*/
std::string arguments(int argc, char** argv) {
    // create error messages
    std::string error_args     = "[Error], Invalid number of arguments: ";
    std::string error_openfile = "[Error], Unable to open input file: ";
    std::string error_makefile = "[Error], Unable to create temporary input file: ";

    // name of result file
    std::string filename = "";

    // determine provided arguments
    switch (argc) {
        case 1: {
            // Read argument from file type 1: redirection file

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
            // Read argument from file type 2: file argument

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

    // return validated filename
    return filename;
}


/**
 * ------------------------------------------
 *  Entry function
 * ------------------------------------------
*/
int main(int argc, char** argv) {
    
    // handle and validate input file
    std::string file = arguments(argc, argv);
    
    // identifiy and categorize tokens 
    analyze(file);

    return 0;
}   