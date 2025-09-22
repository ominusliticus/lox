#include "internals/parse.hpp"

#include "internals/ast.hpp"

#include "util/try.hpp"



Parser::Parser(
    std::vector<Token>&& tokens
) 
    : m_tokens(tokens)
{}

// .....oooO0Oooo.....oooO0Oooo.....oooO0Oooo.....oooO0Oooo.....oooO0Oooo.....oooO0Oooo.....

auto 
Parser::parse(
) -> ErrorOr<std::vector<std::shared_ptr<Statement>>> {
    std::vector<std::shared_ptr<Statement>> statements;
    while (!is_end()) {
        std::shared_ptr<Statement> stmt = TRY(declaration());
        statements.push_back(stmt);
    }
    return statements;
}

// .....oooO0Oooo.....oooO0Oooo.....oooO0Oooo.....oooO0Oooo.....oooO0Oooo.....oooO0Oooo.....

auto
Parser::declaration(
) -> ErrorOr<std::shared_ptr<Statement>> {
    if (match(TokenType::VAR)) return variable_declaration();
    return get_statement();
}

// .....oooO0Oooo.....oooO0Oooo.....oooO0Oooo.....oooO0Oooo.....oooO0Oooo.....oooO0Oooo.....

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

// .....oooO0Oooo.....oooO0Oooo.....oooO0Oooo.....oooO0Oooo.....oooO0Oooo.....oooO0Oooo.....

auto
Parser::get_statement(
) -> ErrorOr<std::shared_ptr<Statement>> {
    if (match(TokenType::IF)) return TRY(if_statement());
    if (match(TokenType::PRINT)) return TRY(print_statement());
    if (match(TokenType::LEFT_BRACE)) {
        std::vector<std::shared_ptr<Statement>> stmts = TRY(get_block());
        return std::static_pointer_cast<Statement>(std::make_shared<Block>(std::move(stmts)));
    }
    return TRY(expression_statement());
}

// .....oooO0Oooo.....oooO0Oooo.....oooO0Oooo.....oooO0Oooo.....oooO0Oooo.....oooO0Oooo.....

auto
Parser::get_block(
) -> ErrorOr<std::vector<std::shared_ptr<Statement>>> {
    std::vector<std::shared_ptr<Statement>> statements;
    while (!check(TokenType::RIGHT_BRACE) && not_end()) {
        std::shared_ptr<Statement> stmt = TRY(declaration());
        statements.push_back(stmt);
    }
    TRY(consume(TokenType::RIGHT_BRACE, ErrorType::EXPECTED_RIGHT_BRACE));
    return statements;
}

// .....oooO0Oooo.....oooO0Oooo.....oooO0Oooo.....oooO0Oooo.....oooO0Oooo.....oooO0Oooo.....

auto
Parser::print_statement(
) -> ErrorOr<std::shared_ptr<Statement>> {
    std::shared_ptr<Expression> expr = TRY(get_expression());
    TRY(consume(TokenType::SEMICOLON, ErrorType::EXPECTED_SEMICOLON));
    return std::static_pointer_cast<Statement>(std::make_shared<PrintStmt>(expr));
}

// .....oooO0Oooo.....oooO0Oooo.....oooO0Oooo.....oooO0Oooo.....oooO0Oooo.....oooO0Oooo.....

auto
Parser::if_statement(
) -> ErrorOr<std::shared_ptr<Statement>> {
   TRY(consume(TokenType::LEFT_PAREN, ErrorType::EXPECTED_LEFT_PAREN)); 
    std::shared_ptr<Expression> condition = TRY(get_expression());
    TRY(consume(TokenType::RIGHT_PAREN, ErrorType::EXPECTED_RIGHT_PAREN));
    std::shared_ptr<Statement> then_branch = TRY(get_statement());
    if (match(TokenType::ELSE)) {
        std::shared_ptr<Statement> else_branch = TRY(get_statement());
        return std::static_pointer_cast<Statement>(
            std::make_shared<IfStmt>(condition, then_branch, else_branch)
        );
    }
    return std::static_pointer_cast<Statement>(
        std::make_shared<IfStmt>(condition, then_branch, nullptr)
    );
}

// .....oooO0Oooo.....oooO0Oooo.....oooO0Oooo.....oooO0Oooo.....oooO0Oooo.....oooO0Oooo.....

