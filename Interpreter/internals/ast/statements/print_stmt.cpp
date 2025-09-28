#include "internals/ast/statements/print_stmt.hpp"

#include "util/try.hpp"

PrintStmt::PrintStmt(
    std::unique_ptr<Expression> expression
)
    : Statement(std::move(expression), StatementType::PRINT)
{}

auto
PrintStmt::visit(
    Interpreter* interpreter
) -> ErrorOr<void> {
    return TRY(interpreter->interpret(this));
}
