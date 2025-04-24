#include "lexer.h"
#include "parser.h"
#include "interpreter.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

using namespace cook;

// Read file contents into a string
std::string readFile(const std::string& path) {
    std::ifstream file(path);
    if (!file.is_open()) {
        std::cerr << "Could not open file: " << path << std::endl;
        exit(1);
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

// Run a Cook program from source
void run(const std::string& source) {
    // Lexical analysis
    Lexer lexer(source);
    std::vector<Token> tokens = lexer.tokenize();

    // Parsing
    Parser parser(tokens);
    std::unique_ptr<Program> program = parser.parse();

    // Interpretation
    Interpreter interpreter;
    interpreter.interpret(*program);
}

// Run a Cook program from a file
void runFile(const std::string& path) {
    std::cout << "Loading file: " << path << std::endl;
    std::string source = readFile(path);
    std::cout << "File loaded, running..." << std::endl;
    run(source);
    std::cout << "Execution complete." << std::endl;
}

// Run an interactive REPL
void runPrompt() {
    std::string line;
    std::cout << "Cook Programming Language v0.1.0" << std::endl;

    while (true) {
        std::cout << "> ";
        if (!std::getline(std::cin, line) || line == "exit") {
            break;
        }

        try {
            run(line);
        } catch (const std::exception& e) {
            std::cerr << "Error: " << e.what() << std::endl;
        }
    }
}

int main(int argc, char* argv[]) {
    try {
        if (argc > 2) {
            std::cout << "Usage: cook [script]" << std::endl;
            return 1;
        } else if (argc == 2) {
            runFile(argv[1]);
        } else {
            runPrompt();
        }
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
