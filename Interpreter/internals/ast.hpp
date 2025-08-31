#pragma once

#include <memory>
#include <vector>
 
#include "internals/object.hpp"
#include "internals/token.hpp"

#include "internals/ast/environment.hpp"

#include "internals/ast/expression.hpp"
#include "internals/ast/expressions/assignment.hpp"
#include "internals/ast/expressions/binary.hpp"
#include "internals/ast/expressions/expression_type.hpp"
#include "internals/ast/expressions/grouping.hpp"
#include "internals/ast/expressions/literal.hpp"
#include "internals/ast/expressions/unary.hpp"
#include "internals/ast/expressions/variable.hpp"

#include "internals/ast/statement.hpp"
#include "internals/ast/statements/expression_stmt.hpp"
#include "internals/ast/statements/print_stmt.hpp"
#include "internals/ast/statements/statement_type.hpp"
#include "internals/ast/statements/var_decl_stmt.hpp"

#include "util/error.hpp"


// TDOO: Our type system is not as convenient as the Java one.
//       We need to create a class heirarchy that allows us to automate much of the visiting down
//       the road.
// TODO: Add documentation strings


class AST
{
public:
    AST() = default;

    ErrorOr<void> interpret(std::vector<std::shared_ptr<Statement>> statements);
    ErrorOr<void> interpreter(std::shared_ptr<Statement> statement);
    ErrorOr<void> execute(std::shared_ptr<Statement> statement);
    ErrorOr<void> assign(Token const& name, Object&& value);
    
    ErrorOr<Object> interpreter(std::shared_ptr<Expression> expression);
    ErrorOr<Object> evaluate(std::shared_ptr<Expression> expression);
    
    bool is_truthy(Object const& obj);
    
private:
    static std::unique_ptr<Environment> m_environment;
};
