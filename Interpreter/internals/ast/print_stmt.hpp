#pragma once

#include <memory>

#include "internals/ast/statement_type.hpp"
#include "internals/ast/statement.hpp"

struct PrintStmt : public Statement {
    PrintStmt(
        std::shared_ptr<Expression> expression
    )
        : Statement(expression, StatementType::PRINT)
    {}
};

