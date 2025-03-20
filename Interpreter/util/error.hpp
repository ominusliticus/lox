# pragma once

#include <concepts>
#include <iostream>
#include <string_view>
#include <utility>

enum class ErrorType {
    FILE_NOT_FOUND,
    UNREACHABLE,
    INVALID_COMMAND,
    INVALID_SYMBOL,
    UNTERMINATED_STRING
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
        case ErrorType::UNREACHABLE:
            ostream << "Line unreachable";
            break;
        case ErrorType::INVALID_COMMAND:
            ostream << "Invalid command";
            break;
        case ErrorType::INVALID_SYMBOL:
            ostream << "Invalid symbol";
            break;
        case ErrorType::UNTERMINATED_STRING:
            ostream << "Unterminated string";
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


// TODO: Add file to error reporting
inline auto
error(
    int       line,
    int       column,
    ErrorType error_type
) -> void {
    std::cout << "\033[31m" << "[Error]" << " "
              << line << ":" 
              << column << " "
              << error_type <<  "\033[0m" << std::endl;
}

inline auto
error(
    std::string_view file,
    int              line,
    int              column,
    ErrorType        error_type
) -> void {
    std::cout << "\033[31m" << "[Error]" << " "
              << file << ":"
              << line << ":" 
              << column << " "
              << error_type <<  "\033[0m" << std::endl;
}
