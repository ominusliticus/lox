#pragma once

#include <iostream>
#include <string>
#include <variant>

#include "internals/nil.hpp"


struct Object {
    Object() = default; 

    explicit Object(double const& literal_);
    explicit Object(std::string const& literal_); 
    explicit Object(bool const& literal_); 
    explicit Object(Nil const& literal_); 

    Object(Object const& other); 

    Object& operator=(Object const& other);

    std::variant<double, std::string, bool, Nil> literal;
};

bool operator==(Object const& right, Object const& left);

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
