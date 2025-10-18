#include "internals/ast/expressions/call.hpp"

#include "util/try.hpp"

#include <stdexcept>

Call::Call(
    std::unique_ptr<Expression> callee_,
    std::unique_ptr<Token> paren_,
    std::vector<std::unique_ptr<Expression>>&& arguments_
) 
    : Expression({}, {}, {}, ExpressionType::CALL)
    , callee{ std::move(callee_) }
    , paren{ std::move(paren_) }
    , arguments{ std::move(arguments_) }
    , m_arity(arguments.size())
{}

auto
Call::visit(
    Interpreter* interpreter
) -> ErrorOr<Object> {
    return TRY(interpreter->interpret(this));
}

auto 
Call::operator()(
    [[maybe_unused]] Interpreter* interpreter,
    [[maybe_unused]] std::vector<Object>&& values
) -> ErrorOr<Object> {
    return ErrorType::CALL_PROHIBITED;
}
