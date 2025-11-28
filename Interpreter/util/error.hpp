# pragma once

#include <concepts>
#include <iostream>
#include <string_view>
#include <type_traits>
#include <utility>

// TODO: The error class could benefit from some concept magic.
//       See AK::ErrorOr implementation

enum class ErrorType {
    FILE_NOT_FOUND,
    UNREACHABLE,
    INVALID_COMMAND,
    INVALID_SYMBOL,
    UNTERMINATED_STRING,
    CLOSING_PAREN,
    EXPECTED_EXPRESSION,
    IMPROPER_UNARY_NEGATION,
    UNMATCHED_BINARY_OP_TYPES,
    UNIMPLEMENTED,
    DIV_BY_ZERO,
    EXPECTED_SEMICOLON,
    EXPECTED_STATEMEMT,
    EXPECTED_VARIABLE_NAME,
    UNKNOWN_IDENTIFIER,
    EXPECTED_ASSIGNMENT_TARGET,
    EXPECTED_RIGHT_BRACE,
    EXPECTED_LEFT_BRACE,
    EXPECTED_LEFT_PAREN_IF,
    EXPECTED_RIGHT_PAREN_IF,
    EXPECTED_LEFT_PAREN_FOR,
    EXPECTED_RIGHT_PAREN_FOR,
    EXPECTED_LEFT_PAREN_WHILE,
    EXPECTED_RIGHT_PAREN_WHILE,
    EXPECTED_LEFT_PAREN_FUN,
    EXPECTED_RIGHT_PAREN_FUN,
    EXCESSIVE_ARGUMENTS,
    EXPECTED_FUNCTION_NAME,
    EXPECTED_PARAMETER_NAME,
    CALL_PROHIBITED,
    BAD_ENVIRONMENT,
    VARIABLE_ALREADY_EXISTS,
    NO_RETURN_POSSIBLE,
    END_OF_ERRORS
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
        case ErrorType::CLOSING_PAREN:
            ostream << "Expected \")\" after expression";
            break;
        case ErrorType::EXPECTED_EXPRESSION:
            ostream << "Expected expression";
            break;
        case ErrorType::IMPROPER_UNARY_NEGATION:
            ostream << "\"-\" operator can only act on Numbers";
            break;
        case ErrorType::UNMATCHED_BINARY_OP_TYPES:
            ostream << "Left and right types must match for binary operation";
            break;
        case ErrorType::UNIMPLEMENTED:
            ostream << "Unimplemented";
            break;
        case ErrorType::DIV_BY_ZERO:
            ostream << "Division by zero is not permitted";
            break;
        case ErrorType::EXPECTED_SEMICOLON:
            ostream << "Expected \";\"";
            break;
        case ErrorType::EXPECTED_STATEMEMT:
            ostream << "Expected statement";
            break;
        case ErrorType::EXPECTED_VARIABLE_NAME:
            ostream << "Expected variable name";
            break;
        case ErrorType::UNKNOWN_IDENTIFIER:
            ostream << "Unknown identifier";
            break;
        case ErrorType::EXPECTED_ASSIGNMENT_TARGET:
            ostream << "Expected assignment target";
            break;
        case ErrorType::EXPECTED_RIGHT_BRACE:
            ostream << "Expected \"}\" to end block";
            break;
        case ErrorType::EXPECTED_LEFT_BRACE:
            ostream << "Expected \"{\" to end block";
            break;
        case ErrorType::EXPECTED_LEFT_PAREN_IF:
            ostream << "Expected \"(\" after \"if\"";
            break;
        case ErrorType::EXPECTED_RIGHT_PAREN_IF:
            ostream << "Expected \")\" to end \"if\" condition";
            break;
        case ErrorType::EXPECTED_LEFT_PAREN_FOR:
            ostream << "Expected \"(\" after \"for\"";
            break;
        case ErrorType::EXPECTED_RIGHT_PAREN_FOR:
            ostream << "Expected \")\" to end \"for\" condition";
            break;
        case ErrorType::EXPECTED_LEFT_PAREN_WHILE:
            ostream << "Expected \"(\" after \"while\"";
            break;
        case ErrorType::EXPECTED_RIGHT_PAREN_WHILE:
            ostream << "Expected \")\" to end \"while\" condition";
            break;
        case ErrorType::EXPECTED_LEFT_PAREN_FUN:
            ostream << "Expected \"(\" after \"fun\"";
            break;
        case ErrorType::EXPECTED_RIGHT_PAREN_FUN:
            ostream << "Expected \")\" to end \"fun\" arguments";
            break;
        case ErrorType::EXCESSIVE_ARGUMENTS:
            ostream << "Cannot have ore than 255 arguments";
            break;
        case ErrorType::EXPECTED_FUNCTION_NAME:
            ostream << "Expected function name";
            break;
        case ErrorType::EXPECTED_PARAMETER_NAME:
            ostream << "Expected parameter name";
            break;
        case ErrorType::CALL_PROHIBITED:
            ostream << "Call from base Call object prohibited";
            break;
        case ErrorType::BAD_ENVIRONMENT:
            ostream << "Could not read local variable in its own initializer";
            break;
        case ErrorType::VARIABLE_ALREADY_EXISTS:
            ostream << "A variable with this name already exists in this scope";
            break;
        case ErrorType::NO_RETURN_POSSIBLE:
            ostream << "Cannot return from the top-level code";
            break;
        case ErrorType::END_OF_ERRORS:
            break;
    }
    return ostream;
}

// Error class can be consolidated using requires clauses more intelligently
template<typename T>
class [[nodiscard]] ErrorOr {
public:
    using ValueType = std::remove_cvref_t<T>;

    ErrorOr() = default;

    template<typename U>
    requires (std::is_convertible_v<std::remove_cvref_t<U>, ValueType>)
    ErrorOr(
        U&& value
    ) 
        : m_is_error{ false }
        , m_error{}
        , m_value(std::move(value))
    {}

    ErrorOr(
        ErrorType error
    ) 
        : m_is_error{ true }
        , m_error{ error }
        , m_value{ ValueType{} }
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

    template<typename U>
    requires (std::same_as<T, U>)
    auto
    operator=(
        ErrorOr<U> const& other
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
    ValueType m_value;
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
    int                line,
    int                column,
    ErrorType          error_type,
    std::string const& error_msg
) -> void {
    std::cout << "\033[31m" << "[Error]" << " "
              << line << ":" 
              << column << " "
              << error_type << " "
              << error_msg << "\033[0m" << std::endl;
}

inline auto
error(
    std::string_view file,
    int              line,
    std::string_view command,
    ErrorType        error_type
) -> void {
    std::cout << "\033[31m" << "[Error]" << " "
              << file << ":"
              << line << ": " 
              << command << " "
              << error_type <<  "\033[0m" << std::endl;
}
