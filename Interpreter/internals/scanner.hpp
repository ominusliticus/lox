#pragma once

#include <memory>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>

#include "internals/nil.hpp"
#include "internals/object.hpp"
#include "internals/token.hpp"

#include "util/error.hpp"
#include "util/print.hpp"


class Scanner {
public:
    Scanner() = default;
    Scanner(std::string source) 
        : m_source{ source }
    {}

    auto
    scan_tokens(
    ) -> ErrorOr<std::vector<Token>&> {
        while (not_end()) {
            m_start = m_current;
            scan_token();
        }

        m_tokens.emplace_back(
            TokenType::ENDOFFILE,
            "",
            Object(),
            m_line,
            0
        );
        return m_tokens;
    }

    auto
    scan_token(
    ) -> void {
        char ch{ advance() };
        switch (ch) {
            case '(': add_token(TokenType::LEFT_PAREN); break;
            case ')': add_token(TokenType::RIGHT_PAREN); break;
            case '{': add_token(TokenType::LEFT_BRACE); break;
            case '}': add_token(TokenType::RIGHT_BRACE); break;
            case ',': add_token(TokenType::COMMA); break;
            case '.': add_token(TokenType::DOT); break;
            case '-': add_token(TokenType::MINUS); break;
            case '+': add_token(TokenType::PLUS); break;
            case ';': add_token(TokenType::SEMICOLON); break;
            case '*': add_token(TokenType::STAR); break;
            case '!': add_token(match('=') ? TokenType::BANG_EQUAL : TokenType::BANG); break;
            case '=': add_token(match('=') ? TokenType::EQUAL_EQUAL : TokenType::EQUAL); break;
            case '<': add_token(match('=') ? TokenType::LESS_EQUALS : TokenType::LESS); break;
            case '>': add_token(match('=') ? TokenType::GREATER_EQUAL : TokenType::GREATER); break;
            case '/': 
                if (match('/')) // Matching single-line comments
                    while (peek() != '\n' && not_end()) advance();
                else if (match('*'))
                    while (!(peek() == '*' && peek_next() == '/') && not_end()) {
                        if (peek() == '\n') ++m_line;
                        advance();
                    }
                else
                 add_token(TokenType::SLASH);
                break;
            case ' ': break;
            case '\r': break;
            case '\t': break;
            case '\n':
                ++m_line;
                break;
            case '"': process_string(); break;
            default: 
                if (is_digit(ch)) process_number();
                else if (is_alphabetic(ch)) {
                    process_identifier();
                }
                else {
                    error(m_line, m_column, ErrorType::INVALID_SYMBOL); 
                    m_has_error |= true;
                }
                break;
        }
    }

    auto
    add_token(
        TokenType type
    ) -> void {
        add_token(type, Object());
    }

    auto
    add_token(
        TokenType   type,
        Object      literal
    ) -> void {
        m_tokens.emplace_back(
            type,
            m_source.substr(m_start, m_current - m_start),
            literal,
            m_line,
            m_column
        );
    }

    auto
    advance(
    ) -> char {
        char ch{ m_source[m_current++] };
        if (ch != '\n') m_column = 0;
        else ++m_column;
        return ch;
    }

    auto
    is_alphabetic(
        char ch
    ) -> bool {
        return ('a' <= ch && ch <= 'z') ||
               ('A' <= ch && ch <= 'Z') ||
               ch == '_';
    }

    auto
    is_alphanumeric(
        char ch
    ) -> bool {
        return is_digit(ch) || is_alphabetic(ch);
    }

    auto
    is_digit(
        char ch
    ) -> bool {
        return '0' <= ch && ch <= '9';
    }

    auto
    match(
        char expected
    ) -> bool {
        if (not_end()) {
            if (m_source[m_current] == expected) {
                ++m_current;
                ++m_column;
                return true;
            }
            else return false;
        }
        else return false;
    }

    auto
    not_end(
    ) -> bool {
        return static_cast<std::size_t>(m_current) < m_source.size();
    }

    auto 
    peek(
    ) -> char {
        return not_end() ? m_source[m_current] : '\0';
    }

    auto
    peek_next(
    ) -> char {
        if (static_cast<std::size_t>(m_current + 1) < m_source.size()) 
            return m_source[m_current + 1];
        else return '\0';
    }

    auto
    process_number(
    ) -> void {
        while (is_digit(peek())) advance();
        // Check decimal
        if (peek() == '.' && is_digit(peek_next())) {
            advance(); // Consume decimal
            while (is_digit(peek())) advance();
        }
        add_token(
            TokenType::NUMBER,
            Object{ std::stod(m_source.substr(m_start, m_current - m_start)) }
        );
    }

    auto
    process_string(
    ) -> void {
        // Lox does not support espace sequences
        // Therefore, Lox can support multi-line strings
        while(peek() != '"' && not_end()) {
            if (peek() == '\n') ++m_line;
            advance();
        }
        if (!not_end()) {
            error(m_line, m_column, ErrorType::UNTERMINATED_STRING);
        }
        // Consume closing quote
        advance();
        add_token(
            TokenType::STRING,
            Object{ m_source.substr(m_start + 1, (m_current - m_start) - 2) }
        );
    }

    auto
    process_identifier(
    ) -> void {
        while(is_alphanumeric(peek())) advance();
        std::string text{ m_source.substr(m_start, m_current - m_start) };
        if (m_keywords.find(text) != m_keywords.end()) add_token(m_keywords[text]);
        else add_token(TokenType::IDENTIFIER);
    }


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
