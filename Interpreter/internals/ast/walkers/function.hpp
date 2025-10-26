#pragma once

#include <vector>

#include "internals/ast/expressions/call.hpp"
#include "internals/ast/statements/fun_decl_stmt.hpp"

struct Function : public Call {
    Function(FunDeclStmt* declaration, Environment* closure);

    virtual ErrorOr<Object> operator()(
        Interpreter* interpreter,
        std::vector<Object>&& values
    ) override;

    int arity() const { return m_declaration->parameters.size(); }
    std::string to_string() { return "<fn " + m_declaration->name->lexeme + ">"; }

private:
    FunDeclStmt* m_declaration;
    Environment* m_closure;
};
