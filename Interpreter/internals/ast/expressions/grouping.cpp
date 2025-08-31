#include "internals/ast/expressions/grouping.hpp"

Grouping::Grouping(
    std::shared_ptr<Expression> expression_
) 
    : Expression(expression_, {}, {}, ExpressionType::GROUPING)
{}
