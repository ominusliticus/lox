#include "internals/ast/binary.hpp"

Binary::Binary(
    std::shared_ptr<Expression> left_expression_,
    std::shared_ptr<Token>      operation_,
    std::shared_ptr<Expression> right_expression_
) 
    : Expression(left_expression_, operation_, right_expression_, ExpressionType::BINARY)
{}
