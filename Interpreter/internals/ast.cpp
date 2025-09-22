#include <memory>
#include <sstream>
#include <utility>
#include <variant>
#include <vector>
 
#include "util/print.hpp"
#include "util/try.hpp"

#include "internals/ast.hpp"

// TODO:
//  0. Add ....oooO0Oooo..... separators
//  1. No use of auto keyword
//  2. Use copy initialization over uniform initialization
//  3. Align on identifier name
//  4. Alight on equal signs

// NOTE: As our inheritance structure neither relies on virtual functions nor virtual distructores,
//       we are safe to use std::static_pointer_cast for all our pointer conversion down our 
//       hierarchy tree

std::unique_ptr<Environment> AST::m_environment = std::make_unique<Environment>();

auto
AST::interpret(
    std::vector<std::shared_ptr<Statement>>&& statements
) -> ErrorOr<void> {
    for (auto const& statement : statements)
        TRY(execute(statement));
    return {};
}

auto 
AST::interpreter(
    std::shared_ptr<Statement> statement
) -> ErrorOr<void> {
    switch (statement->statement_type) {
        case StatementType::EXPRESSION:
            TRY(evaluate(statement->expression));
            break;
        case StatementType::PRINT: 
        { 
            // Variable declarations need to scoped in switch statement
            Object obj = TRY(evaluate(statement->expression));
            print(obj);
            break;
        }
        case StatementType::VAR_DECL: 
        {
            Object value(nil);
            auto var_stmt = std::static_pointer_cast<VarDeclStmt>(statement);
            if (var_stmt->expression)
                value = TRY(evaluate(var_stmt->expression));
            m_environment->define(var_stmt->name.lexeme, std::move(value));
            break;
        }
        case StatementType::BLOCK:
        {
            auto block_stmt = std::static_pointer_cast<Block>(statement);
            m_environment = std::make_unique<Environment>(std::move(m_environment));
            for (auto& stmt : block_stmt->statements)
                TRY(execute(stmt));
            m_environment = std::move(*m_environment.release()).enclosing();
            break;
        }
        case StatementType::IF:
        {
            auto if_stmt = std::static_pointer_cast<IfStmt>(statement);
            auto condition_eval = TRY(evaluate(if_stmt->condition));
            if (is_truthy(condition_eval)) TRY(execute(if_stmt->then_branch));
            else if (if_stmt->else_branch) TRY(execute(if_stmt->else_branch));
            break;
        }
        default:
            return ErrorType::UNREACHABLE;
    }
    return {};
}

auto 
AST::interpreter(
    std::shared_ptr<Expression> expression
) -> ErrorOr<Object> {
    switch (expression->expression_type) {
        case ExpressionType::GROUPING:
            return evaluate(expression->left_expression);
        case ExpressionType::LITERAL:
            return std::reinterpret_pointer_cast<Literal>(expression)->object;
        case ExpressionType::LOGICAL:
        {
            Object left = TRY(evaluate(expression->left_expression));
            if (expression->operation->type == TokenType::OR) {
                if (is_truthy(left)) return left;
            }
            else {
                if (!is_truthy(left)) return left;
            }
            return TRY(evaluate(expression->right_expression));
        }
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
        case ExpressionType::VARIABLE: 
            return TRY(m_environment->get(
                std::static_pointer_cast<Variable>(expression)->name
            ));
        case ExpressionType::ASSIGNMENT:
        {
            auto expr = std::static_pointer_cast<Assignment>(expression);
            Object value = TRY(evaluate(expr->value));
            TRY(m_environment->assign(expr->name.lexeme, std::move(value)));
            return value;
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
AST::evaluate(
    std::shared_ptr<Expression> expression
) -> ErrorOr<Object> {
    auto obj = TRY(interpreter(expression));
    return obj;
}

auto
AST::execute(
    std::shared_ptr<Statement> statement
) -> ErrorOr<void> {
    TRY(interpreter(statement));
    return {};
}

auto
AST::is_truthy(
    Object const& obj
) -> bool {
    // Only explicit false or nil value return false, all other values return true
    if (std::get_if<Nil>(&obj.literal))
        return false;
    if (std::get_if<bool>(&obj.literal))
        return std::get<bool>(obj.literal);
    else return true;
}
