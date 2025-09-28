#pragma once

#include <memory>

#include "internals/ast/expression.hpp"

#include "internals/token.hpp"

struct Logical : public Expression {
    Logical(
        std::unique_ptr<Expression> left,
        std::unique_ptr<Token> operation, 
        std::unique_ptr<Expression> right
    );

    ErrorOr<Object> visit(Interpreter* interpreter) final;
};
