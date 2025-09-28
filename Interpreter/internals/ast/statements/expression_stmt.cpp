#include "internals/ast/statements/expression_stmt.hpp"

#include "util/try.hpp"

ExpressionStmt::ExpressionStmt(
    std::unique_ptr<Expression> expression
) 
    : Statement(std::move(expression), StatementType::EXPRESSION)
{}

auto
ExpressionStmt::visit(
    Interpreter* interpreter
) -> ErrorOr<void> {
    return TRY(interpreter->interpret(this));
}
