#include "internals/ast/expressions/literal.hpp"

Literal::Literal(
    Object const& literal
) 
    : Expression({}, {}, {}, ExpressionType::LITERAL)
    , object{ literal }
{}
