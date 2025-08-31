#pragma once

#include <memory>

#include "internals/ast/statements/statement_type.hpp"
#include "internals/ast/statement.hpp"

struct VarDeclStmt : public Statement {
    VarDeclStmt(Token const& name_, std::shared_ptr<Expression> expression);

    Token name;
};
