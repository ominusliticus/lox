#pragma once

#include <memory>
#include <unordered_map>
#include <string>

#include "util/error.hpp"

#include "internals/object.hpp"
#include "internals/token.hpp"

class Environment {
public:
    Environment() = default;

    void define(std::string const& name, Object&& value);

    ErrorOr<void> assign(std::string const& name, Object&& value);
    
    // Design decision here, lots of copying will happen whenever we get an Object, but that is 
    // better than proliferating the use of std::shared_ptr<Object> everywhere, it's unnecessary.
    // Performance optimization/profiling can be accomplished once everything is done
    ErrorOr<Object> get(Token const& name) const;

private:
    std::unordered_map<std::string, Object> m_values;
};
