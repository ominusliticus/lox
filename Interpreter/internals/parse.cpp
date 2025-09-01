#include "internals/parse.hpp"

#include "internals/ast.hpp"

#include "util/try.hpp"


// TODO:
//  1. No use of auto keyword
//  2. Use copy initialization over uniform initialization
//  3. Align on identifier name
//  4. Alight on equal signs
//  5. Use TRY_LOX over TRY

Parser::Parser(
    std::vector<Token>&& tokens
) 
    : m_tokens{ tokens }
{}

// auto
// parse(
// ) -> ErrorOr<std::shared_ptr<Expression>> {
//     return get_expression();
// }

auto 
Parser::parse(
) -> ErrorOr<std::vector<std::shared_ptr<Statement>>> {
    std::vector<std::shared_ptr<Statement>> statements;
    while (!is_end()) {
        auto stmt = TRY(declaration());
        statements.push_back(stmt);
    }
    return statements;
}

auto
Parser::declaration(
) -> ErrorOr<std::shared_ptr<Statement>> {
    if (match(TokenType::VAR)) return variable_declaration();
    return get_statement();
}

auto 
Parser::variable_declaration(
) -> ErrorOr<std::shared_ptr<Statement>> {
    Token name = *TRY(consume(TokenType::IDENTIFIER, ErrorType::EXPECTED_VARIABLE_NAME));
    std::shared_ptr<Expression> initiailizer;
    if (match(TokenType::EQUAL)) {
        initiailizer = TRY(get_expression());
    }
    TRY(consume(TokenType::SEMICOLON, ErrorType::EXPECTED_SEMICOLON));
    return std::static_pointer_cast<Statement>(std::make_shared<VarDeclStmt>(name, initiailizer));
}

auto
Parser::get_statement(
) -> ErrorOr<std::shared_ptr<Statement>> {
    if (match(TokenType::IF)) return TRY(if_statement());
    if (match(TokenType::PRINT)) return TRY(print_statement());
    if (match(TokenType::LEFT_BRACE)) {
        auto stmts = TRY(get_block());
        return std::static_pointer_cast<Statement>(std::make_shared<Block>(std::move(stmts)));
    }
    return TRY(expression_statement());
}

auto
Parser::get_block(
) -> ErrorOr<std::vector<std::shared_ptr<Statement>>> {
    std::vector<std::shared_ptr<Statement>> statements;
    while (!check(TokenType::RIGHT_BRACE) && not_end()) {
        auto stmt = TRY(declaration());
        statements.push_back(stmt);
    }
    TRY(consume(TokenType::RIGHT_BRACE, ErrorType::EXPECTED_RIGHT_BRACE));
    return statements;
}

auto
Parser::print_statement(
) -> ErrorOr<std::shared_ptr<Statement>> {
    auto expr = TRY(get_expression());
    TRY(consume(TokenType::SEMICOLON, ErrorType::EXPECTED_SEMICOLON));
    return std::static_pointer_cast<Statement>(std::make_shared<PrintStmt>(expr));
}

auto
Parser::if_statement(
) -> ErrorOr<std::shared_ptr<Statement>> {
   TRY(consume(TokenType::LEFT_PAREN, ErrorType::EXPECTED_LEFT_PAREN)); 
    auto condition = TRY(get_expression());
    TRY(consume(TokenType::RIGHT_PAREN, ErrorType::EXPECTED_RIGHT_PAREN));
    auto then_branch = TRY(get_statement());
    if (match(TokenType::ELSE)) {
        auto else_branch = TRY(get_statement());
        return std::static_pointer_cast<Statement>(
            std::make_shared<IfStmt>(condition, then_branch, else_branch)
        );
    }
    return std::static_pointer_cast<Statement>(
        std::make_shared<IfStmt>(condition, then_branch, nullptr)
    );
}

