#pragma once

#include <string>
#include <string_view>
#include <vector>
#include <unordered_map>

#include "internals/nil.hpp"
#include "internals/object.hpp"
#include "internals/token.hpp"

#include "util/error.hpp"

// TODO: Add documentation strings

class Scanner {
public:
    Scanner() = default;
    Scanner(std::string source) ;

    ErrorOr<std::vector<Token>&> scan_tokens();
    void                         scan_token();
    void                         add_token(TokenType type);
    void                         add_token(TokenType type, Object literal);

    bool is_alphabetic(char ch);
    bool is_alphanumeric(char ch);
    bool is_digit(char ch);

    bool match(char expected);
    bool not_end();
    
    char advance();
    char peek();
    char peek_next();

    void process_number();
    void process_string();
    void process_identifier();

private:
    std::string        m_source;
    std::vector<Token> m_tokens{};
    int                m_start{ 0 };
    int                m_current{ 0 };
    int                m_line{ 0 };
    int                m_column{ 0 };
    bool               m_has_error{ false };

    std::unordered_map<std::string_view, TokenType> m_keywords {
        {"and",    TokenType::AND},
        {"class",  TokenType::CLASS},
        {"else",   TokenType::ELSE},
        {"false",  TokenType::FALSE},
        {"for",    TokenType::FOR},
        {"fun",    TokenType::FUN},
        {"if",     TokenType::IF},
        {"nil",    TokenType::NIL},
        {"or",     TokenType::OR},
        {"print",  TokenType::PRINT},
        {"return", TokenType::RETURN},
        {"super",  TokenType::SUPER},
        {"this",   TokenType::THIS},
        {"true",   TokenType::TRUE},
        {"var",    TokenType::VAR},
        {"while",  TokenType::WHILE},
    };
};
