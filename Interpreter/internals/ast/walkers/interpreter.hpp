#pragma once

#include <memory>

#include "internals/object.hpp"
#include "internals/ast/ast_forward.hpp"

#include "util/error.hpp"

class [[nodiscard]] Interpreter {
public:
    Interpreter() = default;

    ErrorOr<Object> interpret(Assignment* expression);
    ErrorOr<Object> interpret(Binary* expression);
    ErrorOr<Object> interpret(Grouping* expression);
    ErrorOr<Object> interpret(Literal* expression);
    ErrorOr<Object> interpret(Logical* expression);
    ErrorOr<Object> interpret(Unary* expression);
    ErrorOr<Object> interpret(Variable* expression);

    ErrorOr<void> interpret(Block* statements);
    ErrorOr<void> interpret(IfStmt* statement);
    ErrorOr<void> interpret(ExpressionStmt* statement);
    ErrorOr<void> interpret(PrintStmt* statement);
    ErrorOr<void> interpret(VarDeclStmt* statement);
    ErrorOr<void> interpret(WhileStmt* statement);

    ErrorOr<Object> evaluate(Expression* expression);
    ErrorOr<void>   execute(Statement* statement);

    bool is_truthy(Object const& obj);

private:
    static std::unique_ptr<Environment> m_environment;

    friend AST;
};

