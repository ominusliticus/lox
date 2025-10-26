#pragma once

#include "internals/ast/statement.hpp"
#include "internals/object.hpp"


struct ReturnStmt : public Statement {
    ReturnStmt(std::unique_ptr<Token> keyword_, std::unique_ptr<Expression> value_);

    virtual ErrorOr<void> visit(Interpreter* interpreter) override;

    std::unique_ptr<Token> keyword;
    std::unique_ptr<Expression> value;
};
