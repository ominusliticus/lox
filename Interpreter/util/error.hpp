# pragma once

#include <concepts>
#include <pthread.h>

#include "print.hpp"


enum class ErrorType {
    FILE_NOT_FOUND
};

template<class OStream>
auto
operator<<(
    OStream& ostream, 
    ErrorType const& error_type
) -> OStream& {
    switch (error_type) {
        case ErrorType::FILE_NOT_FOUND:
            ostream << "File not found";
            break;
    }
    return ostream;
}

template<typename T>
class [[nodiscard]] ErrorOr {
public:
    ErrorOr() = default;
    ErrorOr(
        T&& value
    ) 
        : m_is_error{ false }
        , m_error{}
        , m_value{ value }
    {}

    ErrorOr(
        ErrorType error
    ) 
        : m_is_error{ true }
        , m_error{ error }
        , m_value{}
    {}

    template<typename U>
    requires (std::same_as<T, U>)
    ErrorOr(
        ErrorOr<U>&& other
    )
        : m_is_error{ std::move(other.m_is_error) }
        , m_error{ std::move(other.m_error) }
        , m_value{ std::move(other.m_value) }
    {}
    
    template<typename U>
    requires (std::same_as<T, U>)
    auto
    operator=(
        ErrorOr<U>&& other
    ) -> ErrorOr& { 
        m_is_error = other.m_is_error;
        m_error = other.m_error;
        m_value = other.m_value; 
        return *this;
    }

    auto
    release(
    ) & -> T& {
        return m_value;
    }

    auto
    release(
    ) const& -> T const& {
        return m_value;
    }

    auto
    release(
    ) && -> T&& {
        return std::move(m_value);
    }

    auto
    release(
    ) const&& -> T const&& {
        return std::move(m_value);
    }

    auto
    error(
    ) -> ErrorType {
        return m_error;
    }

    auto
    is_error(
    ) -> bool {
        return m_is_error;
    }

private:
    bool      m_is_error;
    ErrorType m_error;
    T&&       m_value;
};

struct Empty {};


template<>
class [[nodiscard]] ErrorOr<void> {
public:
    ErrorOr() = default;
    ErrorOr(
        Empty&& empty
    )
        : m_is_error{ false }
        , m_error{}
        , m_value{ empty}
    {}

    ErrorOr(
        ErrorType error
    )
        : m_is_error{ true }
        , m_error{ error }
        , m_value{}
    {}

    ErrorOr(
        ErrorOr<void>&& other
    )
        : m_is_error{ std::move(other.m_is_error) }
        , m_error{ std::move(other.m_error) }
        , m_value{ std::move(other.m_value) }
    {}
    
    auto
    operator=(
        ErrorOr<void>&& other
    ) -> ErrorOr& { 
        m_is_error = std::move(other.m_is_error);
        m_error = std::move(other.m_error);
        m_value = std::move(other.m_value); 
        return *this;
    }

    auto
    release(
    ) & -> Empty& {
        return m_value;
    }

    auto
    release(
    ) const& -> Empty const& {
        return m_value;
    }

    auto
    release(
    ) && -> Empty&& {
        return std::move(m_value);
    }

    auto
    release(
    ) const&& -> Empty const&& {
        return std::move(m_value);
    }

    auto
    error(
    ) -> ErrorType {
        return m_error;
    }

    auto
    is_error(
    ) -> bool {
        return m_is_error;
    }

private:
    bool      m_is_error;
    ErrorType m_error;
    Empty     m_value;
};
