#pragma once

#include <memory>
#include <vector>

#include "internals/ast.hpp"
#include "internals/nil.hpp"
#include "internals/token.hpp"

#include "util/error.hpp"


// TODO: Add support for ternary operations
// TODO: Add automatic detection of failed righ expression for binary operations
// TODO: Add synchronize function from Ch. 6


// Implementing the following grammar:
// program     -> declaration* EOF ;
// declaration -> var_decl | statement ;
// var_decl    -> "var" IDENTIFIER ( "=" expression )? ";" ;
// statement   -> print_stmt | expr_stmt ;
// print_stmt  -> "print" expression ";" ;
// expr_stmt   -> expression ";" ;
// expression  -> assignment | block ;
// assignment  -> IDENTIFIER "=" assignment | equality ;
// block       -> expression "," expression;
// equality    -> comparison (( "!=" | "==" ) comparison )* ;
// comparison  -> term ( ( ">" | ">=" | "<" | "<=" ) term )* ;
// term        -> factor ( ( "-"| "+" ) factor )* ;
// factor      -> unary ( ( "/" | "*" ) unary )* ;
// unary       -> ( "!" | "-" ) unary | primary;
// primary     -> NUMBER | STRING |"true" | "false" | "nil" | "(" expression ")" | IDENTIFIER ;
//
// The grammar notation above is translated into code using the table:
// term        code to match and consume a token
// non-term    call to that rule's function
// |           if or switch statement
// * or +      while or for loop
// ?           if statement

class Parser {
public:
    Parser() = default;
    Parser(std::vector<Token> tokens); 

    // auto
    // parse(
    // ) -> ErrorOr<std::shared_ptr<Expression>> {
    //     return get_expression();
    // }
    
    ErrorOr<std::vector<std::shared_ptr<Statement>>> parse();

private:
    ErrorOr<std::shared_ptr<Statement>> declaration();
    ErrorOr<std::shared_ptr<Statement>> variable_declaration();
    ErrorOr<std::shared_ptr<Statement>> get_statement();
    ErrorOr<std::shared_ptr<Statement>> print_statement();
    ErrorOr<std::shared_ptr<Statement>> expression_statement();

    ErrorOr<std::shared_ptr<Expression>> get_expression();
    ErrorOr<std::shared_ptr<Expression>> assignment();
    ErrorOr<std::shared_ptr<Expression>> equality();
    ErrorOr<std::shared_ptr<Expression>> comparison();
    ErrorOr<std::shared_ptr<Expression>> term();
    ErrorOr<std::shared_ptr<Expression>> factor();
    ErrorOr<std::shared_ptr<Expression>> unary();
    ErrorOr<std::shared_ptr<Expression>> primary();
    
    bool check(TokenType token_type);
    bool match(auto... token_types);

    ErrorOr<std::shared_ptr<Token>> consume(TokenType token_type, ErrorType error_type);
    std::shared_ptr<Token>          advance();
    std::shared_ptr<Token>          previous();

    Token const& peek();
    
    bool not_end();
    bool is_end();

private:
    std::vector<Token> m_tokens;
    int                m_current{ 0 };
};

auto
Parser::match(
    auto... token_types // Convient way to represent variadic of TokenType
) -> bool {
    for (auto token_type : {token_types...}) {
        if (check(token_type)) {
            advance();
            return true;
        }
    }
    return false;
}

