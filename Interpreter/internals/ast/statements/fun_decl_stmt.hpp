#pragma once

#include <vector>

#include "internals/ast/ast_forward.hpp"
#include "internals/ast/statement.hpp"


struct FunDeclStmt : public Statement {
    FunDeclStmt(
        std::unique_ptr<Token> name_, 
        std::vector<std::unique_ptr<Token>> parameters_,
        std::vector<std::unique_ptr<Statement>> body_
    );

    virtual ErrorOr<void> visit(Interpreter* interpreter) override;

    std::unique_ptr<Token> name;
    std::vector<std::unique_ptr<Token>> parameters;
    std::vector<std::unique_ptr<Statement>> body;
};
