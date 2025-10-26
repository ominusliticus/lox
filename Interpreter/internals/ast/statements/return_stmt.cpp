#include "internals/ast/statements/return_stmt.hpp"

#include "internals/ast/walkers/interpreter.hpp"

#include "internals/ast/expression.hpp"

#include "util/try.hpp"

ReturnStmt::ReturnStmt(
    std::unique_ptr<Token> keyword_,
    std::unique_ptr<Expression> value_
) 
    : Statement({}, StatementType::RETURN)
    , keyword(std::move(keyword_))
    , value(std::move(value_))
{}

auto
ReturnStmt::visit(
    Interpreter* interpreter
) -> ErrorOr<void> {
    TRY(interpreter->interpret(this));
    return {};
}
