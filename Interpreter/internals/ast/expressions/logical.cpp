#include "internals/ast/expressions/logical.hpp"

Logical::Logical(
    std::shared_ptr<Expression> left,
    std::shared_ptr<Token> operation, 
    std::shared_ptr<Expression> right
)
    : Expression(left, operation, right, ExpressionType::LOGICAL)
{}
