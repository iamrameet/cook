#ifndef COOK_LEXER_H
#define COOK_LEXER_H

#include <string>
#include <vector>

namespace cook {

// Token types
enum class TokenType {
    // Keywords
    INGREDIENT,  // Variable declaration
    RECIPE,      // Function declaration
    COOKBOOK,    // Module/library
    COOK,        // Execute/call
    TASTE,       // Print/output
    
    // Literals
    STRING,
    NUMBER,
    IDENTIFIER,
    
    // Operators
    PLUS,
    MINUS,
    MULTIPLY,
    DIVIDE,
    ASSIGN,
    
    // Delimiters
    LPAREN,      // (
    RPAREN,      // )
    LBRACE,      // {
    RBRACE,      // }
    COMMA,
    SEMICOLON,
    
    // Special
    EOF_TOKEN,
    UNKNOWN
};

// Token structure
struct Token {
    TokenType type;
    std::string lexeme;
    int line;
    int column;
    
    Token(TokenType type, const std::string& lexeme, int line, int column)
        : type(type), lexeme(lexeme), line(line), column(column) {}
};

// Lexer class
class Lexer {
public:
    Lexer(const std::string& source);
    std::vector<Token> tokenize();
    
private:
    std::string source;
    int position = 0;
    int line = 1;
    int column = 1;
    
    char peek();
    char advance();
    bool isAtEnd();
    bool match(char expected);
    
    Token makeToken(TokenType type);
    Token stringToken();
    Token numberToken();
    Token identifierToken();
    
    void skipWhitespace();
    void skipComment();
};

} // namespace cook

#endif // COOK_LEXER_H
