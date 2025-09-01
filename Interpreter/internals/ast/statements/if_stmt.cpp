#include "internals/ast/statements/if_stmt.hpp"

IfStmt::IfStmt(
    std::shared_ptr<Expression> condition_,
    std::shared_ptr<Statement> then_branch_,
    std::shared_ptr<Statement> else_branch_
) 
    : Statement({}, StatementType::IF)
    , condition(condition_)
    , then_branch(then_branch_)
    , else_branch(else_branch_)
{}
