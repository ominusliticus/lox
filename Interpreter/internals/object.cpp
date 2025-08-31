#include "internals/object.hpp"


Object::Object(
    double const& literal_
) 
    : literal{ literal_ }
{}

Object::Object(
    std::string const& literal_
) 
    : literal{ literal_ }
{}

Object::Object(
    bool const& literal_
) 
    : literal{ literal_ }
{}

Object::Object(
    Nil const& literal_
) 
    : literal{ literal_ }
{}

Object::Object(
    Object const& other
) 
    : literal{ other.literal }
{}

auto
Object::operator=(
    Object const& other
) -> Object& {
    literal = other.literal;
    return *this;
}

auto
operator==(
    Object const& right,
    Object const& left
) -> bool {
    return right.literal == left.literal;
}
