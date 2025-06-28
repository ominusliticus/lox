#pragma once

#include <memory>
#include <sstream>
#include <utility>
#include <variant>
#include <vector>
 
#include "util/print.hpp"
#include "util/try.hpp"

#include "internals/ast.hpp"

#include "internals/ast/binary.hpp"
#include "internals/ast/expression_type.hpp"
#include "internals/ast/grouping.hpp"
#include "internals/ast/literal.hpp"
#include "internals/ast/unary.hpp"
#include "internals/ast/variable.hpp"

#include "internals/ast/expression_stmt.hpp"
#include "internals/ast/print_stmt.hpp"
#include "internals/ast/statement_type.hpp"
#include "internals/ast/var_decl_stmt.hpp"

// TDOO: Our type system is not as convenient as the Java one.
//       We need to create a class heirarchy that allows us to automate much of the visiting down
//       the road.

namespace ast {

auto
interpret(
    std::vector<std::shared_ptr<Statement>> statements
) -> ErrorOr<void> {
    for (auto const& statement : statements)
        TRY(execute(statement));
    return {};
}

auto 
interpreter(
    std::shared_ptr<Statement> statement
) -> ErrorOr<void> {
    switch (statement->statement_type) {
        case StatementType::EXPRESSION:
            TRY(evaluate(statement->expression));
            break;
        case StatementType::PRINT:
            Object obj = TRY(evaluate(statement->expression));
            print(obj);
            break;
    }
    return {};
}

auto 
interpreter(
    std::shared_ptr<Expression> expression
) -> ErrorOr<Object> {
    switch (expression->expression_type) {
        case ExpressionType::GROUPING:
            return evaluate(expression->left_expression);
        case ExpressionType::LITERAL:
            return std::reinterpret_pointer_cast<Literal>(expression)->object;
        case ExpressionType::UNARY:
        {
            Object right_obj = TRY(evaluate(expression->right_expression));
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
        case ExpressionType::BINARY:
        {
            Object left_obj  = TRY(evaluate(expression->left_expression));
            Object right_obj = TRY(evaluate(expression->right_expression));

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
        default: 
            return ErrorType::UNREACHABLE;
    }
}

auto
evaluate(
    std::shared_ptr<Expression> expression
) -> ErrorOr<Object> {
    auto obj = TRY(ast::interpreter(expression));
    return obj;
}

auto
execute(
    std::shared_ptr<Statement> statement
) -> ErrorOr<void> {
    TRY(ast::interpreter(statement));
    return {};
}

auto
is_truthy(
    Object const& obj
) -> bool {
    // Only explicit false or nil value return false, all other values return true
    if (std::get_if<Nil>(&obj.literal))
        return false;
    if (std::get_if<bool>(&obj.literal))
        return std::get<bool>(obj.literal);
    else return true;
}
}
