#include "internals/ast/expressions/variable.hpp"

Variable::Variable(
    std::shared_ptr<Token> name_
) 
    : Expression({}, {}, {}, ExpressionType::VARIABLE)
    , name{ *name_ }
{}
