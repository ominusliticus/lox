#include "internals/ast/walkers/interpreter.hpp"
#include "internals/ast/walkers/function.hpp"
#include "internals/ast/walkers/return.hpp"

#include "internals/object.hpp"
#include "internals/token.hpp"

#include "internals/ast/environment.hpp"

#include "internals/ast/expression.hpp"
#include "internals/ast/expressions/assignment.hpp"
#include "internals/ast/expressions/binary.hpp"
#include "internals/ast/expressions/call.hpp"
#include "internals/ast/expressions/expression_type.hpp"
#include "internals/ast/expressions/grouping.hpp"
#include "internals/ast/expressions/literal.hpp"
#include "internals/ast/expressions/logical.hpp"
#include "internals/ast/expressions/unary.hpp"
#include "internals/ast/expressions/variable.hpp"

#include "internals/ast/statement.hpp"
#include "internals/ast/statements/block.hpp"
#include "internals/ast/statements/if_stmt.hpp"
#include "internals/ast/statements/expression_stmt.hpp"
#include "internals/ast/statements/fun_decl_stmt.hpp"
#include "internals/ast/statements/print_stmt.hpp"
#include "internals/ast/statements/return_stmt.hpp"
#include "internals/ast/statements/statement_type.hpp"
#include "internals/ast/statements/var_decl_stmt.hpp"
#include "internals/ast/statements/while_stmt.hpp"

#include "internals/native/clock.hpp"

#include "util/try.hpp"

Interpreter::Interpreter() {
    m_global_environment = std::make_unique<Environment>();
    m_current_environment = m_global_environment.get();
    m_current_environment->define("clock", std::make_unique<native::Clock>());
}

// ....oooO0Oooo....oooO0Oooo....oooO0Oooo....oooO0Oooo....oooO0Oooo....oooO0Oooo....oooO0Oooo....

auto
Interpreter::interpret(
    Assignment* expression
) -> ErrorOr<Object> {
    auto value = TRY(evaluate(expression->value.get()));
    TRY(m_current_environment->assign(expression->name.lexeme, std::move(value)));
    return value;
}

// ....oooO0Oooo....oooO0Oooo....oooO0Oooo....oooO0Oooo....oooO0Oooo....oooO0Oooo....oooO0Oooo....

auto 
Interpreter::interpret(
    Binary* expression
) -> ErrorOr<Object> {
    Object left_obj  = TRY(evaluate(expression->left_expression.get()));
    Object right_obj = TRY(evaluate(expression->right_expression.get()));

    bool is_double = ( std::get_if<double>(&left_obj.literal) &&
                       std::get_if<double>(&right_obj.literal) );
    bool is_string = ( std::get_if<std::string>(&left_obj.literal) &&
                       std::get_if<std::string>(&right_obj.literal) );
    switch (expression->operation->type) {
        case TokenType::MINUS:
            if (is_double)
                return Object{ std::get<double>(left_obj.literal) - 
                                std::get<double>(right_obj.literal) };
            return ErrorType::UNMATCHED_BINARY_OP_TYPES;
        case TokenType::SLASH:
            if (is_double) {
                if (std::get<double>(right_obj.literal) == 0.0)
                    return ErrorType::DIV_BY_ZERO;
                return Object{ std::get<double>(left_obj.literal) /
                                std::get<double>(right_obj.literal) };
            }
            return ErrorType::UNMATCHED_BINARY_OP_TYPES;
        case TokenType::STAR:
            if (is_double)
                return Object{ std::get<double>(left_obj.literal) *
                                std::get<double>(right_obj.literal) };
            else return ErrorType::UNMATCHED_BINARY_OP_TYPES;
            case TokenType::PLUS:
            if (is_double)
                return Object{ std::get<double>(left_obj.literal) + 
                                std::get<double>(right_obj.literal) };
            if (is_string)
                return Object{ std::get<std::string>(left_obj.literal) +
                                std::get<std::string>(right_obj.literal) };
            return ErrorType::UNMATCHED_BINARY_OP_TYPES;
        case TokenType::GREATER:
            if (is_double)
                return Object{ std::get<double>(left_obj.literal) > 
                                std::get<double>(right_obj.literal) };
            return ErrorType::UNMATCHED_BINARY_OP_TYPES;
        case TokenType::GREATER_EQUAL:
            if (is_double)
                return Object{ std::get<double>(left_obj.literal) >= 
                                std::get<double>(right_obj.literal) };
            return ErrorType::UNMATCHED_BINARY_OP_TYPES;
        case TokenType::LESS:
            if (is_double)
                return Object{ std::get<double>(left_obj.literal) < 
                                std::get<double>(right_obj.literal) };
            return ErrorType::UNMATCHED_BINARY_OP_TYPES;
        case TokenType::LESS_EQUALS:
            if (is_double)
                return Object{ std::get<double>(left_obj.literal) <= 
                                std::get<double>(right_obj.literal) };
            return ErrorType::UNMATCHED_BINARY_OP_TYPES;
        case TokenType::EQUAL_EQUAL:
            return Object{ left_obj == right_obj };
        case TokenType::BANG_EQUAL:
            return Object{ !( left_obj == right_obj ) };
        default:
            return ErrorType::UNREACHABLE;
    }
}

