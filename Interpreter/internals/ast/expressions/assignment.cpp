#include "internals/ast/expressions/assignment.hpp"

#include "util/try.hpp"

Assignment::Assignment(
    Token&&                     name_,
    std::unique_ptr<Expression> value_
) 
    : Expression({}, {}, {}, ExpressionType::ASSIGNMENT)
    , name{ std::move(name_) }
    , value{ std::move(value_) }
{}

auto
Assignment::visit(
    Interpreter* interpreter
) -> ErrorOr<Object> {
    return TRY(interpreter->interpret(this));
}
