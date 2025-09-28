#pragma once

#include <vector>
#include <memory>

#include "internals/ast/statement.hpp"
#include "internals/ast/expression.hpp"


struct Block : public Statement {
    Block(std::vector<std::unique_ptr<Statement>>&& statements_);

    ErrorOr<void> visit(Interpreter* interpret) final;

    std::vector<std::unique_ptr<Statement>> statements;
};
