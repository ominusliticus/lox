#pragma once

#include <memory>
#include <vector>

#include "internals/ast/expression.hpp"
#include "internals/ast/expressions/expression_type.hpp"
#include "internals/ast/walkers/interpreter.hpp"

#include "util/error.hpp"

struct Call : public Expression {
    Call(
        std::unique_ptr<Expression> callee_,
        std::unique_ptr<Token> paren_,
        std::vector<std::unique_ptr<Expression>>&& arguments_
    );
    virtual ~Call() = default;

    virtual ErrorOr<Object> visit(Interpreter* interpreter) override;
    virtual ErrorOr<Object> operator()(Interpreter* interpreter, std::vector<Object>&& values);

    std::size_t arity() const { return m_arity; }

    std::unique_ptr<Expression> callee;
    std::unique_ptr<Token> paren;
    std::vector<std::unique_ptr<Expression>> arguments;
    std::size_t m_arity;
};

