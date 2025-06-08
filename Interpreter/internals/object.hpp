#pragma once

#include <iostream>
#include <string>
#include <variant>

#include "internals/nil.hpp"

// TODO: Need something that stores a Lox class type

struct Object {
    Object(
    ) 
        : literal{}
    {}

    explicit
    Object(
        double literal_
    ) 
        : literal{ literal_ }
    {}

    explicit
    Object(
        std::string literal_
    ) 
        : literal{ literal_ }
    {}

    explicit
    Object(
        bool literal_
    ) 
        : literal{ literal_ }
    {}

    explicit
    Object(
        Nil literal_
    ) 
        : literal{ literal_ }
    {}

    Object(
        Object const& other
    ) 
        : literal{ other.literal }
    {}

    std::variant<double, std::string, bool, Nil> literal;
};

inline auto
operator==(
    Object const& right,
    Object const& left
) -> bool {
    return right.literal == left.literal;
}

template<typename OStream>
auto
operator<<(
    OStream& ostream,
    Object& object
) -> OStream& {
    if (std::get_if<double>(&object.literal))
        ostream << std::get<double>(object.literal);
    else if (std::get_if<std::string>(&object.literal))
        ostream << std::get<std::string>(object.literal);
    else if (std::get_if<bool>(&object.literal))
        ostream << std::get<bool>(object.literal);
    else if (std::get_if<Nil>(&object.literal))
        ostream << "nil";
    return ostream;
}

template<typename OStream>
auto
operator<<(
    OStream& ostream,
    Object const& object
) -> OStream& {
    if (std::get_if<double>(&object.literal))
        ostream << std::get<double>(object.literal);
    else if (std::get_if<std::string>(&object.literal))
        ostream << std::get<std::string>(object.literal);
    else if (std::get_if<bool>(&object.literal))
        ostream << std::get<bool>(object.literal);
    else if (std::get_if<Nil>(&object.literal))
        ostream << "nil";
    return ostream;
}
