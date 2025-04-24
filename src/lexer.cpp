#include "lexer.h"
#include <unordered_map>
#include <cctype>

namespace cook {

// Keyword mapping
static const std::unordered_map<std::string, TokenType> keywords = {
    {"ingredient", TokenType::INGREDIENT},
    {"recipe", TokenType::RECIPE},
    {"cookbook", TokenType::COOKBOOK},
    {"cook", TokenType::COOK},
    {"taste", TokenType::TASTE}
};

Lexer::Lexer(const std::string& source) : source(source) {}

std::vector<Token> Lexer::tokenize() {
    std::vector<Token> tokens;
    
    while (!isAtEnd()) {
        // Skip whitespace and comments
        skipWhitespace();
        
        if (isAtEnd()) break;
        
        // Process the next token
        char c = advance();
        
        switch (c) {
            // Single-character tokens
            case '(': tokens.push_back(makeToken(TokenType::LPAREN)); break;
            case ')': tokens.push_back(makeToken(TokenType::RPAREN)); break;
            case '{': tokens.push_back(makeToken(TokenType::LBRACE)); break;
            case '}': tokens.push_back(makeToken(TokenType::RBRACE)); break;
            case ',': tokens.push_back(makeToken(TokenType::COMMA)); break;
            case ';': tokens.push_back(makeToken(TokenType::SEMICOLON)); break;
            
            // Operators
            case '+': tokens.push_back(makeToken(TokenType::PLUS)); break;
            case '-': tokens.push_back(makeToken(TokenType::MINUS)); break;
            case '*': tokens.push_back(makeToken(TokenType::MULTIPLY)); break;
            case '/': 
                if (match('/')) {
                    skipComment();
                } else {
                    tokens.push_back(makeToken(TokenType::DIVIDE));
                }
                break;
            case '=': tokens.push_back(makeToken(TokenType::ASSIGN)); break;
            
            // String literals
            case '"': tokens.push_back(stringToken()); break;
            
            // Number literals and identifiers
            default:
                if (std::isdigit(c)) {
                    tokens.push_back(numberToken());
                } else if (std::isalpha(c) || c == '_') {
                    tokens.push_back(identifierToken());
                } else {
                    tokens.push_back(makeToken(TokenType::UNKNOWN));
                }
                break;
        }
    }
    
    // Add EOF token
    tokens.push_back(Token(TokenType::EOF_TOKEN, "", line, column));
    
    return tokens;
}

char Lexer::peek() {
    if (isAtEnd()) return '\0';
    return source[position];
}

char Lexer::advance() {
    char current = source[position++];
    column++;
    return current;
}

bool Lexer::isAtEnd() {
    return position >= source.length();
}

bool Lexer::match(char expected) {
    if (isAtEnd() || source[position] != expected) return false;
    position++;
    column++;
    return true;
}

Token Lexer::makeToken(TokenType type) {
    return Token(type, std::string(1, source[position - 1]), line, column - 1);
}

Token Lexer::stringToken() {
    int startColumn = column - 1;
    std::string value;
    
    while (!isAtEnd() && peek() != '"') {
        if (peek() == '\n') {
            line++;
            column = 1;
        }
        value += advance();
    }
    
    // Consume the closing quote
    if (!isAtEnd()) advance();
    
    return Token(TokenType::STRING, value, line, startColumn);
}

Token Lexer::numberToken() {
    int startColumn = column - 1;
    int start = position - 1;
    
    while (!isAtEnd() && std::isdigit(peek())) {
        advance();
    }
    
    // Look for a decimal part
    if (!isAtEnd() && peek() == '.' && std::isdigit(source[position + 1])) {
        // Consume the '.'
        advance();
        
        while (!isAtEnd() && std::isdigit(peek())) {
            advance();
        }
    }
    
    std::string value = source.substr(start, position - start);
    return Token(TokenType::NUMBER, value, line, startColumn);
}

Token Lexer::identifierToken() {
    int startColumn = column - 1;
    int start = position - 1;
    
    while (!isAtEnd() && (std::isalnum(peek()) || peek() == '_')) {
        advance();
    }
    
    std::string text = source.substr(start, position - start);
    
    // Check if it's a keyword
    TokenType type = TokenType::IDENTIFIER;
    auto it = keywords.find(text);
    if (it != keywords.end()) {
        type = it->second;
    }
    
    return Token(type, text, line, startColumn);
}

void Lexer::skipWhitespace() {
    while (!isAtEnd()) {
        char c = peek();
        switch (c) {
            case ' ':
            case '\r':
            case '\t':
                advance();
                break;
            case '\n':
                line++;
                column = 1;
                advance();
                break;
            default:
                return;
        }
    }
}

void Lexer::skipComment() {
    // Skip until the end of the line
    while (!isAtEnd() && peek() != '\n') {
        advance();
    }
}

} // namespace cook
