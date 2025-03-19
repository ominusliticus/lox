#pragma once

#include <string>
#include <string_view>
#include <vector>

#include "internals/token.hpp"

#include "util/error.hpp"


class Scanner {
public:
    static constexpr std::string_view nil{ "" };
    Scanner() = default;
    Scanner(std::string source) 
        : m_source{ source }
    {}

    auto
    scan_tokens(
    ) -> ErrorOr<std::vector<Token>&> {
        while (!not_end()) {
            m_start = m_current;
            scan_token();
        }

        m_tokens.emplace_back(
            TokenType::ENDOFFILE,
            "",
            nil.data(),
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
            default: 
                error(m_line, m_column, ErrorType::INVALID_SYMBOL); 
                m_has_error &= false;
                break;
        }
    }

    auto
    add_token(
        TokenType type
    ) -> void {
        add_token(type, nil.data());
    }

    auto
    add_token(
        TokenType        type,
        std::string_view literal
    ) -> void {
        std::string_view text{ m_source.substr(m_start, m_current) };
        m_tokens.emplace_back(
            type,
            text,
            literal,
            m_line,
            m_column
        );
    }

    auto
    advance(
    ) -> char {
        return m_source[m_current++];
    }

    auto
    match(
        char expected
    ) -> bool {
        if (not_end()) {
            if (m_source[m_current] == expected) {
                ++m_current;
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


private:
    std::string        m_source;
    std::vector<Token> m_tokens;
    int                m_start;
    int                m_current;
    int                m_line;
    int                m_column;
    bool               m_has_error;
};
