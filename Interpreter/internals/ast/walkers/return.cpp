#include "internals/ast/walkers/return.hpp"

Return::Return(
    Object&& value
) 
    : m_msg("LOX RETURN")
    , m_value(std::move(value))
{}

auto
Return::what(
) const noexcept -> const char* {
    return m_msg.c_str();
}

auto
Return::value(
) -> Object& {
    return m_value;
}
