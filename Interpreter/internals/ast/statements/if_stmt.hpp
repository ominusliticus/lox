#pragma once

#include <memory>

#include "internals/ast/statement.hpp"
#include "internals/ast/expression.hpp"

struct IfStmt : Statement {
    IfStmt(
        std::unique_ptr<Expression> condition_,
        std::unique_ptr<Statement> then_branch_,
        std::unique_ptr<Statement> else_branch_
    );

    ErrorOr<void> visit(Interpreter* interpret) final;

    std::unique_ptr<Expression> condition;
    std::unique_ptr<Statement>  then_branch;
    std::unique_ptr<Statement>  else_branch;
};
