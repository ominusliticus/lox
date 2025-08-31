#pragma once

#include <memory>

#include "internals/ast/expressions/expression_type.hpp"
#include "internals/ast/expression.hpp"

#include "internals/token.hpp"

struct Variable : public Expression {
    Variable(std::shared_ptr<Token> name_); 

    Token name;
};

