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
    if (m_enclosing)
        return m_enclosing->assign(name, std::move(value));

    return ErrorType::UNKNOWN_IDENTIFIER;
}

auto
Environment::get(
    Token const& name
) const -> ErrorOr<Object> {
    auto itr = m_values.find(name.lexeme);
    if (itr != m_values.end())
        return ErrorOr<Object>(std::move(itr->second));
    if (m_enclosing) 
        return m_enclosing->get(name);

    return ErrorType::UNKNOWN_IDENTIFIER;
}

auto Environment::enclosing(
) && -> std::unique_ptr<Environment> {
    return std::move(m_enclosing);
}
