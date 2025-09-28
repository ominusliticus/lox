#pragma once

#include "internals/object.hpp"

#include "internals/ast/environment.hpp"
#include "internals/ast/expression.hpp"
#include "internals/ast/statement.hpp"

#include "util/error.hpp"

class [[nodiscard]] Printer {
    Printer() = default;

    ErrorOr<Object> interpret(Assignment* expr);
    ErrorOr<Object> interpret(Grouping* expr);
    ErrorOr<Object> interpret(Literal* expr);
    ErrorOr<Object> interpret(Logical* expr);
    ErrorOr<Object> interpret(Unary* expr);
    ErrorOr<Object> interpret(Variable* expr);

    ErrorOr<void> interpret(Block* expr);
    ErrorOr<void> interpret(IfStmt* expr);
    ErrorOr<void> interpret(PrintStmt* expr);
    ErrorOr<void> interpret(VarDeclStmt* expr);
};


