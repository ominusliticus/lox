#pragma once

#include <memory>

#include "internals/ast/expressions/expression_type.hpp"
#include "internals/ast/expression.hpp"

#include "internals/token.hpp"

struct Assignment : public Expression {
    Assignment(Token&& name_, std::shared_ptr<Expression> value_);
    
    Token                       name;
    std::shared_ptr<Expression> value;
};

