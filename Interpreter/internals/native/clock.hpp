#pragma once

#include "internals/object.hpp"
#include "internals/ast/ast_forward.hpp"
#include "internals/ast/expressions/call.hpp"


namespace native {

struct Clock : public Call {
    Clock();

    virtual ~Clock() = default;
    
    virtual ErrorOr<Object> operator()(
        Interpreter* interpreter,
        std::vector<Object>&& values
    ) override;
};

} // native
