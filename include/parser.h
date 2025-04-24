#ifndef COOK_PARSER_H
#define COOK_PARSER_H

#include "lexer.h"
#include "ast.h"
#include <vector>
#include <memory>

namespace cook {

class Parser {
public:
    Parser(const std::vector<Token>& tokens);
    std::unique_ptr<Program> parse();
    
private:
    std::vector<Token> tokens;
    int current = 0;
    
    // Helper methods
    Token peek();
    Token previous();
    Token advance();
    bool isAtEnd();
    bool check(TokenType type);
    bool match(TokenType type);
    bool match(std::initializer_list<TokenType> types);
    Token consume(TokenType type, const std::string& message);
    
    // Parsing methods
    std::unique_ptr<Statement> declaration();
    std::unique_ptr<Statement> ingredientDeclaration();
    std::unique_ptr<Statement> recipeDeclaration();
    std::unique_ptr<Statement> statement();
    std::unique_ptr<Statement> expressionStatement();
    
    std::unique_ptr<Expression> expression();
    std::unique_ptr<Expression> assignment();
    std::unique_ptr<Expression> term();
    std::unique_ptr<Expression> factor();
    std::unique_ptr<Expression> primary();
    
    // Error handling
    void synchronize();
};

} // namespace cook

#endif // COOK_PARSER_H
