#include "internals/ast/statements/expression_stmt.hpp"

ExpressionStmt::ExpressionStmt(
    std::shared_ptr<Expression> expression
) 
    : Statement(expression, StatementType::EXPRESSION)
{}
