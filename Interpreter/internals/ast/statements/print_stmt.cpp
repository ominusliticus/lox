#include "internals/ast/statements/print_stmt.hpp"


PrintStmt::PrintStmt(
    std::shared_ptr<Expression> expression
)
    : Statement(expression, StatementType::PRINT)
{}
