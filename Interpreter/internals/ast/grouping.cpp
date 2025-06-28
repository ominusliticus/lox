#include "internals/ast/grouping.hpp"

Grouping::Grouping(
    std::shared_ptr<Expression> expression_
) 
    : Expression(expression_, {}, {}, ExpressionType::GROUPING)
{}
