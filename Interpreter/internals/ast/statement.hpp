#pragma once

#include <memory>

#include "internals/ast/statement_type.hpp"
#include "internals/ast/expression.hpp"

struct Statement {
    Statement(Statement&& statement_);
    Statement(std::shared_ptr<Expression> expression_, StatementType statement_type_);

    std::shared_ptr<Expression> expression;
    StatementType               statement_type;
};
