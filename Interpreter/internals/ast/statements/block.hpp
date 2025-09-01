#pragma once

#include <vector>
#include <memory>

#include "internals/ast/statement.hpp"
#include "internals/ast/expression.hpp"


struct Block : public Statement {
    Block(std::vector<std::shared_ptr<Statement>>&& statements_);

    std::vector<std::shared_ptr<Statement>> statements;
};
