#pragma once

#include <memory>

#include "internals/ast/expressions/expression_type.hpp"
#include "internals/ast/expression.hpp"
#include "internals/ast/walkers/interpreter.hpp"
// #include "internals/ast/walkers/printer.hpp"

#include "internals/token.hpp"

struct Variable : public Expression {
    Variable(std::unique_ptr<Token> name_); 

    ErrorOr<Object> visit(Interpreter* interpreter) final;
    // ErrorOr<void>   visit(Printer const& printer);

    Token name;
};

