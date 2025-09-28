#include "internals/ast/expressions/variable.hpp"

#include "util/try.hpp"

Variable::Variable(
    std::unique_ptr<Token> name_
) 
    : Expression({}, {}, {}, ExpressionType::VARIABLE)
    , name{ *name_ }
{}

auto
Variable::visit(
    Interpreter* interpreter
) -> ErrorOr<Object> {
    return TRY(interpreter->interpret(this));
}
