#pragma once

#include <memory>

#include "internals/ast/expressions/expression_type.hpp"
#include "internals/ast/walkers/interpreter.hpp"

#include "internals/token.hpp"

struct Expression {
    Expression(Expression&& expression);
    Expression(
        std::unique_ptr<Expression> left_expression_,
        std::unique_ptr<Token>      operation_,
        std::unique_ptr<Expression> right_expression_,
        ExpressionType              expression_type_
    );
    virtual ~Expression() = default;

    virtual ErrorOr<Object> visit(Interpreter* interpreter) = 0;

    std::unique_ptr<Expression> left_expression;
    std::unique_ptr<Token>      operation;
    std::unique_ptr<Expression> right_expression;
    ExpressionType              expression_type;
};
