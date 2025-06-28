#include "internals/ast/var_decl_stmt.hpp"

VarDeclStmt::VarDeclStmt(
    Token const&                name_,
    std::shared_ptr<Expression> expression
)
    : Statement(expression, StatementType::VAR_DECL)
    , name{ name_ }
{}
