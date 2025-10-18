#include "util/try.hpp"

#include "internals/ast/environment.hpp"

auto
Environment::define(
    std::string const& name,
    Object&& value
) -> void {
    m_values.insert({name, std::move(value)});
}

// .....oooO0Oooo.....oooO0Oooo.....oooO0Oooo.....oooO0Oooo.....oooO0Oooo.....oooO0Oooo.....

auto
Environment::define(
    std::string const& name,
    std::unique_ptr<Call> value
) -> void {
    m_functions.insert({name, std::move(value)});
}

// .....oooO0Oooo.....oooO0Oooo.....oooO0Oooo.....oooO0Oooo.....oooO0Oooo.....oooO0Oooo.....

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

// .....oooO0Oooo.....oooO0Oooo.....oooO0Oooo.....oooO0Oooo.....oooO0Oooo.....oooO0Oooo.....

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

// .....oooO0Oooo.....oooO0Oooo.....oooO0Oooo.....oooO0Oooo.....oooO0Oooo.....oooO0Oooo.....

auto
Environment::get_function(
    Token const& name
) const -> ErrorOr<Call*> {
    auto itr = m_functions.find(name.lexeme);
    if (itr != m_functions.end())
        return ErrorOr<Call*>(std::move(itr->second.get()));
    if (m_enclosing) 
        return m_enclosing->get_function(name);

    return ErrorType::UNKNOWN_IDENTIFIER;
}

// .....oooO0Oooo.....oooO0Oooo.....oooO0Oooo.....oooO0Oooo.....oooO0Oooo.....oooO0Oooo.....

auto
Environment::check_names(
    Token const& name
) const -> std::string {
    bool in_variables = m_values.find(name.lexeme) != m_values.end();
    bool in_functions = m_functions.find(name.lexeme) != m_functions.end();
    if (!in_variables && !in_functions) {
        if (m_enclosing) return m_enclosing->check_names(name);
        else return "none";
    }
    else if (in_variables)  return "variables";
    else return "functions";
}

// .....oooO0Oooo.....oooO0Oooo.....oooO0Oooo.....oooO0Oooo.....oooO0Oooo.....oooO0Oooo.....

auto Environment::enclosing(
) -> Environment* {
    return m_enclosing;
}
