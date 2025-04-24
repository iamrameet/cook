#include "parser.h"
#include <stdexcept>

namespace cook {

Parser::Parser(const std::vector<Token>& tokens) : tokens(tokens) {}

std::unique_ptr<Program> Parser::parse() {
    auto program = std::make_unique<Program>();
    
    while (!isAtEnd()) {
        try {
            program->statements.push_back(declaration());
        } catch (const std::exception& e) {
            synchronize();
        }
    }
    
    return program;
}

Token Parser::peek() {
    return tokens[current];
}

Token Parser::previous() {
    return tokens[current - 1];
}

Token Parser::advance() {
    if (!isAtEnd()) current++;
    return previous();
}

bool Parser::isAtEnd() {
    return peek().type == TokenType::EOF_TOKEN;
}

bool Parser::check(TokenType type) {
    if (isAtEnd()) return false;
    return peek().type == type;
}

bool Parser::match(TokenType type) {
    if (check(type)) {
        advance();
        return true;
    }
    return false;
}

bool Parser::match(std::initializer_list<TokenType> types) {
    for (auto type : types) {
        if (check(type)) {
            advance();
            return true;
        }
    }
    return false;
}

Token Parser::consume(TokenType type, const std::string& message) {
    if (check(type)) return advance();
    
    throw std::runtime_error(message + " at line " + 
                            std::to_string(peek().line) + 
                            ", column " + std::to_string(peek().column));
}

std::unique_ptr<Statement> Parser::declaration() {
    if (match(TokenType::INGREDIENT)) return ingredientDeclaration();
    if (match(TokenType::RECIPE)) return recipeDeclaration();
    
    return statement();
}

std::unique_ptr<Statement> Parser::ingredientDeclaration() {
    Token name = consume(TokenType::IDENTIFIER, "Expect ingredient name");
    
    std::unique_ptr<Expression> initializer = nullptr;
    if (match(TokenType::ASSIGN)) {
        initializer = expression();
    }
    
    consume(TokenType::SEMICOLON, "Expect ';' after ingredient declaration");
    return std::make_unique<IngredientStmt>(name.lexeme, std::move(initializer));
}

std::unique_ptr<Statement> Parser::recipeDeclaration() {
    Token name = consume(TokenType::IDENTIFIER, "Expect recipe name");
    
    consume(TokenType::LPAREN, "Expect '(' after recipe name");
    
    std::vector<std::string> parameters;
    if (!check(TokenType::RPAREN)) {
        do {
            Token param = consume(TokenType::IDENTIFIER, "Expect parameter name");
            parameters.push_back(param.lexeme);
        } while (match(TokenType::COMMA));
    }
    
    consume(TokenType::RPAREN, "Expect ')' after parameters");
    consume(TokenType::LBRACE, "Expect '{' before recipe body");
    
    std::vector<std::unique_ptr<Statement>> body;
    while (!check(TokenType::RBRACE) && !isAtEnd()) {
        body.push_back(declaration());
    }
    
    consume(TokenType::RBRACE, "Expect '}' after recipe body");
    
    return std::make_unique<RecipeStmt>(name.lexeme, std::move(parameters), std::move(body));
}

std::unique_ptr<Statement> Parser::statement() {
    if (match(TokenType::TASTE)) {
        auto expr = expression();
        consume(TokenType::SEMICOLON, "Expect ';' after taste statement");
        return std::make_unique<TasteStmt>(std::move(expr));
    }
    
    return expressionStatement();
}

std::unique_ptr<Statement> Parser::expressionStatement() {
    auto expr = expression();
    consume(TokenType::SEMICOLON, "Expect ';' after expression");
    return std::make_unique<ExpressionStmt>(std::move(expr));
}

std::unique_ptr<Expression> Parser::expression() {
    return assignment();
}

std::unique_ptr<Expression> Parser::assignment() {
    auto expr = term();
    
    if (match(TokenType::ASSIGN)) {
        auto value = assignment();
        
        if (auto* varExpr = dynamic_cast<VariableExpr*>(expr.get())) {
            return std::make_unique<AssignExpr>(varExpr->name, std::move(value));
        }
        
        throw std::runtime_error("Invalid assignment target");
    }
    
    return expr;
}

std::unique_ptr<Expression> Parser::term() {
    auto expr = factor();
    
    while (match({TokenType::PLUS, TokenType::MINUS})) {
        TokenType op = previous().type;
        auto right = factor();
        
        BinaryExpr::Operator binOp = (op == TokenType::PLUS) 
                                    ? BinaryExpr::Operator::ADD 
                                    : BinaryExpr::Operator::SUBTRACT;
        
        expr = std::make_unique<BinaryExpr>(binOp, std::move(expr), std::move(right));
    }
    
    return expr;
}

std::unique_ptr<Expression> Parser::factor() {
    auto expr = primary();
    
    while (match({TokenType::MULTIPLY, TokenType::DIVIDE})) {
        TokenType op = previous().type;
        auto right = primary();
        
        BinaryExpr::Operator binOp = (op == TokenType::MULTIPLY) 
                                    ? BinaryExpr::Operator::MULTIPLY 
                                    : BinaryExpr::Operator::DIVIDE;
        
        expr = std::make_unique<BinaryExpr>(binOp, std::move(expr), std::move(right));
    }
    
    return expr;
}

std::unique_ptr<Expression> Parser::primary() {
    if (match(TokenType::NUMBER)) {
        return std::make_unique<LiteralExpr>(
            LiteralExpr::Type::NUMBER, previous().lexeme);
    }
    
    if (match(TokenType::STRING)) {
        return std::make_unique<LiteralExpr>(
            LiteralExpr::Type::STRING, previous().lexeme);
    }
    
    if (match(TokenType::IDENTIFIER)) {
        std::string name = previous().lexeme;
        
        // Check if it's a function call
        if (match(TokenType::LPAREN)) {
            std::vector<std::unique_ptr<Expression>> arguments;
            
            if (!check(TokenType::RPAREN)) {
                do {
                    arguments.push_back(expression());
                } while (match(TokenType::COMMA));
            }
            
            consume(TokenType::RPAREN, "Expect ')' after arguments");
            
            return std::make_unique<CallExpr>(name, std::move(arguments));
        }
        
        // Otherwise it's a variable reference
        return std::make_unique<VariableExpr>(name);
    }
    
    if (match(TokenType::LPAREN)) {
        auto expr = expression();
        consume(TokenType::RPAREN, "Expect ')' after expression");
        return expr;
    }
    
    throw std::runtime_error("Expect expression");
}

void Parser::synchronize() {
    advance();
    
    while (!isAtEnd()) {
        if (previous().type == TokenType::SEMICOLON) return;
        
        switch (peek().type) {
            case TokenType::INGREDIENT:
            case TokenType::RECIPE:
            case TokenType::COOKBOOK:
            case TokenType::TASTE:
                return;
            default:
                break;
        }
        
        advance();
    }
}

} // namespace cook
