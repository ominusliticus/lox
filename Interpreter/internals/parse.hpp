#pragma once

#include <memory>
#include <vector>

#include "internals/ast.hpp"
#include "internals/nil.hpp"
#include "internals/token.hpp"

#include "util/error.hpp"
#include "util/try.hpp"


// TODO: Add support for ternary operations
// TODO: Add automatic detection of failed righ expression for binary operations
// TODO: Add synchronize function from Ch. 6


// Implementing the following grammar:
// program     -> statement* EOF ;
// statement   -> print_stmt | expr_stmt ;
// print_stmt  -> "print" expression ";" ;
// expr_stmt   -> expression ";" ;
// expression  -> equality | block;
// block       -> expression "," expression;
// equality    -> comparison (( "!=" | "==" ) comparison )* ;
// comparison  -> term ( ( ">" | ">=" | "<" | "<=" ) term )* ;
// term        -> factor ( ( "-"| "+" ) factor )* ;
// factor      -> unary ( ( "/" | "*" ) unary )* ;
// unary       -> ( "!" | "-" ) unary
//              | primary;
// primary     -> NUMBER | STRING |"true" | "false" | "bil"
//              | "(" expression ")" ;
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

    Parser(
        std::vector<Token> tokens
    ) 
        : m_tokens{ tokens }
    {}

    // auto
    // parse(
    // ) -> ErrorOr<std::shared_ptr<Expression>> {
    //     return get_expression();
    // }
    
    auto 
    parse(
    ) -> ErrorOr<std::vector<std::shared_ptr<Statement>>> {
        std::vector<std::shared_ptr<Statement>> statements;
        while (!is_end()) {
            auto stmt = TRY(get_statement());
            statements.push_back(stmt);
        }
        return statements;
    }

