#pragma once

#include <string_view>
#include <string>

#include "internals/object.hpp"

enum class TokenType {
    // Single-character tokens
    LEFT_PAREN,
    RIGHT_PAREN,
    LEFT_BRACE,
    RIGHT_BRACE,
    COMMA,
    DOT,
    MINUS,
    PLUS,
    SEMICOLON,
    SLASH, 
    STAR,
    // One or tow character tokens
    BANG,
    BANG_EQUAL,
    EQUAL,
    EQUAL_EQUAL,
    GREATER,
    GREATER_EQUAL,
    LESS,
    LESS_EQUALS,
    // Literals
    IDENTIFIER,
    STRING,
    NUMBER,
    // Keyword
    AND,
    CLASS,
    ELSE,
    FALSE,
    FUN,
    FOR, 
    IF,
    NIL,
    OR,
    PRINT,
    RETURN,
    SUPER,
    THIS,
    TRUE,
    VAR, 
    WHILE,
    ENDOFFILE
};

struct Token {
    Token(
        TokenType   type_,
        std::string lexeme_,
        Object      literal_,
        int         line_,
        int         column_
    ) 
        : type(type_)
        , lexeme(lexeme_)
        , literal(literal_)
        , line(line_)
        , column(column_)
    {}

    TokenType   type;
    std::string lexeme;
    Object      literal;
    int         line;
    int         column;
};

template<class OStream>
auto
operator<<(
    OStream&  ostream,
    TokenType type
) -> OStream& {
    switch (type) {
        case TokenType::LEFT_PAREN:    ostream << "("; break;
        case TokenType::RIGHT_PAREN:   ostream << ")"; break;
        case TokenType::LEFT_BRACE:    ostream << "{"; break;
        case TokenType::RIGHT_BRACE:   ostream << "}"; break;
        case TokenType::COMMA:         ostream << ","; break;
        case TokenType::DOT:           ostream << "."; break;
        case TokenType::MINUS:         ostream << "-"; break;
        case TokenType::PLUS:          ostream << "+"; break;
        case TokenType::SEMICOLON:     ostream << ";"; break;
        case TokenType::SLASH:         ostream << "/"; break;
        case TokenType::STAR:          ostream << "*"; break;
        case TokenType::BANG:          ostream << "!"; break;
        case TokenType::BANG_EQUAL:    ostream << "!="; break;
        case TokenType::EQUAL:         ostream << "="; break;
        case TokenType::EQUAL_EQUAL:   ostream << "=="; break;
        case TokenType::GREATER:       ostream << "<"; break;
        case TokenType::GREATER_EQUAL: ostream << "<="; break;
        case TokenType::LESS:          ostream << ">"; break;
        case TokenType::LESS_EQUALS:   ostream << ">="; break;
        case TokenType::IDENTIFIER:    ostream << "ident"; break;
        case TokenType::STRING:        ostream << "string"; break;
        case TokenType::NUMBER:        ostream << "number"; break;
        case TokenType::AND:           ostream << "&&"; break;
        case TokenType::CLASS:         ostream << "class"; break;
        case TokenType::ELSE:          ostream << "else"; break;
        case TokenType::FALSE:         ostream << "false"; break;
        case TokenType::FUN:           ostream << "func"; break;
        case TokenType::FOR:           ostream << "for"; break;
        case TokenType::IF:            ostream << "if"; break;
        case TokenType::NIL:           ostream << "NIL"; break;
        case TokenType::OR:            ostream << "||"; break;
        case TokenType::PRINT:         ostream << "print"; break;
        case TokenType::RETURN:        ostream << "return"; break;
        case TokenType::SUPER:         ostream << "super"; break;
        case TokenType::THIS:          ostream << "this"; break;
        case TokenType::TRUE:          ostream << "true"; break;
        case TokenType::VAR:           ostream << "var"; break;
        case TokenType::WHILE:         ostream << "while"; break;
        case TokenType::ENDOFFILE:     ostream << "EOF"; break;
    }
    return ostream;
}

template<class OStream>
auto
operator<<(
    OStream& ostream,
    Token const& token
) -> OStream& {
    ostream << token.type << " " 
            << token.lexeme << " " 
            << token.literal;
    return ostream;
}
