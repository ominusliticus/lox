#pragma once

#include <memory>

#include "internals/ast/expressions/expression_type.hpp"
#include "internals/ast/walkers/interpreter.hpp"

#include "internals/token.hpp"

struct Expression {
    Expression(
        std::unique_ptr<Expression> left_expression_,
        std::unique_ptr<Token>      operation_,
        std::unique_ptr<Expression> right_expression_,
        ExpressionType              expression_type_
    );

    Expression(Expression const& other) = delete;
    Expression& operator=(Expression const& other) = delete;

    Expression(Expression&& expression_) = default;
    Expression& operator=(Expression&& expression_) = default;

    virtual ~Expression() = default;

    virtual ErrorOr<Object> visit(Interpreter* interpreter) = 0;

    std::unique_ptr<Expression> left_expression;
    std::unique_ptr<Token>      operation;
    std::unique_ptr<Expression> right_expression;
    ExpressionType              expression_type;
};