auto
Parser::expression_statement(
) -> ErrorOr<std::shared_ptr<Statement>> {
    std::shared_ptr<Expression> expr = TRY(get_expression());
    TRY(consume(TokenType::SEMICOLON, ErrorType::EXPECTED_SEMICOLON));
    return std::static_pointer_cast<Statement>(std::make_shared<ExpressionStmt>(expr));
}

// .....oooO0Oooo.....oooO0Oooo.....oooO0Oooo.....oooO0Oooo.....oooO0Oooo.....oooO0Oooo.....

auto
Parser::get_expression(
) -> ErrorOr<std::shared_ptr<Expression>> {
    return assignment();
}

// .....oooO0Oooo.....oooO0Oooo.....oooO0Oooo.....oooO0Oooo.....oooO0Oooo.....oooO0Oooo.....

auto
Parser::assignment(
) -> ErrorOr<std::shared_ptr<Expression>> {
    std::shared_ptr<Expression> left_expression = TRY(or_expression());
    if (match(TokenType::EQUAL)) {
        std::shared_ptr<Token>      equals = previous();
        std::shared_ptr<Expression> value  = TRY_LOX(assignment(), previous());
        if (value->expression_type == ExpressionType::VARIABLE) {
            Token name = std::static_pointer_cast<Variable>(value)->name;
            std::shared_ptr<Assignment> result = std::make_shared<Assignment>(
                std::move(name), value
            );
            return std::static_pointer_cast<Expression>(result);
        }
        return ErrorType::EXPECTED_ASSIGNMENT_TARGET;
    }
    return left_expression;
}

// .....oooO0Oooo.....oooO0Oooo.....oooO0Oooo.....oooO0Oooo.....oooO0Oooo.....oooO0Oooo.....

auto
Parser::or_expression(
) -> ErrorOr<std::shared_ptr<Expression>> {
    std::shared_ptr<Expression> left_expression = TRY_LOX(and_expression(), previous());
    while (match(TokenType::OR)) {
        std::shared_ptr<Token>      operation = previous();
        std::shared_ptr<Expression> right     = TRY_LOX(and_expression(), previous());
        left_expression = std::static_pointer_cast<Expression>(
            std::make_shared<Logical>(left_expression, operation, right)
        );
    }
    return left_expression;
}

// .....oooO0Oooo.....oooO0Oooo.....oooO0Oooo.....oooO0Oooo.....oooO0Oooo.....oooO0Oooo.....

auto 
Parser::and_expression(
) -> ErrorOr<std::shared_ptr<Expression>> {
    std::shared_ptr<Expression> left_expression = TRY_LOX(equality(), previous());
    while (match(TokenType::AND)) {
        std::shared_ptr<Token>      operation = previous();
        std::shared_ptr<Expression> right     = TRY_LOX(equality(), previous());
        left_expression = std::static_pointer_cast<Expression>(
            std::make_shared<Logical>(left_expression, operation, right)
        );
    }
    return left_expression;
}

// .....oooO0Oooo.....oooO0Oooo.....oooO0Oooo.....oooO0Oooo.....oooO0Oooo.....oooO0Oooo.....

auto
Parser::equality(
) -> ErrorOr<std::shared_ptr<Expression>> {
    std::shared_ptr<Expression> left_expression = TRY_LOX(comparison(), previous());
    while (match(TokenType::BANG_EQUAL, TokenType::EQUAL_EQUAL)) {
        std::shared_ptr<Token>      operation        = previous();
        std::shared_ptr<Expression> right_expression = TRY_LOX(comparison(), previous());
        left_expression = std::make_shared<Binary>(
            left_expression, operation, right_expression
        );
    }
    return std::static_pointer_cast<Expression>(left_expression);
}

// .....oooO0Oooo.....oooO0Oooo.....oooO0Oooo.....oooO0Oooo.....oooO0Oooo.....oooO0Oooo.....

auto
Parser::comparison(
) -> ErrorOr<std::shared_ptr<Expression>> {
    std::shared_ptr<Expression> left_expression = TRY_LOX(term(), previous());
    while (match(
        TokenType::GREATER, TokenType::GREATER_EQUAL,
        TokenType::LESS, TokenType::LESS_EQUALS)
    ) {
        std::shared_ptr<Token>      operation        = previous();
        std::shared_ptr<Expression> right_expression = TRY_LOX(term(), previous()); 
        left_expression = std::make_shared<Binary>(
            left_expression, operation, right_expression
        );
    }
    return std::static_pointer_cast<Expression>(left_expression);
}

// .....oooO0Oooo.....oooO0Oooo.....oooO0Oooo.....oooO0Oooo.....oooO0Oooo.....oooO0Oooo.....

