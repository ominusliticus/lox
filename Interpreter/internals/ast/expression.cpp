#include "internals/ast/expression.hpp"

Expression::Expression(
    std::unique_ptr<Expression> left_expression_,
    std::unique_ptr<Token>      operation_,
    std::unique_ptr<Expression> right_expression_,
    ExpressionType              expression_type_
) 
    : left_expression{ std::move(left_expression_) }
    , operation{ std::move(operation_) }
    , right_expression{ std::move(right_expression_) }
    , expression_type{ expression_type_ }
{}
