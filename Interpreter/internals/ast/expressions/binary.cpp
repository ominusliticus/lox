#include "internals/ast/expressions/binary.hpp"

#include "util/try.hpp"

Binary::Binary(
    std::unique_ptr<Expression> left_expression_,
    std::unique_ptr<Token>      operation_,
    std::unique_ptr<Expression> right_expression_
) 
    : Expression(
        std::move(left_expression_), 
        std::move(operation_), 
        std::move(right_expression_),
        ExpressionType::BINARY
    )
{}


auto
Binary::visit(
    Interpreter* interpreter
) -> ErrorOr<Object> {
    return TRY(interpreter->interpret(this));
}
