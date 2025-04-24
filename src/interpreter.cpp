#include "interpreter.h"
#include <iostream>
#include <stdexcept>

namespace cook {

// Environment implementation
void Environment::define(const std::string& name, const Value& value) {
    values[name] = value;
}

Value Environment::get(const std::string& name) {
    auto it = values.find(name);
    if (it != values.end()) {
        return it->second;
    }

    throw std::runtime_error("Undefined ingredient '" + name + "'");
}

void Environment::assign(const std::string& name, const Value& value) {
    auto it = values.find(name);
    if (it != values.end()) {
        it->second = value;
        return;
    }

    throw std::runtime_error("Undefined ingredient '" + name + "'");
}

// Interpreter implementation
Interpreter::Interpreter() {}

void Interpreter::interpret(const Program& program) {
    for (const auto& stmt : program.statements) {
        executeStatement(stmt.get());
    }
}

void Interpreter::executeStatement(const Statement* stmt) {
    if (auto exprStmt = dynamic_cast<const ExpressionStmt*>(stmt)) {
        executeExpressionStmt(exprStmt);
    } else if (auto ingredientStmt = dynamic_cast<const IngredientStmt*>(stmt)) {
        executeIngredientStmt(ingredientStmt);
    } else if (auto recipeStmt = dynamic_cast<const RecipeStmt*>(stmt)) {
        executeRecipeStmt(recipeStmt);
    } else if (auto tasteStmt = dynamic_cast<const TasteStmt*>(stmt)) {
        executeTasteStmt(tasteStmt);
    } else {
        throw std::runtime_error("Unknown statement type");
    }
}

void Interpreter::executeExpressionStmt(const ExpressionStmt* stmt) {
    evaluateExpression(stmt->expression.get());
}

void Interpreter::executeIngredientStmt(const IngredientStmt* stmt) {
    Value value;
    if (stmt->initializer) {
        value = evaluateExpression(stmt->initializer.get());
    } else {
        // Default value is empty string
        value = std::string("");
    }

    environment.define(stmt->name, value);
}

void Interpreter::executeRecipeStmt(const RecipeStmt* stmt) {
    // Store the recipe for later execution
    std::vector<std::unique_ptr<Statement>> bodyCopy;

    // Deep copy the body statements
    for (const auto& statement : stmt->body) {
        if (auto exprStmt = dynamic_cast<const ExpressionStmt*>(statement.get())) {
            bodyCopy.push_back(std::make_unique<ExpressionStmt>(
                std::unique_ptr<Expression>(exprStmt->expression->clone())));
        } else if (auto tasteStmt = dynamic_cast<const TasteStmt*>(statement.get())) {
            bodyCopy.push_back(std::make_unique<TasteStmt>(
                std::unique_ptr<Expression>(tasteStmt->expression->clone())));
        } else if (auto ingredientStmt = dynamic_cast<const IngredientStmt*>(statement.get())) {
            bodyCopy.push_back(std::make_unique<IngredientStmt>(
                ingredientStmt->name,
                ingredientStmt->initializer ?
                    std::unique_ptr<Expression>(ingredientStmt->initializer->clone()) : nullptr));
        }
        // We're skipping nested recipes for simplicity
    }

    recipes[stmt->name] = Recipe(stmt->parameters, std::move(bodyCopy));

    std::cout << "Recipe '" << stmt->name << "' defined with "
              << stmt->parameters.size() << " parameters" << std::endl;
}

void Interpreter::executeTasteStmt(const TasteStmt* stmt) {
    Value value = evaluateExpression(stmt->expression.get());

    // Print the value
    if (value.isNumber()) {
        std::cout << value.getNumber() << std::endl;
    } else if (value.isString()) {
        std::cout << value.getString() << std::endl;
    }
}

Value Interpreter::evaluateExpression(const Expression* expr) {
    if (auto literalExpr = dynamic_cast<const LiteralExpr*>(expr)) {
        return evaluateLiteralExpr(literalExpr);
    } else if (auto variableExpr = dynamic_cast<const VariableExpr*>(expr)) {
        return evaluateVariableExpr(variableExpr);
    } else if (auto binaryExpr = dynamic_cast<const BinaryExpr*>(expr)) {
        return evaluateBinaryExpr(binaryExpr);
    } else if (auto assignExpr = dynamic_cast<const AssignExpr*>(expr)) {
        return evaluateAssignExpr(assignExpr);
    } else if (auto callExpr = dynamic_cast<const CallExpr*>(expr)) {
        return evaluateCallExpr(callExpr);
    } else {
        throw std::runtime_error("Unknown expression type");
    }
}

Value Interpreter::evaluateLiteralExpr(const LiteralExpr* expr) {
    if (expr->type == LiteralExpr::Type::NUMBER) {
        return std::stod(expr->value);
    } else {
        return expr->value;
    }
}

Value Interpreter::evaluateVariableExpr(const VariableExpr* expr) {
    return environment.get(expr->name);
}

Value Interpreter::evaluateBinaryExpr(const BinaryExpr* expr) {
    Value left = evaluateExpression(expr->left.get());
    Value right = evaluateExpression(expr->right.get());

    // Handle numeric operations
    if (left.isNumber() && right.isNumber()) {
        double leftVal = left.getNumber();
        double rightVal = right.getNumber();

        switch (expr->op) {
            case BinaryExpr::Operator::ADD:
                return leftVal + rightVal;
            case BinaryExpr::Operator::SUBTRACT:
                return leftVal - rightVal;
            case BinaryExpr::Operator::MULTIPLY:
                return leftVal * rightVal;
            case BinaryExpr::Operator::DIVIDE:
                if (rightVal == 0) {
                    throw std::runtime_error("Division by zero");
                }
                return leftVal / rightVal;
        }
    }

    // Handle string concatenation
    if (expr->op == BinaryExpr::Operator::ADD) {
        std::string leftStr, rightStr;

        if (left.isString()) {
            leftStr = left.getString();
        } else {
            leftStr = std::to_string(left.getNumber());
        }

        if (right.isString()) {
            rightStr = right.getString();
        } else {
            rightStr = std::to_string(right.getNumber());
        }

        return leftStr + rightStr;
    }

    throw std::runtime_error("Invalid operands for binary operator");
}

Value Interpreter::evaluateAssignExpr(const AssignExpr* expr) {
    Value value = evaluateExpression(expr->value.get());
    environment.assign(expr->name, value);
    return value;
}

Value Interpreter::evaluateCallExpr(const CallExpr* expr) {
    // Look up the recipe
    auto it = recipes.find(expr->callee);
    if (it == recipes.end()) {
        throw std::runtime_error("Undefined recipe '" + expr->callee + "'");
    }

    const Recipe& recipe = it->second;

    // Evaluate arguments
    std::vector<Value> arguments;
    for (const auto& arg : expr->arguments) {
        arguments.push_back(evaluateExpression(arg.get()));
    }

    // Check argument count
    if (arguments.size() != recipe.parameters.size()) {
        throw std::runtime_error("Expected " + std::to_string(recipe.parameters.size()) +
                                " arguments but got " + std::to_string(arguments.size()));
    }

    // Execute the recipe body with the arguments
    executeRecipeBody(recipe, arguments);

    // For now, return a default value
    return std::string("recipe result");
}

void Interpreter::executeRecipeBody(const Recipe& recipe, const std::vector<Value>& arguments) {
    // Create a new environment for the recipe execution
    Environment previousEnv = environment;

    // Bind parameters to arguments
    for (size_t i = 0; i < recipe.parameters.size(); i++) {
        environment.define(recipe.parameters[i], arguments[i]);
    }

    // Execute the recipe body
    for (const auto& stmt : recipe.body) {
        executeStatement(stmt.get());
    }

    // Restore the previous environment
    environment = previousEnv;
}

} // namespace cook
