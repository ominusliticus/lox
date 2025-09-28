#pragma once

#include <memory>

#include "internals/ast/expressions/expression_type.hpp"
#include "internals/ast/expression.hpp"

#include "internals/token.hpp"

struct Assignment : public Expression {
    Assignment(Token&& name_, std::unique_ptr<Expression> value_);

    ErrorOr<Object> visit(Interpreter* interpreter) final;
    
    Token                       name;
    std::unique_ptr<Expression> value;
};