private:
    auto
    get_statement(
    ) -> ErrorOr<std::shared_ptr<Statement>> {
        if (match(TokenType::PRINT)) return TRY(print_statement());
        return TRY(expression_statement());
    }

    auto
    print_statement(
    ) -> ErrorOr<std::shared_ptr<Statement>> {
        auto expr = TRY(get_expression());
        TRY(consume(TokenType::SEMICOLON, ErrorType::EXPECTED_SEMICOLON));
        return std::static_pointer_cast<Statement>(std::make_shared<PrintStmt>(expr));
    }

    auto
    expression_statement(
    ) -> ErrorOr<std::shared_ptr<Statement>> {
        auto expr = TRY(get_expression());
        TRY(consume(TokenType::SEMICOLON, ErrorType::EXPECTED_SEMICOLON));
        return std::static_pointer_cast<Statement>(std::make_shared<ExpressionStmt>(expr));
    }

    auto
    get_expression(
    ) -> ErrorOr<std::shared_ptr<Expression>> {
        std::shared_ptr<Expression> left_expression{ TRY_LOX(equality(), previous()) };
        while (match(TokenType::COMMA)) {
            std::shared_ptr<Token> operation{ previous() };
            std::shared_ptr<Expression> right_expression{ TRY_LOX(equality(), previous()) };
            left_expression = std::static_pointer_cast<Expression>(std::make_shared<Binary>(
                left_expression, operation, right_expression
            ));
        }
        return left_expression;
    }

    auto
    equality(
    ) -> ErrorOr<std::shared_ptr<Expression>> {
        std::shared_ptr<Expression> left_expression{ TRY_LOX(comparison(), previous()) };
        while (match(TokenType::BANG_EQUAL, TokenType::EQUAL_EQUAL)) {
            std::shared_ptr<Token>      operation{ previous() };
            std::shared_ptr<Expression> right_expression{ TRY_LOX(comparison(), previous()) };
            left_expression = std::make_shared<Binary>(
                left_expression, operation, right_expression
            );
        }
        return std::static_pointer_cast<Expression>(left_expression);
    }

    auto
    comparison(
    ) -> ErrorOr<std::shared_ptr<Expression>> {
        std::shared_ptr<Expression> left_expression{ TRY_LOX(term(), previous()) };
        while (match(
            TokenType::GREATER, TokenType::GREATER_EQUAL,
            TokenType::LESS, TokenType::LESS_EQUALS)
        ) {
            std::shared_ptr<Token>      operation{ previous() };
            std::shared_ptr<Expression> right_expression{ TRY_LOX(term(), previous()) }; 
            left_expression = std::make_shared<Binary>(
                left_expression, operation, right_expression
            );
        }
        return std::static_pointer_cast<Expression>(left_expression);
    }

    auto
    term(
    ) -> ErrorOr<std::shared_ptr<Expression>> {
        std::shared_ptr<Expression> left_expression{ TRY_LOX(factor(), previous()) };
        while (match(TokenType::MINUS, TokenType::PLUS)) {
            std::shared_ptr<Token>      operation{ previous() };
            std::shared_ptr<Expression> right_expression{ TRY_LOX(unary(), previous()) };
            left_expression = std::make_shared<Binary>(
                left_expression, operation, right_expression
            );
        }
        return std::static_pointer_cast<Expression>(left_expression);
    }

    auto
    factor(
    ) -> ErrorOr<std::shared_ptr<Expression>> {
        std::shared_ptr<Expression> left_expression{ TRY_LOX(unary(), previous()) };
        while (match(TokenType::SLASH, TokenType::STAR)) {
            std::shared_ptr<Token>      operation{ previous() };
            std::shared_ptr<Expression> right_expression{ TRY_LOX(factor(), previous()) };
            left_expression = std::make_shared<Binary>(
                left_expression, operation, right_expression
            );
        }
        return std::static_pointer_cast<Expression>(left_expression);
    }

    auto
    unary(
    ) -> ErrorOr<std::shared_ptr<Expression>> {
        if (match(TokenType::BANG, TokenType::MINUS)) {
            std::shared_ptr<Token> operation{ previous() };
            std::shared_ptr<Expression> right_expression{ TRY_LOX(unary(), previous()) };
            return std::static_pointer_cast<Expression>(
                std::make_shared<Unary>(operation, right_expression)
            );
        }
        return primary();
    }

    auto
    primary(
    ) -> ErrorOr<std::shared_ptr<Expression>> {
        if (match(TokenType::FALSE)) 
            return std::static_pointer_cast<Expression>(std::make_shared<Literal>(Object(false)));
        if (match(TokenType::TRUE))
            return std::static_pointer_cast<Expression>(std::make_shared<Literal>(Object(true)));
        if (match(TokenType::NIL))
            return std::static_pointer_cast<Expression>(std::make_shared<Literal>(Object(nil)));
        if (match(TokenType::NUMBER, TokenType::STRING)) {
            return std::static_pointer_cast<Expression>( 
                std::make_shared<Literal>(previous()->literal)
            );
        }
        if (match(TokenType::LEFT_PAREN)) {
            std::shared_ptr<Expression> expr{ TRY_LOX(get_expression(), previous()) };
            TRY_LOX(consume(TokenType::RIGHT_PAREN, ErrorType::CLOSING_PAREN), previous());
            return std::static_pointer_cast<Expression>(std::make_shared<Grouping>(expr));
        }
        if (match(TokenType::IDENTIFIER))
            return ErrorType::UNIMPLEMENTED;
        return ErrorType::EXPECTED_EXPRESSION;
    }

    auto 
    consume(
        TokenType token_type,
        ErrorType error_type
    ) -> ErrorOr<std::shared_ptr<Token>> {
        if (check(token_type)) return advance();
        else return error_type;
    }

    auto
    match(
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

    auto
    check(
        TokenType token_type
    ) -> bool {
        if (not_end()) return peek().type == token_type;
        else return false;
    }

    auto 
    advance(
    ) -> std::shared_ptr<Token> {
        if (not_end()) ++m_current;
        return previous();
    }

    auto
    previous(
    ) -> std::shared_ptr<Token> {
        return std::make_shared<Token>(m_tokens[m_current - 1]);
    }

    auto
    peek(
    ) -> Token const& {
        return m_tokens[m_current];
    }

    auto
    not_end(
    ) -> bool {
        return !is_end();
    }

    auto 
    is_end(
    ) -> bool {
        return peek().type == TokenType::ENDOFFILE;
    }


private:
    std::vector<Token> m_tokens;
    int                m_current{ 0 };
};
