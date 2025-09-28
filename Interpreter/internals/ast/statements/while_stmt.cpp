#include "internals/ast/statements/while_stmt.hpp"

#include "util/try.hpp"

WhileStmt::WhileStmt(std::unique_ptr<Expression> condition_, std::unique_ptr<Statement> body_)
    : Statement({}, StatementType::WHILE)
    , condition{ std::move(condition_) }
    , body{ std::move(body_) }
{}

auto
WhileStmt::visit(
    Interpreter* interpreter
) -> ErrorOr<void> {
    return TRY(interpreter->interpret(this));
}
