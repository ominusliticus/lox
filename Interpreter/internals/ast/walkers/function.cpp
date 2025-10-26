#include "internals/ast/walkers/function.hpp"

#include <memory>

#include "internals/nil.hpp"

#include "internals/ast/environment.hpp"

#include "util/try.hpp"

Function::Function(
    FunDeclStmt* declaration,
    Environment* closure
) 
    : Call({}, {}, {})
    , m_declaration(declaration)
    , m_closure(closure)
{}


auto
Function::operator()(
    Interpreter* interpreter,
    std::vector<Object>&& values
) -> ErrorOr<Object> {
    std::unique_ptr<Environment> new_environment = std::make_unique<Environment>(
        m_closure,
        "FUNCTION:" + m_declaration->name->lexeme
    );
    for (std::size_t n = 0; n < values.size(); ++n)
        new_environment->define(m_declaration->parameters[n]->lexeme, std::move(values[n]));
    TRY(interpreter->execute_block(std::move(m_declaration->body), new_environment.get()));
    return Object(nil);
}
