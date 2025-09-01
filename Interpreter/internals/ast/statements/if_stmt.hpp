#pragma once

#include <memory>

#include "internals/ast/statement.hpp"
#include "internals/ast/expression.hpp"

struct IfStmt : Statement {
    IfStmt(
        std::shared_ptr<Expression> condition_,
        std::shared_ptr<Statement> then_branch_,
        std::shared_ptr<Statement> else_branch_
    );

    std::shared_ptr<Expression> condition;
    std::shared_ptr<Statement>  then_branch;
    std::shared_ptr<Statement>  else_branch;
};
