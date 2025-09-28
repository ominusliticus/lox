#include "internals/ast/expressions/grouping.hpp"

#include "util/try.hpp"

Grouping::Grouping(
    std::unique_ptr<Expression> expression_
) 
    : Expression(std::move(expression_), {}, {}, ExpressionType::GROUPING)
{}

auto
Grouping::visit(
    Interpreter *interpreter
) -> ErrorOr<Object> {
    return TRY(interpreter->interpret(this));
}
