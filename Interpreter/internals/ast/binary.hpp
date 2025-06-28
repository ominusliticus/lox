#pragma once

#include "internals/ast/expression_type.hpp"
#include "internals/ast/expression.hpp"

struct Binary : public Expression {
    Binary(
        std::shared_ptr<Expression> left_expression_,
        std::shared_ptr<Token>      operation_,
        std::shared_ptr<Expression> right_expression_
    );
};

