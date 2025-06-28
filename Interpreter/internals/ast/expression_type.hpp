#pragma once

#include <cstdint>

enum class ExpressionType : std::uint8_t {
    BINARY,
    GROUPING,
    LITERAL,
    UNARY,
    VARIABLE
};

