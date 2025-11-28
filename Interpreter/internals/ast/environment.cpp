#include "util/error.hpp"
#include "util/print.hpp"
#include "util/try.hpp"
#include <cstddef>

#include "internals/ast/environment.hpp"

Environment::Environment(
    std::string const& name
) 
    : m_enclosing(nullptr)
    , m_recursion_depth(0)
    , m_name(name)
{}

// .....oooO0Oooo.....oooO0Oooo.....oooO0Oooo.....oooO0Oooo.....oooO0Oooo.....oooO0Oooo.....

Environment::Environment(
    Environment* enclosing,
    std::string const& name
) 
    : m_enclosing(enclosing)
    , m_recursion_depth((enclosing->m_recursion_depth) + 1)
    , m_name(enclosing->m_name + " -> (" + name + ")" + std::to_string(m_recursion_depth))
{}

// .....oooO0Oooo.....oooO0Oooo.....oooO0Oooo.....oooO0Oooo.....oooO0Oooo.....oooO0Oooo.....

auto
Environment::values(
) -> Environment::VariableMap const& {
    return m_values;
}

// .....oooO0Oooo.....oooO0Oooo.....oooO0Oooo.....oooO0Oooo.....oooO0Oooo.....oooO0Oooo.....

auto
Environment::functions(
) -> Environment::FunctionMap const& {
    return m_functions;
}

// .....oooO0Oooo.....oooO0Oooo.....oooO0Oooo.....oooO0Oooo.....oooO0Oooo.....oooO0Oooo.....

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
Environment::assign_at(
    int distance,
    Token const& name,
    Object&& value
) -> ErrorOr<void> {
    TRY(ancestor(distance))->m_values[name.lexeme] = std::move(value);
    return {};
}

// .....oooO0Oooo.....oooO0Oooo.....oooO0Oooo.....oooO0Oooo.....oooO0Oooo.....oooO0Oooo.....

auto
Environment::ancestor(
    int distance
) const -> ErrorOr<Environment*>  {
    Environment* env = const_cast<Environment*>(this);
    for (int i = 0; i < distance; ++i)
        env = env->enclosing();
    if (env) return env;
    return ErrorType::BAD_ENVIRONMENT;
}

// .....oooO0Oooo.....oooO0Oooo.....oooO0Oooo.....oooO0Oooo.....oooO0Oooo.....oooO0Oooo.....

auto
Environment::get(
    Token const& name
) const -> ErrorOr<Object> {
    auto itr = m_values.find(name.lexeme);
    if (itr != m_values.end())
        return ErrorOr<Object>(itr->second);
    if (m_enclosing) 
        return m_enclosing->get(name);

    return ErrorType::UNKNOWN_IDENTIFIER;
}

// .....oooO0Oooo.....oooO0Oooo.....oooO0Oooo.....oooO0Oooo.....oooO0Oooo.....oooO0Oooo.....

auto
Environment::get_function(
    Token const& name
) const -> ErrorOr<Call*> {
    // print("(function)", name.lexeme, ":", m_name);
    auto itr = m_functions.find(name.lexeme);
    if (itr != m_functions.end())
        return ErrorOr<Call*>(itr->second.get());
    if (m_enclosing)
        return m_enclosing->get_function(name);
    return ErrorType::UNKNOWN_IDENTIFIER;
}

// .....oooO0Oooo.....oooO0Oooo.....oooO0Oooo.....oooO0Oooo.....oooO0Oooo.....oooO0Oooo.....

auto
Environment::get_at(
    int distance,
    std::string const& name
) const -> ErrorOr<Object> {
    auto env = TRY(ancestor(distance));
    auto values = env->m_values;
    if (auto itr = values.find(name); itr != values.end())
        return itr->second;
    
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

// .....oooO0Oooo.....oooO0Oooo.....oooO0Oooo.....oooO0Oooo.....oooO0Oooo.....oooO0Oooo.....

auto
Environment::count(
) -> std::size_t { 
    return m_values.size() + m_functions.size(); 
}

// .....oooO0Oooo.....oooO0Oooo.....oooO0Oooo.....oooO0Oooo.....oooO0Oooo.....oooO0Oooo.....

auto
Environment::depth(
) -> int { 
    return m_recursion_depth;
}

// .....oooO0Oooo.....oooO0Oooo.....oooO0Oooo.....oooO0Oooo.....oooO0Oooo.....oooO0Oooo.....

auto
Environment::inspect(
) const -> void {
    print("Environment:", m_recursion_depth);
    print("Variables");
    if (m_values.size() > 0)
        for (auto const& [key, value] : m_values)
            print(key, value);

    print("Functions");
    if (m_functions.size() > 0)
        for (auto const& [key, func] : m_functions)
            print(key, func);

    if (m_enclosing)
        m_enclosing->inspect();
}