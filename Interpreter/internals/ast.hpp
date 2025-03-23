#pragma once

#include <memory>
#include <sstream>
 
#include "internals/token.hpp"

#include "util/print.hpp"

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

struct ExpressionVisitor {
       ExpressionVisitor() = default;

    auto
    printer(
        std::stringstream&          string_builder,
        std::shared_ptr<Expression> expression
    ) -> void {
        switch(expression->expression_type) {
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
};
