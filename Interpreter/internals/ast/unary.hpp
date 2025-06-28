#pragma once

#include "internals/ast/expression_type.hpp"
#include "internals/ast/expression.hpp"

struct Unary : public Expression {
    Unary(std::shared_ptr<Token> operation_, std::shared_ptr<Expression> right_expression_);
};
