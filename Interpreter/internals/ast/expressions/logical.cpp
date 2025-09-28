#include "internals/ast/expressions/logical.hpp"

#include "util/try.hpp"

Logical::Logical(
    std::unique_ptr<Expression> left,
    std::unique_ptr<Token> operation, 
    std::unique_ptr<Expression> right
)
    : Expression(std::move(left), std::move(operation), std::move(right), ExpressionType::LOGICAL)
{}

auto
Logical::visit(
    Interpreter* interpreter
) -> ErrorOr<Object> {
    return TRY(interpreter->interpret(this));
}
