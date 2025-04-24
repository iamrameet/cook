#ifndef COOK_AST_H
#define COOK_AST_H

#include <string>
#include <vector>
#include <memory>

namespace cook {

// Forward declarations
class Expression;
class Statement;

// Base AST node
class ASTNode {
public:
    virtual ~ASTNode() = default;
};

// Program - the root of our AST
class Program : public ASTNode {
public:
    std::vector<std::unique_ptr<Statement>> statements;

    Program() = default;
};

// Base Expression class
class Expression : public ASTNode {
public:
    virtual ~Expression() = default;
    virtual Expression* clone() const = 0;
};

// Literal expression (numbers, strings)
class LiteralExpr : public Expression {
public:
    enum class Type { NUMBER, STRING };

    Type type;
    std::string value;

    LiteralExpr(Type type, const std::string& value)
        : type(type), value(value) {}

    Expression* clone() const override {
        return new LiteralExpr(type, value);
    }
};

// Variable reference expression
class VariableExpr : public Expression {
public:
    std::string name;

    VariableExpr(const std::string& name) : name(name) {}

    Expression* clone() const override {
        return new VariableExpr(name);
    }
};

// Binary expression (a + b, etc.)
class BinaryExpr : public Expression {
public:
    enum class Operator { ADD, SUBTRACT, MULTIPLY, DIVIDE };

    Operator op;
    std::unique_ptr<Expression> left;
    std::unique_ptr<Expression> right;

    BinaryExpr(Operator op, std::unique_ptr<Expression> left, std::unique_ptr<Expression> right)
        : op(op), left(std::move(left)), right(std::move(right)) {}

    Expression* clone() const override {
        return new BinaryExpr(
            op,
            std::unique_ptr<Expression>(left->clone()),
            std::unique_ptr<Expression>(right->clone())
        );
    }
};

// Assignment expression (a = b)
class AssignExpr : public Expression {
public:
    std::string name;
    std::unique_ptr<Expression> value;

    AssignExpr(const std::string& name, std::unique_ptr<Expression> value)
        : name(name), value(std::move(value)) {}

    Expression* clone() const override {
        return new AssignExpr(
            name,
            std::unique_ptr<Expression>(value->clone())
        );
    }
};

// Call expression (recipe())
class CallExpr : public Expression {
public:
    std::string callee;
    std::vector<std::unique_ptr<Expression>> arguments;

    CallExpr(const std::string& callee, std::vector<std::unique_ptr<Expression>> arguments)
        : callee(callee), arguments(std::move(arguments)) {}

    Expression* clone() const override {
        std::vector<std::unique_ptr<Expression>> argsCopy;
        for (const auto& arg : arguments) {
            argsCopy.push_back(std::unique_ptr<Expression>(arg->clone()));
        }
        return new CallExpr(callee, std::move(argsCopy));
    }
};

// Base Statement class
class Statement : public ASTNode {
public:
    virtual ~Statement() = default;
};

// Expression statement
class ExpressionStmt : public Statement {
public:
    std::unique_ptr<Expression> expression;

    ExpressionStmt(std::unique_ptr<Expression> expression)
        : expression(std::move(expression)) {}
};

// Variable declaration statement (ingredient)
class IngredientStmt : public Statement {
public:
    std::string name;
    std::unique_ptr<Expression> initializer;

    IngredientStmt(const std::string& name, std::unique_ptr<Expression> initializer)
        : name(name), initializer(std::move(initializer)) {}
};

// Function declaration statement (recipe)
class RecipeStmt : public Statement {
public:
    std::string name;
    std::vector<std::string> parameters;
    std::vector<std::unique_ptr<Statement>> body;

    RecipeStmt(const std::string& name, std::vector<std::string> parameters,
               std::vector<std::unique_ptr<Statement>> body)
        : name(name), parameters(std::move(parameters)), body(std::move(body)) {}
};

// Print statement (taste)
class TasteStmt : public Statement {
public:
    std::unique_ptr<Expression> expression;

    TasteStmt(std::unique_ptr<Expression> expression)
        : expression(std::move(expression)) {}
};

} // namespace cook

#endif // COOK_AST_H
