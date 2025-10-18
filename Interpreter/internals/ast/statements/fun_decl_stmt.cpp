
#include "internals/ast/statements/fun_decl_stmt.hpp"

#include "util/try.hpp"

#include "internals/ast/walkers/interpreter.hpp"

FunDeclStmt::FunDeclStmt(
    std::unique_ptr<Token> name_, 
    std::vector<std::unique_ptr<Token>> parameters_,
    std::vector<std::unique_ptr<Statement>> body_
) 
    : Statement({}, StatementType::FUN_DECL)
    , name(std::move(name_))
    , parameters(std::move(parameters_))
    , body(std::move(body_))
{}

auto
FunDeclStmt::visit(
    Interpreter* interpreter
) -> ErrorOr<void> {
    return TRY(interpreter->interpret(this));
}
