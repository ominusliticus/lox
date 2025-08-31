#pragma once

#include "internals/ast/expressions/expression_type.hpp"
#include "internals/ast/expression.hpp"


struct Grouping : public Expression {
    Grouping(std::shared_ptr<Expression> expression_); 
};
