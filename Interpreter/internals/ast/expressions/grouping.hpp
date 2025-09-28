#pragma once

#include "internals/ast/expressions/expression_type.hpp"
#include "internals/ast/expression.hpp"


struct Grouping : public Expression {
    Grouping(std::unique_ptr<Expression> expression_); 

    ErrorOr<Object> visit(Interpreter* interpreter) final;
};
