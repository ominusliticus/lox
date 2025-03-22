#pragma once

#include <memory>
#include <sstream>
 
#include "internals/token.hpp"

struct Expression {
    Expression(
        Expression&& expression
    ) 
        : left_expression{ expression.left_expression }
        , operation{ expression.operation }
        , right_expression{ expression.right_expression }
    {}

    Expression(
        std::shared_ptr<Expression> left_expression_,
        std::shared_ptr<Token>      operation_,
        std::shared_ptr<Expression> right_expression_
    ) 
        : left_expression{ left_expression_ }
        , operation{ operation_ }
        , right_expression{ right_expression_ }
    {}

    std::shared_ptr<Expression> left_expression;
    std::shared_ptr<Token>      operation;
    std::shared_ptr<Expression> right_expression;
};

struct Binary : public Expression {
    Binary(
        std::shared_ptr<Expression> left_expression_,
        std::shared_ptr<Token>      operation_,
        std::shared_ptr<Expression> right_expression_
    ) 
        : Expression(left_expression_, operation_, right_expression_)
    {}
};


struct Grouping : public Expression {
    Grouping(
        std::shared_ptr<Expression> left_expression_
    ) 
        : Expression(left_expression_, {}, {})
    {}
};

struct Literal : public Expression {
    Literal(
        std::shared_ptr<Token> operation_
    ) 
        : Expression({}, operation_, {})
    {}
};

struct Unary : public Expression {
    Unary(
        std::shared_ptr<Token>      operation_,
        std::shared_ptr<Expression> right_expression_
    ) 
        : Expression({}, operation_, right_expression_)
    {}
};


struct ExpressionVisitor {
       ExpressionVisitor() = default;

    auto
    printer(
        std::stringstream& string_builder,
        Expression const& expression
    ) -> void {
        string_builder << "(";
        if (expression.operation)
            string_builder << expression.operation->lexeme;
        if (expression.left_expression) {
            string_builder << " ";
            this->printer(string_builder, *expression.left_expression);
        }
        if (expression.right_expression) {
            string_builder << " ";
            this->printer(string_builder, *expression.right_expression);
        }
        string_builder << ")";
    }

};
