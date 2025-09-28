#include "internals/ast/statement.hpp"

// Statement::Statement(
//     Statement&& statement_
// )
//     : expression{ std::move(statement_.expression) }
//     , statement_type{ std::move(statement_.statement_type) }
// {}

Statement::Statement(
    std::unique_ptr<Expression> expression_,
    StatementType               statement_type_
)
    : expression{ std::move(expression_) }
    , statement_type{ statement_type_ }
{}
