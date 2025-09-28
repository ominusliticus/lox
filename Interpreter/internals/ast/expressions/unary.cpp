#include "internals/ast/expressions/unary.hpp"

#include "util/try.hpp"

Unary::Unary(
    std::unique_ptr<Token>      operation_,
    std::unique_ptr<Expression> right_expression_
) 
    : Expression({}, std::move(operation_), std::move(right_expression_), ExpressionType::UNARY)
{}

auto
Unary::visit(
    Interpreter* interpreter
) -> ErrorOr<Object> {
    return TRY(interpreter->interpret(this));
}
