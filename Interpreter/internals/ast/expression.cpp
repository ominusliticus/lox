#include "internals/ast/expression.hpp"

Expression::Expression(
    Expression&& expression
) 
    : left_expression{ expression.left_expression }
    , operation{ expression.operation }
    , right_expression{ expression.right_expression }
    , expression_type{ expression.expression_type }
{}

Expression::Expression(
    std::shared_ptr<Expression> left_expression_,
    std::shared_ptr<Token>      operation_,
    std::shared_ptr<Expression> right_expression_,
    ExpressionType              expression_type_
) 
    : left_expression{ left_expression_ }
    , operation{ operation_ }
    , right_expression{ right_expression_ }
    , expression_type{ expression_type_ }
{}
