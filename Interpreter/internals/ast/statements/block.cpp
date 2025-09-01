#include "internals/ast/statements/block.hpp"


Block::Block(
    std::vector<std::shared_ptr<Statement>>&& statements_
) 
    : Statement({}, StatementType::BLOCK)
    , statements(std::move(statements_))
{}
