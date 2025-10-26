#pragma once

#include <memory>
#include <unordered_map>
#include <string>
#include <variant>

#include "util/error.hpp"

#include "internals/object.hpp"
#include "internals/token.hpp"

#include "internals/ast/ast_forward.hpp"
#include "internals/ast/expressions/call.hpp"

class Environment {
public:
    using VariableMap = std::unordered_map<std::string, Object>;
    using FunctionMap = std::unordered_map<std::string, std::unique_ptr<Call>>;

    Environment() = default;
    Environment(std::string const& name);
    Environment(Environment* enclosing, std::string const& name);

    VariableMap const& values();
    FunctionMap const& functions();

    void define(std::string const& name, Object&& value);
    void define(std::string const& name, std::unique_ptr<Call> value);

    ErrorOr<void> assign(std::string const& name, Object&& value);
    
    // Design decision here, lots of copying will happen whenever we get an Object, but that is 
    // better than proliferating the use of std::shared_ptr<Object> everywhere, it's unnecessary.
    // Performance optimization/profiling can be accomplished once everything is done
    //
    // There is some thoght about lexically scope variables being kept around if used as 
    // std::shared_ptr
    ErrorOr<Object> get(Token const& name) const;
    ErrorOr<Call*>  get_function(Token const& name) const;

    std::string check_names(Token const& name) const;
    Environment* enclosing();

    std::size_t count();
    int depth();

    friend class Interpreter;

private:
    Environment* m_enclosing = nullptr;
    int m_recursion_depth = -1;
    std::string m_name;
    VariableMap  m_values;
    FunctionMap  m_functions;
};
