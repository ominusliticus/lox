#pragma once

#include <memory>

#include "internals/ast/statement_type.hpp"
#include "internals/ast/statement.hpp"
#include "internals/ast/expression.hpp"

struct ExpressionStmt : public Statement {
    ExpressionStmt(
        std::shared_ptr<Expression> expression
    ) 
        : Statement(expression, StatementType::EXPRESSION)
    {}
};
