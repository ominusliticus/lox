#pragma once

enum class StatementType {
    PRINT,
    EXPRESSION,
    VAR_DECL,
    BLOCK,
    IF,
    WHILE,
    FUN_DECL
};


template<typename OStream>
auto
operator<<(
    OStream& ostream,
    StatementType type
) -> OStream& {
    switch (type) {
        case StatementType::PRINT:
            ostream << "PRINT";
            break;
        case StatementType::EXPRESSION:
            ostream << "EXPRESSION";
            break;
        case StatementType::VAR_DECL:
            ostream << "VAR_DECL";
            break;
        case StatementType::BLOCK:
            ostream << "BLOCK";
            break;
        case StatementType::IF:
            ostream << "IF";
            break;
        case StatementType::WHILE:
            ostream << "WHILE";
            break;
        case StatementType::FUN_DECL:
            ostream << "FUN_DECL";
            break;
    }
    return ostream;
}
