#pragma once

#include <cstdint>

enum class ExpressionType : std::uint8_t {
    BINARY,
    GROUPING,
    LITERAL,
    LOGICAL,
    UNARY,
    VARIABLE,
    ASSIGNMENT,
    CALL
};

template<typename OStream>
auto
operator<<(
    OStream& ostream,
    ExpressionType type
) -> OStream& {
    switch (type) {
        case ExpressionType::BINARY:
            ostream << "BINARY";
            break;
        case ExpressionType::GROUPING:
            ostream << "GROUPING";
            break;
        case ExpressionType::LITERAL:
            ostream << "LITERAL";
            break;
        case ExpressionType::LOGICAL:
            ostream << "LOGICAL";
            break;
        case ExpressionType::VARIABLE:
            ostream << "VARIABLE";
            break;
        case ExpressionType::UNARY:
            ostream << "UNARY";
            break;
        case ExpressionType::ASSIGNMENT:
            ostream << "ASSIGNMENT";
            break;
        case ExpressionType::CALL:
            ostream << "CALL";
            break;
    }
    return ostream;
}