auto
Parser::expression_statement(
) -> ErrorOr<std::shared_ptr<Statement>> {
    auto expr = TRY(get_expression());
    TRY(consume(TokenType::SEMICOLON, ErrorType::EXPECTED_SEMICOLON));
    return std::static_pointer_cast<Statement>(std::make_shared<ExpressionStmt>(expr));
}

auto
Parser::get_expression(
) -> ErrorOr<std::shared_ptr<Expression>> {
    return assignment();
    // std::shared_ptr<Expression> left_expression{ TRY_LOX(equality(), previous()) };
    // while (match(TokenType::COMMA)) {
    //     std::shared_ptr<Token> operation{ previous() };
    //     std::shared_ptr<Expression> right_expression{ TRY_LOX(equality(), previous()) };
    //     left_expression = std::static_pointer_cast<Expression>(std::make_shared<Binary>(
    //         left_expression, operation, right_expression
    //     ));
    // }
    // return left_expression;
}

auto
Parser::assignment(
) -> ErrorOr<std::shared_ptr<Expression>> {
    auto expr = TRY(or_expression());

    if (match(TokenType::EQUAL)) {
        auto equals = previous();
        auto value = TRY(assignment());
        if (value->expression_type == ExpressionType::VARIABLE) {
            Token name = std::static_pointer_cast<Variable>(value)->name;
            auto result = std::make_shared<Assignment>(std::move(name), value);
            return std::static_pointer_cast<Expression>(result);
        }
        return ErrorType::EXPECTED_ASSIGNMENT_TARGET;
    }
    return expr;
}

auto
Parser::or_expression(
) -> ErrorOr<std::shared_ptr<Expression>> {
    auto expr = TRY(and_expression());
    while (match(TokenType::OR)) {
        auto operation = previous();
        auto right = TRY(and_expression());
        expr = std::static_pointer_cast<Expression>(
            std::make_shared<Logical>(expr, operation, right)
        );
    }
    return expr;
}

auto 
Parser::and_expression(
) -> ErrorOr<std::shared_ptr<Expression>> {
    auto expr = TRY(equality());
    while (match(TokenType::AND)) {
        auto operation = previous();
        auto right = TRY(equality());
        expr = std::static_pointer_cast<Expression>(
            std::make_shared<Logical>(expr, operation, right)
        );
    }
    return expr;
}

auto
Parser::equality(
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
Parser::comparison(
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
Parser::term(
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
Parser::factor(
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
Parser::unary(
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
Parser::primary(
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
    if (match(TokenType::IDENTIFIER))
        return std::static_pointer_cast<Expression>(std::make_shared<Variable>(previous()));
    if (match(TokenType::LEFT_PAREN)) {
        std::shared_ptr<Expression> expr{ TRY_LOX(get_expression(), previous()) };
        TRY_LOX(consume(TokenType::RIGHT_PAREN, ErrorType::CLOSING_PAREN), previous());
        return std::static_pointer_cast<Expression>(std::make_shared<Grouping>(expr));
    }
    return ErrorType::EXPECTED_EXPRESSION;
}

auto 
Parser::consume(
    TokenType token_type,
    ErrorType error_type
) -> ErrorOr<std::shared_ptr<Token>> {
    if (check(token_type)) return advance();
    else return error_type;
}

auto
Parser::check(
    TokenType token_type
) -> bool {
    if (not_end()) return peek().type == token_type;
    else return false;
}

auto 
Parser::advance(
) -> std::shared_ptr<Token> {
    if (not_end()) ++m_current;
    return previous();
}

auto
Parser::previous(
) -> std::shared_ptr<Token> {
    return std::make_shared<Token>(m_tokens[m_current - 1]);
}

auto
Parser::peek(
) -> Token const& {
    return m_tokens[m_current];
}

auto
Parser::not_end(
) -> bool {
    return !is_end();
}

auto 
Parser::is_end(
) -> bool {
    return peek().type == TokenType::ENDOFFILE;
}
