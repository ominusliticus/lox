#include "internals/ast/statements/var_decl_stmt.hpp"

#include "util/try.hpp"

VarDeclStmt::VarDeclStmt(
    Token const&                name_,
    std::unique_ptr<Expression> expression
)
    : Statement(std::move(expression), StatementType::VAR_DECL)
    , name{ name_ }
{}

auto
VarDeclStmt::visit(
    Interpreter* interpreter
) -> ErrorOr<void> {
    return TRY(interpreter->interpret(this));
}
