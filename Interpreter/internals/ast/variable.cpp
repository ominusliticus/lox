#include "internals/ast/variable.hpp"

Variable::Variable(
    Token name_
) 
    : Expression({}, {}, {}, ExpressionType::VARIABLE)
    , name{ std::make_shared<Token>(name_) }
{}
