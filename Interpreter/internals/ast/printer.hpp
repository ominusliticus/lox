#pragma once

#include <sstream>
#include <memory>

#include "internals/ast/expression.hpp"
#include "internals/ast/expression_type.hpp"
#include "internals/ast/literal.hpp"

namespace ast {

auto
printer(
    std::stringstream&         string_buffer,
    std::shared_ptr<Expression> expression
) -> void;

}


// TODO: Add printer for Statments
