#pragma once

#include <memory>
#include <vector>
 
#include "internals/object.hpp"
#include "internals/token.hpp"

#include "internals/ast/expression.hpp"
#include "internals/ast/binary.hpp"
#include "internals/ast/expression_type.hpp"
#include "internals/ast/grouping.hpp"
#include "internals/ast/literal.hpp"
#include "internals/ast/unary.hpp"
#include "internals/ast/variable.hpp"

#include "internals/ast/statement.hpp"
#include "internals/ast/expression_stmt.hpp"
#include "internals/ast/print_stmt.hpp"
#include "internals/ast/statement_type.hpp"
#include "internals/ast/var_decl_stmt.hpp"

#include "util/error.hpp"


// TDOO: Our type system is not as convenient as the Java one.
//       We need to create a class heirarchy that allows us to automate much of the visiting down
//       the road.
// TODO: Add documentation strings


namespace ast 
{

auto
interpret(
    std::vector<std::shared_ptr<Statement>> statements
) -> ErrorOr<void>;

auto 
interpreter(
    std::shared_ptr<Statement> statement
) -> ErrorOr<void>;

auto 
interpreter(
    std::shared_ptr<Expression> expression
) -> ErrorOr<Object>;

auto
evaluate(
    std::shared_ptr<Expression> expression
) -> ErrorOr<Object>;

auto
execute(
    std::shared_ptr<Statement> statement
) -> ErrorOr<void>;

auto
is_truthy(
    Object const& obj
) -> bool;
}
