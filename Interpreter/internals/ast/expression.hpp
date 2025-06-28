#pragma once

#include <memory>

#include "internals/ast/expression_type.hpp"

#include "internals/token.hpp"

struct Expression {
    Expression(Expression&& expression);
    Expression(
        std::shared_ptr<Expression> left_expression_,
        std::shared_ptr<Token>      operation_,
        std::shared_ptr<Expression> right_expression_,
        ExpressionType              expression_type_
    );

    std::shared_ptr<Expression> left_expression;
    std::shared_ptr<Token>      operation;
    std::shared_ptr<Expression> right_expression;
    ExpressionType              expression_type;
};
