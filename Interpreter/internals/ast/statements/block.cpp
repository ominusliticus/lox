#include "internals/ast/statements/block.hpp"

#include "util/try.hpp"

Block::Block(
    std::vector<std::unique_ptr<Statement>>&& statements_
) 
    : Statement({}, StatementType::BLOCK)
    , statements(std::move(statements_))
{}

auto
Block::visit(
    Interpreter* interpreter
) -> ErrorOr<void> {
    return TRY(interpreter->interpret(this));
}
