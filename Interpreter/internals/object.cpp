#include "internals/object.hpp"


Object::Object(
    double literal_
) 
    : literal{ literal_ }
{}

Object::Object(
    std::string literal_
) 
    : literal{ literal_ }
{}

Object::Object(
    bool literal_
) 
    : literal{ literal_ }
{}

Object::Object(
    Nil literal_
) 
    : literal{ literal_ }
{}

Object::Object(
    Object const& other
) 
    : literal{ other.literal }
{}


auto
operator==(
    Object const& right,
    Object const& left
) -> bool {
    return right.literal == left.literal;
}
