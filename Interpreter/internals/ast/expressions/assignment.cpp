#include "internals/ast/expressions/assignment.hpp"

Assignment::Assignment(
    Token&&                     name_,
    std::shared_ptr<Expression> value_
) 
    : Expression({}, {}, {}, ExpressionType::ASSIGNMENT)
    , name{ name_ }
    , value{ value_ }
{}
