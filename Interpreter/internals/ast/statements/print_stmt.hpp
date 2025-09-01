#pragma once

#include <memory>

#include "internals/ast/statements/statement_type.hpp"
#include "internals/ast/statement.hpp"

struct PrintStmt : public Statement {
    PrintStmt(std::shared_ptr<Expression> expression);
};