auto
Parser::term(
) -> ErrorOr<std::shared_ptr<Expression>> {
    std::shared_ptr<Expression> left_expression = TRY_LOX(factor(), previous());
    while (match(TokenType::MINUS, TokenType::PLUS)) {
        std::shared_ptr<Token>      operation        = previous();
        std::shared_ptr<Expression> right_expression = TRY_LOX(unary(), previous());
        left_expression = std::make_shared<Binary>(
            left_expression, operation, right_expression
        );
    }
    return std::static_pointer_cast<Expression>(left_expression);
}

// .....oooO0Oooo.....oooO0Oooo.....oooO0Oooo.....oooO0Oooo.....oooO0Oooo.....oooO0Oooo.....

auto
Parser::factor(
) -> ErrorOr<std::shared_ptr<Expression>> {
    std::shared_ptr<Expression> left_expression = TRY_LOX(unary(), previous());
    while (match(TokenType::SLASH, TokenType::STAR)) {
        std::shared_ptr<Token>      operation        = previous();
        std::shared_ptr<Expression> right_expression = TRY_LOX(factor(), previous());
        left_expression = std::make_shared<Binary>(
            left_expression, operation, right_expression
        );
    }
    return std::static_pointer_cast<Expression>(left_expression);
}

// .....oooO0Oooo.....oooO0Oooo.....oooO0Oooo.....oooO0Oooo.....oooO0Oooo.....oooO0Oooo.....

auto
Parser::unary(
) -> ErrorOr<std::shared_ptr<Expression>> {
    if (match(TokenType::BANG, TokenType::MINUS)) {
        std::shared_ptr<Token> operation             = previous();
        std::shared_ptr<Expression> right_expression = TRY_LOX(unary(), previous());
        return std::static_pointer_cast<Expression>(
            std::make_shared<Unary>(operation, right_expression)
        );
    }
    return primary();
}

// .....oooO0Oooo.....oooO0Oooo.....oooO0Oooo.....oooO0Oooo.....oooO0Oooo.....oooO0Oooo.....

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
        std::shared_ptr<Expression> expr = TRY_LOX(get_expression(), previous());
        TRY_LOX(consume(TokenType::RIGHT_PAREN, ErrorType::CLOSING_PAREN), previous());
        return std::static_pointer_cast<Expression>(std::make_shared<Grouping>(expr));
    }
    return ErrorType::EXPECTED_EXPRESSION;
}

// .....oooO0Oooo.....oooO0Oooo.....oooO0Oooo.....oooO0Oooo.....oooO0Oooo.....oooO0Oooo.....

auto 
Parser::consume(
    TokenType token_type,
    ErrorType error_type
) -> ErrorOr<std::shared_ptr<Token>> {
    if (check(token_type)) return advance();
    else return error_type;
}

// .....oooO0Oooo.....oooO0Oooo.....oooO0Oooo.....oooO0Oooo.....oooO0Oooo.....oooO0Oooo.....

auto
Parser::check(
    TokenType token_type
) -> bool {
    if (not_end()) return peek().type == token_type;
    else return false;
}

// .....oooO0Oooo.....oooO0Oooo.....oooO0Oooo.....oooO0Oooo.....oooO0Oooo.....oooO0Oooo.....

auto 
Parser::advance(
) -> std::shared_ptr<Token> {
    if (not_end()) ++m_current;
    return previous();
}

// .....oooO0Oooo.....oooO0Oooo.....oooO0Oooo.....oooO0Oooo.....oooO0Oooo.....oooO0Oooo.....

auto
Parser::previous(
) -> std::shared_ptr<Token> {
    return std::make_shared<Token>(m_tokens[m_current - 1]);
}

// .....oooO0Oooo.....oooO0Oooo.....oooO0Oooo.....oooO0Oooo.....oooO0Oooo.....oooO0Oooo.....

auto
Parser::peek(
) -> Token const& {
    return m_tokens[m_current];
}

// .....oooO0Oooo.....oooO0Oooo.....oooO0Oooo.....oooO0Oooo.....oooO0Oooo.....oooO0Oooo.....

auto
Parser::not_end(
) -> bool {
    return !is_end();
}

// .....oooO0Oooo.....oooO0Oooo.....oooO0Oooo.....oooO0Oooo.....oooO0Oooo.....oooO0Oooo.....

auto 
Parser::is_end(
) -> bool {
    return peek().type == TokenType::ENDOFFILE;
}
