#pragma once

#include "internals/ast/expressions/expression_type.hpp"
#include "internals/ast/expression.hpp"

#include "internals/object.hpp"

struct Literal : public Expression {
    Literal(Object const& literal); 

    Object object;
};
