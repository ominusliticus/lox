#include "internals/ast/literal.hpp"

Literal::Literal(
    Object literal
) 
    : Expression({}, {}, {}, ExpressionType::LITERAL)
    , object{ literal }
{}
