#pragma once

#include <memory>

#include "internals/ast/statements/statement_type.hpp"
#include "internals/ast/statement.hpp"

struct VarDeclStmt : public Statement {
    VarDeclStmt(Token const& name_, std::unique_ptr<Expression> expression);

    ErrorOr<void> visit(Interpreter* interpret) final;

    Token name;
};
