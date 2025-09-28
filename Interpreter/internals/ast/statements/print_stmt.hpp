#pragma once

#include <memory>

#include "internals/ast/statements/statement_type.hpp"
#include "internals/ast/statement.hpp"

struct PrintStmt : public Statement {
    PrintStmt(std::unique_ptr<Expression> expression);

    ErrorOr<void> visit(Interpreter* interpret) final;
};

