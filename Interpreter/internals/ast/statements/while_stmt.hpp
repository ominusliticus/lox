#pragma once

#include "internals/ast/statement.hpp"
#include "internals/ast/walkers/interpreter.hpp"

#include "util/error.hpp"

struct WhileStmt : public Statement {
    WhileStmt(std::unique_ptr<Expression> condition_, std::unique_ptr<Statement> body_);

    ErrorOr<void> visit(Interpreter* interpreter) final;

    std::unique_ptr<Expression> condition;
    std::unique_ptr<Statement> body;
};
