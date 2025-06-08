#pragma once

#include <memory>
#include <sstream>
#include <utility>
#include <variant>
 
#include "internals/object.hpp"
#include "internals/token.hpp"

#include "util/error.hpp"
#include "util/print.hpp"
#include "util/try.hpp"

// TDOO: Our type system is not as convenient as the Java one.
//       We need to create a class heirarchy that allows us to automate much of the visiting down
//       the road.

enum class ExpressionType : std::uint8_t {
    BINARY,
    GROUPING,
    LITERAL,
    UNARY
};

struct Expression {
    Expression(
        Expression&& expression
    ) 
        : left_expression{ expression.left_expression }
        , operation{ expression.operation }
        , right_expression{ expression.right_expression }
        , expression_type{ expression.expression_type }
    {}

    Expression(
        std::shared_ptr<Expression> left_expression_,
        std::shared_ptr<Token>      operation_,
        std::shared_ptr<Expression> right_expression_,
        ExpressionType              expression_type_
    ) 
        : left_expression{ left_expression_ }
        , operation{ operation_ }
        , right_expression{ right_expression_ }
        , expression_type{ expression_type_ }
    {}

    std::shared_ptr<Expression> left_expression;
    std::shared_ptr<Token>      operation;
    std::shared_ptr<Expression> right_expression;
    ExpressionType              expression_type;
};

struct Binary : public Expression {
    Binary(
        std::shared_ptr<Expression> left_expression_,
        std::shared_ptr<Token>      operation_,
        std::shared_ptr<Expression> right_expression_
    ) 
        : Expression(left_expression_, operation_, right_expression_, ExpressionType::BINARY)
    {}
};


struct Grouping : public Expression {
    Grouping(
        std::shared_ptr<Expression> expression_
    ) 
        : Expression(expression_, {}, {}, ExpressionType::GROUPING)
    {}
};

struct Literal : public Expression {
    Literal(
        Object literal
    ) 
        : Expression({}, {}, {}, ExpressionType::LITERAL)
        , object{ literal }
    {}

    Object object;
};

struct Unary : public Expression {
    Unary(
        std::shared_ptr<Token>      operation_,
        std::shared_ptr<Expression> right_expression_
    ) 
        : Expression({}, operation_, right_expression_, ExpressionType::UNARY)
    {}
};

enum class StatementType {
    PRINT,
    EXPRESSION
};

struct Statement {
    Statement(
        Statement&& statement_
    )
        : expression{ statement_.expression }
        , statement_type{ statement_.statement_type }
    {}

    Statement(
        std::shared_ptr<Expression> expression_,
        StatementType               statement_type_
    )
        : expression{ expression_ }
        , statement_type{ statement_type_ }
    {}

    std::shared_ptr<Expression> expression;
    StatementType               statement_type;
};

struct PrintStmt : public Statement {
    PrintStmt(
        std::shared_ptr<Expression> expression
    )
        : Statement(expression, StatementType::PRINT)
    {}
};

struct ExpressionStmt : public Statement {
    ExpressionStmt(
        std::shared_ptr<Expression> expression
    ) 
        : Statement(expression, StatementType::EXPRESSION)
    {}
};

struct ExpressionVisitor {
       ExpressionVisitor() = default;

    auto
    printer(
        std::stringstream&          string_builder,
        std::shared_ptr<Expression> expression
    ) -> void {
        switch (expression->expression_type) {
            case ExpressionType::BINARY:
                string_builder << "(" << expression->operation->lexeme << " ";
                this->printer(string_builder, expression->left_expression);
                string_builder << " ";
                this->printer(string_builder, expression->right_expression);
                string_builder << ")";
                break;
            case ExpressionType::GROUPING:
                string_builder << "(group ";
                this->printer(string_builder, expression->left_expression);
                string_builder << ")";
                break;
            case ExpressionType::LITERAL:
                string_builder << std::reinterpret_pointer_cast<Literal>(expression)->object;
                break;
            case ExpressionType::UNARY:
                string_builder << "(";
                string_builder << expression->operation->lexeme << " ";
                this->printer(string_builder, expression->right_expression);
                string_builder << ")";
                break;
        }
    }

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
        auto obj = TRY(this->interpreter(expression));
        return obj;
    }

    auto
    execute(
        std::shared_ptr<Statement> statement
    ) -> ErrorOr<void> {
        TRY(this->interpreter(statement));
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
};
