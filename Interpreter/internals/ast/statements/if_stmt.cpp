#include "internals/ast/statements/if_stmt.hpp"

#include "util/try.hpp"

IfStmt::IfStmt(
    std::unique_ptr<Expression> condition_,
    std::unique_ptr<Statement> then_branch_,
    std::unique_ptr<Statement> else_branch_
) 
    : Statement({}, StatementType::IF)
    , condition(std::move(condition_))
    , then_branch(std::move(then_branch_))
    , else_branch(std::move(else_branch_))
{}

auto
IfStmt::visit(
    Interpreter* interpreter
) -> ErrorOr<void> {
    return TRY(interpreter->interpret(this));
}
