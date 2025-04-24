#ifndef COOK_INTERPRETER_H
#define COOK_INTERPRETER_H

#include "ast.h"
#include <unordered_map>
#include <string>
#include <memory>

namespace cook {

// Value class for the interpreter (instead of std::variant)
class Value {
public:
    enum class Type { NUMBER, STRING };

    Value() : type(Type::STRING), stringValue("") {}
    Value(double val) : type(Type::NUMBER), numberValue(val) {}
    Value(const std::string& val) : type(Type::STRING), stringValue(val) {}

    Type getType() const { return type; }
    double getNumber() const { return numberValue; }
    std::string getString() const { return stringValue; }

    bool isNumber() const { return type == Type::NUMBER; }
    bool isString() const { return type == Type::STRING; }

private:
    Type type;
    double numberValue = 0.0;
    std::string stringValue;
};

// Environment to store variables
class Environment {
public:
    Environment() = default;

    void define(const std::string& name, const Value& value);
    Value get(const std::string& name);
    void assign(const std::string& name, const Value& value);

private:
    std::unordered_map<std::string, Value> values;
};

// Recipe structure to store function definitions
struct Recipe {
    std::vector<std::string> parameters;
    std::vector<std::unique_ptr<Statement>> body;

    // Default constructor required for std::unordered_map
    Recipe() = default;

    Recipe(std::vector<std::string> params, std::vector<std::unique_ptr<Statement>> stmts)
        : parameters(std::move(params)), body(std::move(stmts)) {}
};

// Interpreter class
class Interpreter {
public:
    Interpreter();
    void interpret(const Program& program);

private:
    Environment environment;
    std::unordered_map<std::string, Recipe> recipes;

    // Statement visitors
    void executeStatement(const Statement* stmt);
    void executeExpressionStmt(const ExpressionStmt* stmt);
    void executeIngredientStmt(const IngredientStmt* stmt);
    void executeRecipeStmt(const RecipeStmt* stmt);
    void executeTasteStmt(const TasteStmt* stmt);

    // Expression visitors
    Value evaluateExpression(const Expression* expr);
    Value evaluateLiteralExpr(const LiteralExpr* expr);
    Value evaluateVariableExpr(const VariableExpr* expr);
    Value evaluateBinaryExpr(const BinaryExpr* expr);
    Value evaluateAssignExpr(const AssignExpr* expr);
    Value evaluateCallExpr(const CallExpr* expr);

    // Helper methods
    void executeRecipeBody(const Recipe& recipe, const std::vector<Value>& arguments);
};

} // namespace cook

#endif // COOK_INTERPRETER_H
