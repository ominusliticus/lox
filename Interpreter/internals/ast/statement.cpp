#include "internals/ast/statement.hpp"

Statement::Statement(
    Statement&& statement_
)
    : expression{ statement_.expression }
    , statement_type{ statement_.statement_type }
{}

Statement::Statement(
    std::shared_ptr<Expression> expression_,
    StatementType               statement_type_
)
    : expression{ expression_ }
    , statement_type{ statement_type_ }
{}
