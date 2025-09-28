#include "internals/ast/expressions/literal.hpp"

#include "util/try.hpp"

Literal::Literal(
    Object const& literal
) 
    : Expression({}, {}, {}, ExpressionType::LITERAL)
    , object{ literal }
{}

auto
Literal::visit(
    Interpreter* interpreter
) -> ErrorOr<Object> {
    return TRY(interpreter->interpret(this));
}
