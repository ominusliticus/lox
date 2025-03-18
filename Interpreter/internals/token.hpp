#pragma once

#include <string_view>

namespace lox {

enum class TokenType {
    SEMICOLON
};

struct Token {
    TokenType        type;
    std::string_view token;
};

}
