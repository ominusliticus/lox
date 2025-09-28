#pragma once

#include "internals/ast/expressions/expression_type.hpp"
#include "internals/ast/expression.hpp"

struct Binary : public Expression {
    Binary(
        std::unique_ptr<Expression> left_expression_,
        std::unique_ptr<Token>      operation_,
        std::unique_ptr<Expression> right_expression_
    );

    ErrorOr<Object> visit(Interpreter* interpreter) final;
};

