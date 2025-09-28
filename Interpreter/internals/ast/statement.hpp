#pragma once

#include <memory>

#include "internals/ast/statements/statement_type.hpp"
#include "internals/ast/expression.hpp"
#include "internals/ast/walkers/interpreter.hpp"

struct Statement {
    Statement(Statement const& statment_) = delete;
    Statement& operator=(Statement const& other) = delete;

    Statement(Statement&& statement_) = default;
    Statement& operator=(Statement&& statement_) = default;

    Statement(std::unique_ptr<Expression> expression_, StatementType statement_type_);
    virtual ~Statement() = default;

    virtual ErrorOr<void> visit(Interpreter* interpreter) = 0;

    std::unique_ptr<Expression> expression;
    StatementType               statement_type;
};
