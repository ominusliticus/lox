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
// declaration -> func_decl | var_decl | statement ;
// func_decl   -> "fun" function ;
// function    -> IDENTIFIER "(" parameters? ")" stmt_block;
// parameters  -> IDENTIFIER ( "," IDENTIFIER )* ;
// var_decl    -> "var" IDENTIFIER ( "=" expression )? ";" ;
// statement   -> expr_stmt | for_stmt | if_stmt | print_stmt |return_stmt 
//                          | while_stmt | stmt_block ;
// for_stmt    -> "for" "(" ( var_decl | expr_stmt | ";" ) expression? ";" expression? ")" statement ;
// if_stmt     -> "if" "(" expression ")" statement ( "else" statement)? ;
// while_stmt  -> "while" "(" expression ")" statement ;
// print_stmt  -> "print" expression ";" ;
// return_stmt -> "return" expression? ";" ;
// expr_stmt   -> expression ";" ;
// stmt_block  -> "{" declaration* "}";
// expression  -> assignment | expr_block ;
// assignment  -> IDENTIFIER "=" assignment | logic_or ;
// logic_or    -> logic_and ( "or" logic_and )* ;
// logic_and   -> equality ( "and" equality )* ;
// expr_block  -> expression "," expression;
// equality    -> comparison (( "!=" | "==" ) comparison )* ;
// comparison  -> term ( ( ">" | ">=" | "<" | "<=" ) term )* ;
// term        -> factor ( ( "-"| "+" ) factor )* ;
// factor      -> unary ( ( "/" | "*" ) unary )* ;
// unary       -> ( "!" | "-" ) unary | call ; 
// call        -> primary | ( "(" arguments? ")" )* ;
// primary     -> NUMBER | STRING |"true" | "false" | "nil" | "(" expression ")" | IDENTIFIER ;
// arguments   -> expression ( "," expression )* ;
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
    Parser(std::vector<Token>&& tokens); 

    // auto
    // parse(
    // ) -> ErrorOr<std::unique_ptr<Expression>> {
    //     return get_expression();
    // }
    
    ErrorOr<std::vector<std::unique_ptr<Statement>>> parse();

private:
    ErrorOr<std::unique_ptr<Statement>> declaration();
    ErrorOr<std::unique_ptr<Statement>> variable_declaration();
    ErrorOr<std::unique_ptr<Statement>> get_statement();
    ErrorOr<std::unique_ptr<Statement>> print_statement();
    ErrorOr<std::unique_ptr<Statement>> for_statement();
    ErrorOr<std::unique_ptr<Statement>> if_statement();
    ErrorOr<std::unique_ptr<Statement>> while_statement();
    ErrorOr<std::unique_ptr<Statement>> expression_statement();
    ErrorOr<std::unique_ptr<Statement>> return_statement();
    ErrorOr<std::unique_ptr<Statement>> function(std::string kind);

    ErrorOr<std::vector<std::unique_ptr<Statement>>> get_block();

    ErrorOr<std::unique_ptr<Expression>> get_expression();
    ErrorOr<std::unique_ptr<Expression>> assignment();
    ErrorOr<std::unique_ptr<Expression>> or_expression();
    ErrorOr<std::unique_ptr<Expression>> and_expression();
    ErrorOr<std::unique_ptr<Expression>> equality();
    ErrorOr<std::unique_ptr<Expression>> comparison();
    ErrorOr<std::unique_ptr<Expression>> term();
    ErrorOr<std::unique_ptr<Expression>> factor();
    ErrorOr<std::unique_ptr<Expression>> unary();
    ErrorOr<std::unique_ptr<Expression>> call();
    ErrorOr<std::unique_ptr<Expression>> primary();

    ErrorOr<std::unique_ptr<Call>> finish_call(std::unique_ptr<Expression> callee);
    
    bool check(TokenType token_type);
    bool match(auto... token_types);

    ErrorOr<std::unique_ptr<Token>> consume(TokenType token_type, ErrorType error_type);
    std::unique_ptr<Token>          advance();
    std::unique_ptr<Token>          previous();

    Token const& peek();
    Token const& peek_next();
    
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

