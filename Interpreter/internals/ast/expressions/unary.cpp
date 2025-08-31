#include "internals/ast/expressions/unary.hpp"

Unary::Unary(
    std::shared_ptr<Token>      operation_,
    std::shared_ptr<Expression> right_expression_
) 
    : Expression({}, operation_, right_expression_, ExpressionType::UNARY)
{}
