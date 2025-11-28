#pragma once

#include <deque>
#include <unordered_map>

#include "internals/ast/walkers/interpreter.hpp"
#include "internals/ast/walkers/function.hpp"

#include "internals/token.hpp"
#include "internals/ast/environment.hpp"

#include "internals/ast/statements/fun_decl_stmt.hpp"

#include "util/error.hpp"

class [[nodiscard]] Resolver : public Interpreter {
public:
    using Scope = std::unordered_map<std::string, bool>;
    using ScopePtr = std::unique_ptr<Scope>;
    // Using deque instead of stack to have index operation
    using Scopes = std::deque<ScopePtr>;

    enum class FunctionType {
        NONE,
        FUNCTION
    };

public:
    Resolver() = default;
    Resolver(Interpreter* interpreter) : m_interpreter(interpreter) {}

    virtual ErrorOr<Object> interpret(Assignment* expression) override;
    virtual ErrorOr<Object> interpret(Binary* expression) override;
    virtual ErrorOr<Object> interpret(Call* expression) override;
    virtual ErrorOr<Object> interpret(Grouping* expression) override;
    virtual ErrorOr<Object> interpret(Literal* expression) override;
    virtual ErrorOr<Object> interpret(Logical* expression) override;
    virtual ErrorOr<Object> interpret(Unary* expression) override;
    virtual ErrorOr<Object> interpret(Variable* expression) override;

    virtual ErrorOr<void> interpret(Block* statements) override;
    virtual ErrorOr<void> interpret(IfStmt* statement) override;
    virtual ErrorOr<void> interpret(ExpressionStmt* statement) override;
    virtual ErrorOr<void> interpret(FunDeclStmt* statement) override;
    virtual ErrorOr<void> interpret(PrintStmt* statement) override;
    virtual ErrorOr<void> interpret(VarDeclStmt* statement) override;
    virtual ErrorOr<void> interpret(WhileStmt* statement) override;
    virtual ErrorOr<void> interpret(ReturnStmt* statement) override;

    virtual ErrorOr<void> interpret(std::vector<std::unique_ptr<Statement>> const& statements);

private:
    Interpreter* m_interpreter;
    Scopes       m_scopes;
    FunctionType m_current_function = FunctionType::NONE;

    ErrorOr<void> resolve_function(FunDeclStmt* statement, FunctionType type);
    ErrorOr<void> declare(Token const& name);

    void define(Token const& name);
    void resolve_local(Expression* expression, Token const& name);
    void begin_scope();
    void end_scope();
};