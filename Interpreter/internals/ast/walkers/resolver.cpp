#include "internals/ast/walkers/resolver.hpp"
#include "internals/ast/walkers/function.hpp"
#include "internals/ast/walkers/interpreter.hpp"
#include "internals/ast/walkers/return.hpp"

#include "internals/object.hpp"
#include "internals/token.hpp"

#include "internals/ast/environment.hpp"

#include "internals/ast/expression.hpp"
#include "internals/ast/expressions/assignment.hpp"
#include "internals/ast/expressions/binary.hpp"
#include "internals/ast/expressions/call.hpp"
#include "internals/ast/expressions/expression_type.hpp"
#include "internals/ast/expressions/grouping.hpp"
#include "internals/ast/expressions/literal.hpp"
#include "internals/ast/expressions/logical.hpp"
#include "internals/ast/expressions/unary.hpp"
#include "internals/ast/expressions/variable.hpp"

#include "internals/ast/statement.hpp"
#include "internals/ast/statements/block.hpp"
#include "internals/ast/statements/if_stmt.hpp"
#include "internals/ast/statements/expression_stmt.hpp"
#include "internals/ast/statements/fun_decl_stmt.hpp"
#include "internals/ast/statements/print_stmt.hpp"
#include "internals/ast/statements/return_stmt.hpp"
#include "internals/ast/statements/statement_type.hpp"
#include "internals/ast/statements/var_decl_stmt.hpp"
#include "internals/ast/statements/while_stmt.hpp"

#include "internals/native/clock.hpp"

#include "util/try.hpp"
#include "util/colors.hpp"



auto
Resolver::interpret(
    Assignment* expression 
) -> ErrorOr<Object> {
    TRY(expression->value->visit(this));
    resolve_local(expression, expression->name);
    return Object(nil);
}

// ....oooO0Oooo....oooO0Oooo....oooO0Oooo....oooO0Oooo....oooO0Oooo....oooO0Oooo....oooO0Oooo....

auto
Resolver::interpret(
    Binary* expression
) -> ErrorOr<Object> {
    TRY(expression->left_expression->visit(this));
    TRY(expression->right_expression->visit(this));
    return Object(nil);
}

// ....oooO0Oooo....oooO0Oooo....oooO0Oooo....oooO0Oooo....oooO0Oooo....oooO0Oooo....oooO0Oooo....

auto
Resolver::interpret(
    Call* expression
) -> ErrorOr<Object> {
    TRY(expression->callee->visit(this));
    for (auto const& argument : expression->arguments)
        TRY(argument->visit(this));
    return Object(nil);
}

// ....oooO0Oooo....oooO0Oooo....oooO0Oooo....oooO0Oooo....oooO0Oooo....oooO0Oooo....oooO0Oooo....

auto
Resolver::interpret(
    Grouping* expression
) -> ErrorOr<Object> {
    TRY(expression->left_expression->visit(this));
    return Object(nil);
}

// ....oooO0Oooo....oooO0Oooo....oooO0Oooo....oooO0Oooo....oooO0Oooo....oooO0Oooo....oooO0Oooo....

auto
Resolver::interpret(
    [[maybe_unused]] Literal* expression
) -> ErrorOr<Object> {
    return Object(nil);
}

// ....oooO0Oooo....oooO0Oooo....oooO0Oooo....oooO0Oooo....oooO0Oooo....oooO0Oooo....oooO0Oooo....

auto
Resolver::interpret(
    Logical* expression
) -> ErrorOr<Object> {
    TRY(expression->left_expression->visit(this));
    TRY(expression->right_expression->visit(this));
    return Object(nil);
}

// ....oooO0Oooo....oooO0Oooo....oooO0Oooo....oooO0Oooo....oooO0Oooo....oooO0Oooo....oooO0Oooo....

auto
Resolver::interpret(
    Unary* expression
) -> ErrorOr<Object> {
    TRY(expression->right_expression->visit(this));
    return Object(nil);
}

// ....oooO0Oooo....oooO0Oooo....oooO0Oooo....oooO0Oooo....oooO0Oooo....oooO0Oooo....oooO0Oooo....

auto
Resolver::interpret(
    Variable* expression 
) -> ErrorOr<Object> {
    auto scope = !m_scopes.empty() ? m_scopes.front().get() : nullptr;
    if (scope) 
        if (auto itr = scope->find(expression->name.lexeme); itr != scope->end())
            if (itr->second == false)
                return ErrorType::BAD_ENVIRONMENT;
    resolve_local(expression, expression->name);
    return Object(nil);
}

// ....oooO0Oooo....oooO0Oooo....oooO0Oooo....oooO0Oooo....oooO0Oooo....oooO0Oooo....oooO0Oooo....

auto
Resolver::interpret(
    Block* statements
) -> ErrorOr<void> {
    begin_scope();
    TRY(interpret(statements->statements));
    end_scope();
    return {};
}

// ....oooO0Oooo....oooO0Oooo....oooO0Oooo....oooO0Oooo....oooO0Oooo....oooO0Oooo....oooO0Oooo....

auto
Resolver::interpret(
    ExpressionStmt* statement
) -> ErrorOr<void> {
    TRY(statement->expression->visit(this));
    return {};
}

// ....oooO0Oooo....oooO0Oooo....oooO0Oooo....oooO0Oooo....oooO0Oooo....oooO0Oooo....oooO0Oooo....

