#pragma once

#include <memory>

#include "internals/ast/expression.hpp"

#include "internals/token.hpp"

struct Logical : public Expression {
    Logical(
        std::shared_ptr<Expression> left,
        std::shared_ptr<Token> operation, 
        std::shared_ptr<Expression> right
    );
};
