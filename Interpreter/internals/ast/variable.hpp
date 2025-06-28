#pragma once

#include <memory>

#include "internals/ast/expression_type.hpp"
#include "internals/ast/expression.hpp"

#include "internals/token.hpp"

struct Variable : public Expression {
    Variable(Token name_); 

    std::shared_ptr<Token> name;
};

