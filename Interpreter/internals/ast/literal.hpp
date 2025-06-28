#pragma once

#include "internals/ast/expression_type.hpp"
#include "internals/ast/expression.hpp"

#include "internals/object.hpp"

struct Literal : public Expression {
    Literal(Object literal); 

    Object object;
};