// ....oooO0Oooo....oooO0Oooo....oooO0Oooo....oooO0Oooo....oooO0Oooo....oooO0Oooo....oooO0Oooo....

auto
Interpreter::interpret(
    Call* expression 
) -> ErrorOr<Object> {
    Token name = reinterpret_cast<Variable*>(expression->callee.get())->name;
    Function *function = reinterpret_cast<Function*>(
        TRY(m_current_environment->get_function(name))
    );
    std::vector<Object> arguments;
    for (auto const& argument : expression->arguments)
        arguments.push_back(TRY(evaluate(argument.get())));
    // Intentionally using exceptions to catch the return value statement execution
    try {
        TRY(function->operator()(this, arguments));
    } catch (Return return_value) {
        return return_value.value();
    }
    return {};
}

// ....oooO0Oooo....oooO0Oooo....oooO0Oooo....oooO0Oooo....oooO0Oooo....oooO0Oooo....oooO0Oooo....

auto 
Interpreter::interpret(
    Grouping* expression
) -> ErrorOr<Object> {
    return TRY(evaluate(expression->left_expression.get()));
}

// ....oooO0Oooo....oooO0Oooo....oooO0Oooo....oooO0Oooo....oooO0Oooo....oooO0Oooo....oooO0Oooo....

auto
Interpreter::interpret(
    Literal* expression
) -> ErrorOr<Object> {
    return expression->object;
}

// ....oooO0Oooo....oooO0Oooo....oooO0Oooo....oooO0Oooo....oooO0Oooo....oooO0Oooo....oooO0Oooo....

auto
Interpreter::interpret(
    Logical* expression
) -> ErrorOr<Object> {
    Object left = TRY(evaluate(expression->left_expression.get()));
    if (expression->operation->type == TokenType::OR) {
        if (is_truthy(left)) return left;
    }
    else {
        if (!is_truthy(left)) return left;
    }
    return TRY(evaluate(expression->right_expression.get()));
}

// ....oooO0Oooo....oooO0Oooo....oooO0Oooo....oooO0Oooo....oooO0Oooo....oooO0Oooo....oooO0Oooo....

auto
Interpreter::interpret(
    Unary* expression
) -> ErrorOr<Object> {
    Object right_obj = TRY(evaluate(expression->right_expression.get()));
    switch (expression->operation->type) {
        case TokenType::BANG:
            return Object { !is_truthy(right_obj) };
        case TokenType::MINUS:
            if (std::get_if<double>(&right_obj.literal)) 
                return Object{ -std::get<double>(right_obj.literal) };
            return ErrorType::IMPROPER_UNARY_NEGATION;
        default:
            return ErrorType::UNREACHABLE;
    }
}

// ....oooO0Oooo....oooO0Oooo....oooO0Oooo....oooO0Oooo....oooO0Oooo....oooO0Oooo....oooO0Oooo....

auto
Interpreter::interpret(
    Variable* expression
) -> ErrorOr<Object> {
    return TRY(m_current_environment->get(expression->name));
}

// ....oooO0Oooo....oooO0Oooo....oooO0Oooo....oooO0Oooo....oooO0Oooo....oooO0Oooo....oooO0Oooo....

auto
Interpreter::evaluate(
    Expression* expression
) -> ErrorOr<Object> {
    Object obj = TRY(expression->visit(this));
    return obj;
}

// ....oooO0Oooo....oooO0Oooo....oooO0Oooo....oooO0Oooo....oooO0Oooo....oooO0Oooo....oooO0Oooo....

auto 
Interpreter::interpret(
    Block* statements
) -> ErrorOr<void> {
    std::unique_ptr<Environment> new_envirnoment = std::make_unique<Environment>(
        m_current_environment
    );
    TRY(execute_block(std::move(statements->statements), new_envirnoment.get()));
    return {};
}

// ....oooO0Oooo....oooO0Oooo....oooO0Oooo....oooO0Oooo....oooO0Oooo....oooO0Oooo....oooO0Oooo....

auto 
Interpreter::interpret(
    ExpressionStmt* statement
) -> ErrorOr<void> {
    TRY(evaluate(statement->expression.get()));
    return {};
}

