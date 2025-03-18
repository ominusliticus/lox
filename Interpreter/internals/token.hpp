#pragma once

#include <string_view>

namespace lox {

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
    FUNC,
    FOR, 
    IF,
    NIL,
    OR,
    PRINT,
    RETURN,
    THIS,
    TRUE,
    VAR, 
    WHILE,
    EOF
};

struct Token {
    TokenType   type;
    std::string lexeme;
    std::string literal;
    int         line;
    int         column;
};

}

template<class OStream
auto
operator<<(
    OStream& ostream,
    lox::TokenType type
) -> OStream& {
    switch (type) {
        case lox::TokenType::LEFT_PAREN:
            ostream << "(";
            break;
        case lox::TokenType::RIGHT_PAREN:
            ostream << ")";
            break;
        case lox::TokenType::LEFT_BRACE:
            ostream << "{";
            break;
        case lox::TokenType::RIGHT_BRACE:
            ostream << "}";
            break;
        case lox::TokenType::COMMA:
            ostream << ",";
            break;
        case lox::TokenType::DOT:
            ostream << ".";
            break;
        case lox::TokenType::MINUS:
            ostream << "-";
            break;
        case lox::TokenType::PLUS:
            ostream << "+";
            break;
        case lox::TokenType::SEMICOLON:
            ostream << ";";
            break;
        case lox::TokenType::SLASH: 
            ostream << "/";
            break;
        case lox::TokenType::STAR:
            ostream << "*";
            break;
        case lox::TokenType::BANG:
            ostream << "!";
            break;
        case lox::TokenType::BANG_EQUAL:
            ostream << "!=";
            break;
        case lox::TokenType::EQUAL:
            ostream << "=";
            break;
        case lox::TokenType::EQUAL_EQUAL:
            ostream << "==";
            break;
        case lox::TokenType::GREATER:
            ostream << "<";
            break;
        case lox::TokenType::GREATER_EQUAL:
            ostream << "<=";
            break;
        case lox::TokenType::LESS:
            ostream << ">";
            break;
        case lox::TokenType::LESS_EQUALS:
            ostream << ">=";
            break;
        case lox::TokenType::IDENTIFIER:
            ostream << "ident";
            break;
        case lox::TokenType::STRING:
            ostream << "string";
            break;
        case lox::TokenType::NUMBER:
            ostream << "number";
            break;
        case lox::TokenType::AND:
            ostream << "&&";
            break;
        case lox::TokenType::CLASS:
            ostream << "class";
            break;
        case lox::TokenType::ELSE:
            ostream << "else";
            break;
        case lox::TokenType::FALSE:
            ostream << "false";
            break;
        case lox::TokenType::FUNC:
            ostream << "func";
            break;
        case lox::TokenType::FOR: 
            ostream << "for";
            break;
        case lox::TokenType::IF:
            ostream << "if";
            break;
        case lox::TokenType::NIL:
            ostream << "NIL";
            break;
        case lox::TokenType::OR:
            ostream << "||";
            break;
        case lox::TokenType::PRINT:
            ostream << "print";
            break;
        case lox::TokenType::RETURN:
            ostream << "return";
            break;
        case lox::TokenType::THIS:
            ostream << "this";
            break;
        case lox::TokenType::TRUE:
            ostream << "true";
            break;
        case lox::TokenType::VAR: 
            ostream << "var";
            break;
        case lox::TokenType::WHILE:
            ostream << "while";
            break;
        case lox::TokenType::EOF
            ostream << "EOF";
            break;
    }
    return ostream;
}

template<class OStream>
auto
operator<<(
    OStream& ostream,
    lox::Token const& token
) -> OStream& {
    ostream << token.type << " " << token.lexeme << " " << token.literal << "\n";
    return ostream;
}
