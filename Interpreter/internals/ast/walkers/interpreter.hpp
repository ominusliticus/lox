#pragma once

#include <memory>
#include <vector>

#include "internals/object.hpp"
#include "internals/ast/ast_forward.hpp"

#include "util/error.hpp"

class [[nodiscard]] Interpreter {
public:
    Interpreter();

    ErrorOr<Object> interpret(Assignment* expression);
    ErrorOr<Object> interpret(Binary* expression);
    ErrorOr<Object> interpret(Call* expression);
    ErrorOr<Object> interpret(Grouping* expression);
    ErrorOr<Object> interpret(Literal* expression);
    ErrorOr<Object> interpret(Logical* expression);
    ErrorOr<Object> interpret(Unary* expression);
    ErrorOr<Object> interpret(Variable* expression);

    ErrorOr<void> interpret(Block* statements);
    ErrorOr<void> interpret(IfStmt* statement);
    ErrorOr<void> interpret(ExpressionStmt* statement);
    ErrorOr<void> interpret(FunDeclStmt* statement);
    ErrorOr<void> interpret(PrintStmt* statement);
    ErrorOr<void> interpret(VarDeclStmt* statement);
    ErrorOr<void> interpret(WhileStmt* statement);

    ErrorOr<Object> evaluate(Expression* expression);
    ErrorOr<void>   execute(Statement* statement);
    ErrorOr<void>   execute_block(
        std::vector<std::unique_ptr<Statement>> const& statements, 
        Environment* environment
    );

    bool is_truthy(Object const& obj);

    Environment* globals();

private:
    std::unique_ptr<Environment> m_global_environment;
    Environment* m_current_environment;

    friend AST;
};