// ....oooO0Oooo....oooO0Oooo....oooO0Oooo....oooO0Oooo....oooO0Oooo....oooO0Oooo....oooO0Oooo....

auto 
Interpreter::interpret(
    FunDeclStmt* statement
) -> ErrorOr<void> {
    std::unique_ptr<Function> function = std::make_unique<Function>(statement);
    m_current_environment->define(statement->name->lexeme, std::move(function));
    return {};
}

// ....oooO0Oooo....oooO0Oooo....oooO0Oooo....oooO0Oooo....oooO0Oooo....oooO0Oooo....oooO0Oooo....

auto
Interpreter::interpret(
    IfStmt* statement
) -> ErrorOr<void> {
    auto condition_eval = TRY(evaluate(statement->condition.get()));
    if (is_truthy(condition_eval)) TRY(execute(statement->then_branch.get()));
    else if (statement->else_branch) TRY(execute(statement->else_branch.get()));
    return {};
}

// ....oooO0Oooo....oooO0Oooo....oooO0Oooo....oooO0Oooo....oooO0Oooo....oooO0Oooo....oooO0Oooo....

auto 
Interpreter::interpret(
    PrintStmt* statement
) -> ErrorOr<void> {
    // Variable declarations need to scoped in switch statement
    Object object = TRY(evaluate(statement->expression.get()));
    print("\033[35m", object, "\033[0m");
    return {};
}

// ....oooO0Oooo....oooO0Oooo....oooO0Oooo....oooO0Oooo....oooO0Oooo....oooO0Oooo....oooO0Oooo....

auto
Interpreter::interpret(
    VarDeclStmt* statement
) -> ErrorOr<void> {
    Object value(nil);
    if (statement->expression)
        value = TRY(evaluate(statement->expression.get()));
    m_current_environment->define(statement->name.lexeme, std::move(value));
    return {};
}

// ....oooO0Oooo....oooO0Oooo....oooO0Oooo....oooO0Oooo....oooO0Oooo....oooO0Oooo....oooO0Oooo....

auto
Interpreter::interpret(
    WhileStmt* statement
) -> ErrorOr<void> {
    while (true) {
        Object object = TRY(evaluate(statement->condition.get()));
        bool truthy = is_truthy(object);
        if (!truthy) break;
        TRY(execute(statement->body.get()));
    }
    return {};
}

// ....oooO0Oooo....oooO0Oooo....oooO0Oooo....oooO0Oooo....oooO0Oooo....oooO0Oooo....oooO0Oooo...

auto
Interpreter::interpret(
    ReturnStmt* statement
) -> ErrorOr<void> {
    Object value = Object(nil);
    if (statement->value) value = TRY(evaluate(statement->value.get()));

    throw Return(std::move(value));
    return {};
}

// ....oooO0Oooo....oooO0Oooo....oooO0Oooo....oooO0Oooo....oooO0Oooo....oooO0Oooo....oooO0Oooo....

auto
Interpreter::execute(
    Statement* statement
) -> ErrorOr<void> {
    TRY(statement->visit(this));
    return {};
}

// ....oooO0Oooo....oooO0Oooo....oooO0Oooo....oooO0Oooo....oooO0Oooo....oooO0Oooo....oooO0Oooo....

auto
Interpreter::execute_block(
    const std::vector<std::unique_ptr<Statement>>& statements,
    Environment* environment
) -> ErrorOr<void> {
    Environment* previous = m_current_environment;
    m_current_environment = environment;
    for (auto const& statement : statements)
        TRY(statement->visit(this));
    m_current_environment = previous;
    return {};
}

// ....oooO0Oooo....oooO0Oooo....oooO0Oooo....oooO0Oooo....oooO0Oooo....oooO0Oooo....oooO0Oooo....

auto
Interpreter::is_truthy(
    Object const& obj
) -> bool {
    // Only explicit false or nil value return false, all other values return true
    if (std::get_if<Nil>(&obj.literal))
        return false;
    if (std::get_if<bool>(&obj.literal))
        return std::get<bool>(obj.literal);
    else return true;
}

// ....oooO0Oooo....oooO0Oooo....oooO0Oooo....oooO0Oooo....oooO0Oooo....oooO0Oooo....oooO0Oooo....

auto
Interpreter::globals(
) ->Environment* {
    return m_global_environment.get();
}

// ....oooO0Oooo....oooO0Oooo....oooO0Oooo....oooO0Oooo....oooO0Oooo....oooO0Oooo....oooO0Oooo....

auto
Interpreter::current_environment(
) ->Environment* {
    return m_current_environment;
}