auto
Resolver::interpret(
    FunDeclStmt* statement
) -> ErrorOr<void> {
    TRY(declare(*statement->name.get()));
    define(*statement->name.get());
    TRY(resolve_function(statement, FunctionType::FUNCTION));
    return {};
}

// ....oooO0Oooo....oooO0Oooo....oooO0Oooo....oooO0Oooo....oooO0Oooo....oooO0Oooo....oooO0Oooo....

auto
Resolver::interpret(
    IfStmt* statement
) -> ErrorOr<void> {
    TRY(statement->condition->visit(this));
    TRY(statement->then_branch->visit(this));
    if (statement->else_branch) TRY(statement->else_branch->visit(this));
    return {};
}

// ....oooO0Oooo....oooO0Oooo....oooO0Oooo....oooO0Oooo....oooO0Oooo....oooO0Oooo....oooO0Oooo....

auto
Resolver::interpret(
    PrintStmt* statement
) -> ErrorOr<void> {
    TRY(statement->expression->visit(this));
    return {};
}

// ....oooO0Oooo....oooO0Oooo....oooO0Oooo....oooO0Oooo....oooO0Oooo....oooO0Oooo....oooO0Oooo....

auto
Resolver::interpret(
    ReturnStmt* statement
) -> ErrorOr<void> {
    if (m_current_function == FunctionType::NONE)
        return ErrorType::NO_RETURN_POSSIBLE;
    if (statement->value)
        TRY(statement->value->visit(this));
    return {};
}

// ....oooO0Oooo....oooO0Oooo....oooO0Oooo....oooO0Oooo....oooO0Oooo....oooO0Oooo....oooO0Oooo....

auto
Resolver::interpret(
    VarDeclStmt* statement
) -> ErrorOr<void> {
    TRY(declare(statement->name));
    if (statement->expression)
        TRY(statement->expression->visit(this));
    define(statement->name);
    return {};
}

// ....oooO0Oooo....oooO0Oooo....oooO0Oooo....oooO0Oooo....oooO0Oooo....oooO0Oooo....oooO0Oooo....

auto
Resolver::interpret(
    WhileStmt* statement
) -> ErrorOr<void> {
    TRY(statement->condition->visit(this));
    TRY(statement->body->visit(this));
    return {};
}

// ....oooO0Oooo....oooO0Oooo....oooO0Oooo....oooO0Oooo....oooO0Oooo....oooO0Oooo....oooO0Oooo....

auto
Resolver::interpret(
    std::vector<std::unique_ptr<Statement>> const& statements
) -> ErrorOr<void> {
    for (auto const& statement : statements)
        TRY(statement->visit(this));
    return {};
}

// ....oooO0Oooo....oooO0Oooo....oooO0Oooo....oooO0Oooo....oooO0Oooo....oooO0Oooo....oooO0Oooo....

auto
Resolver::resolve_local(
    Expression* expression,
    Token const& name
) -> void {
    // Note: The Java Stack class makes index 0 the bottom of the stack, while in the std::deque
    // class, the the zero index refers to the top of index.
    // Hence, instead of size - n - 1, we us n. Equivalently, the inner-most scope is at index 0
    for (std::size_t n = 0; n < m_scopes.size(); ++n) {
        auto const& scope = m_scopes[n].get();
        if (scope->find(name.lexeme) != scope->end()) {
            m_interpreter->resolve(expression, n);
            return;
        }
    }
    return;
}

// ....oooO0Oooo....oooO0Oooo....oooO0Oooo....oooO0Oooo....oooO0Oooo....oooO0Oooo....oooO0Oooo....

auto
Resolver::resolve_function(
    FunDeclStmt* statement,
    FunctionType type
) -> ErrorOr<void> {
    FunctionType enclosing_function = m_current_function;
    m_current_function = type;
    begin_scope();
    for (auto const& parameter : statement->parameters) {
        TRY(declare(*parameter.get()));
        define(*parameter.get());
    }
    TRY(interpret(statement->body));
    end_scope();
    m_current_function = enclosing_function;
    return {};
}

// ....oooO0Oooo....oooO0Oooo....oooO0Oooo....oooO0Oooo....oooO0Oooo....oooO0Oooo....oooO0Oooo....

auto
Resolver::begin_scope(
) -> void {
    m_scopes.push_front(std::make_unique<Scope>());
}

// ....oooO0Oooo....oooO0Oooo....oooO0Oooo....oooO0Oooo....oooO0Oooo....oooO0Oooo....oooO0Oooo....

auto
Resolver::end_scope(
) -> void {
    m_scopes.pop_front();
}

// ....oooO0Oooo....oooO0Oooo....oooO0Oooo....oooO0Oooo....oooO0Oooo....oooO0Oooo....oooO0Oooo....

auto
Resolver::declare(
    Token const& name
) -> ErrorOr<void> {
    if (m_scopes.empty()) return{};
    Scope* scope = m_scopes.front().get();
    if (scope->find(name.lexeme) !=  scope->end())
        return ErrorType::VARIABLE_ALREADY_EXISTS;
    scope->insert({name.lexeme, false});
    return {};
}

// ....oooO0Oooo....oooO0Oooo....oooO0Oooo....oooO0Oooo....oooO0Oooo....oooO0Oooo....oooO0Oooo....

auto
Resolver::define(
    Token const& name
) -> void {
    if (m_scopes.empty()) return;
    m_scopes.front().get()->at(name.lexeme) = true;
}