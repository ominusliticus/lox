#pragma once

#include <memory>

#include "internals/ast/statements/statement_type.hpp"
#include "internals/ast/statement.hpp"
#include "internals/ast/expression.hpp"

struct ExpressionStmt : public Statement {
    ExpressionStmt(std::unique_ptr<Expression> expression); 

    ErrorOr<void> visit(Interpreter* interpreter) final;
};
