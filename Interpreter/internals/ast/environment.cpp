#include "util/try.hpp"

#include "internals/ast/environment.hpp"

auto
Environment::define(
    std::string const& name,
    Object&& value
) -> void {
    m_values[name] = std::move(value);
}

auto
Environment::assign(
    std::string const& name,
    Object&& value
) -> ErrorOr<void> {
    if (m_values.find(name) != m_values.end()) {
        m_values[name] = std::move(value);
        return {};
    }
    return ErrorType::UNKNOWN_IDENTIFIER;
}

auto
Environment::get(
    Token const& name
) const -> ErrorOr<Object> {
    auto itr = m_values.find(name.lexeme);
    if (itr != m_values.end())
        return itr->second;

    return ErrorType::UNKNOWN_IDENTIFIER;
}
