#pragma once

#include <exception>

#include "internals/object.hpp"

class Return : public std::exception {
public:
    Return(Object&& value);

    virtual const char* what() const noexcept override;
    Object& value();

private:
    std::string m_msg;
    Object m_value;
};
