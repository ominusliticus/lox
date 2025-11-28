#pragma once

#include <memory>
#include <unordered_map>
#include <vector>

#include "internals/object.hpp"
#include "internals/token.hpp" 
#include "internals/ast/ast_forward.hpp"

#include "util/error.hpp"

// TODO: Make this an abstract interface and move AST execution details to a class that inherits
// from this abstract interface called _Executor_. This is such that all overloaders have a uniform
// interface
class [[nodiscard]] Interpreter {
public:
    Interpreter();
    virtual ~Interpreter() = default;

    virtual ErrorOr<Object> interpret(Assignment* expression);
    virtual ErrorOr<Object> interpret(Binary* expression);
    virtual ErrorOr<Object> interpret(Call* expression);
    virtual ErrorOr<Object> interpret(Grouping* expression);
    virtual ErrorOr<Object> interpret(Literal* expression);
    virtual ErrorOr<Object> interpret(Logical* expression);
    virtual ErrorOr<Object> interpret(Unary* expression);
    virtual ErrorOr<Object> interpret(Variable* expression);

    virtual ErrorOr<void> interpret(Block* statements);
    virtual ErrorOr<void> interpret(IfStmt* statement);
    virtual ErrorOr<void> interpret(ExpressionStmt* statement);
    virtual ErrorOr<void> interpret(FunDeclStmt* statement);
    virtual ErrorOr<void> interpret(PrintStmt* statement);
    virtual ErrorOr<void> interpret(VarDeclStmt* statement);
    virtual ErrorOr<void> interpret(WhileStmt* statement);
    virtual ErrorOr<void> interpret(ReturnStmt* statement);

    ErrorOr<Object> evaluate(Expression* expression);
    ErrorOr<void>   execute(Statement* statement);
    ErrorOr<void>   execute_block(
        std::vector<std::unique_ptr<Statement>> const& statements, 
        Environment* environment
    );

    bool is_truthy(Object const& obj);

    Environment* globals();
    Environment* current_environment();

    void resolve(Expression* expression, int depth);

    ErrorOr<Object> lookup_variable(Token const& name, Expression* expression);

private:
    std::unique_ptr<Environment>         m_global_environment;
    Environment*                         m_current_environment;
    std::unordered_map<Expression*, int> m_locals;

    friend AST;
};

