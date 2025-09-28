#include "internals/parse.hpp"

#include "internals/ast.hpp"

#include "internals/object.hpp"
#include "internals/token.hpp"

#include "internals/ast/expression.hpp"
#include "internals/ast/expressions/assignment.hpp"
#include "internals/ast/expressions/binary.hpp"
#include "internals/ast/expressions/expression_type.hpp"
#include "internals/ast/expressions/grouping.hpp"
#include "internals/ast/expressions/literal.hpp"
#include "internals/ast/expressions/logical.hpp"
#include "internals/ast/expressions/unary.hpp"
#include "internals/ast/expressions/variable.hpp"

#include "internals/ast/statement.hpp"
#include "internals/ast/statements/block.hpp"
#include "internals/ast/statements/if_stmt.hpp"
#include "internals/ast/statements/expression_stmt.hpp"
#include "internals/ast/statements/print_stmt.hpp"
#include "internals/ast/statements/statement_type.hpp"
#include "internals/ast/statements/var_decl_stmt.hpp"
#include "internals/ast/statements/while_stmt.hpp"

#include "util/try.hpp"



Parser::Parser(
    std::vector<Token>&& tokens
) 
    : m_tokens(tokens)
{}

// .....oooO0Oooo.....oooO0Oooo.....oooO0Oooo.....oooO0Oooo.....oooO0Oooo.....oooO0Oooo.....

auto 
Parser::parse(
) -> ErrorOr<std::vector<std::unique_ptr<Statement>>> {
    std::vector<std::unique_ptr<Statement>> statements;
    while (!is_end()) {
        std::unique_ptr<Statement> stmt = TRY_LOX(declaration(), previous());
        statements.push_back(std::move(stmt));
    }
    return statements;
}

// .....oooO0Oooo.....oooO0Oooo.....oooO0Oooo.....oooO0Oooo.....oooO0Oooo.....oooO0Oooo.....

auto
Parser::declaration(
) -> ErrorOr<std::unique_ptr<Statement>> {
    if (match(TokenType::VAR)) return TRY_LOX(variable_declaration(), previous());
    return TRY_LOX(get_statement(), previous());
}

// .....oooO0Oooo.....oooO0Oooo.....oooO0Oooo.....oooO0Oooo.....oooO0Oooo.....oooO0Oooo.....

auto 
Parser::variable_declaration(
) -> ErrorOr<std::unique_ptr<Statement>> {
    Token name = *TRY_LOX(
        consume(TokenType::IDENTIFIER, ErrorType::EXPECTED_VARIABLE_NAME), previous()
    );
    std::unique_ptr<Expression> initiailizer;
    if (match(TokenType::EQUAL)) {
        initiailizer = TRY_LOX(get_expression(), previous());
    }
    TRY_LOX(consume(TokenType::SEMICOLON, ErrorType::EXPECTED_SEMICOLON), previous());
    return ErrorOr<std::unique_ptr<Statement>>(
        std::make_unique<VarDeclStmt>(name, std::move(initiailizer))
    );
}

// .....oooO0Oooo.....oooO0Oooo.....oooO0Oooo.....oooO0Oooo.....oooO0Oooo.....oooO0Oooo.....

auto
Parser::get_statement(
) -> ErrorOr<std::unique_ptr<Statement>> {
    if (match(TokenType::FOR)) return TRY_LOX(for_statement(), previous());
    if (match(TokenType::IF)) return TRY_LOX(if_statement(), previous());
    if (match(TokenType::PRINT)) return TRY_LOX(print_statement(), previous());
    if (match(TokenType::WHILE)) return TRY_LOX(while_statement(), previous());
    if (match(TokenType::LEFT_BRACE)) {
        std::vector<std::unique_ptr<Statement>> stmts = TRY_LOX(get_block(), previous());
        return ErrorOr<std::unique_ptr<Statement>>(
            std::make_unique<Block>(std::move(stmts))
        );
    }
    return TRY_LOX(expression_statement(), previous());
}

// .....oooO0Oooo.....oooO0Oooo.....oooO0Oooo.....oooO0Oooo.....oooO0Oooo.....oooO0Oooo.....

auto
Parser::get_block(
) -> ErrorOr<std::vector<std::unique_ptr<Statement>>> {
    std::vector<std::unique_ptr<Statement>> statements;
    while (!check(TokenType::RIGHT_BRACE) && not_end()) {
        std::unique_ptr<Statement> stmt = TRY_LOX(declaration(), previous());
        statements.push_back(std::move(stmt));
    }
    TRY_LOX(consume(TokenType::RIGHT_BRACE, ErrorType::EXPECTED_RIGHT_BRACE), previous());
    return statements;
}

// .....oooO0Oooo.....oooO0Oooo.....oooO0Oooo.....oooO0Oooo.....oooO0Oooo.....oooO0Oooo.....

auto
Parser::print_statement(
) -> ErrorOr<std::unique_ptr<Statement>> {
    std::unique_ptr<Expression> expr = TRY_LOX(get_expression(), previous());
    TRY_LOX(consume(TokenType::SEMICOLON, ErrorType::EXPECTED_SEMICOLON), previous());
    return ErrorOr<std::unique_ptr<Statement>>(std::make_unique<PrintStmt>(std::move(expr)));
}

// .....oooO0Oooo.....oooO0Oooo.....oooO0Oooo.....oooO0Oooo.....oooO0Oooo.....oooO0Oooo.....

auto
Parser::for_statement(
) -> ErrorOr<std::unique_ptr<Statement>> {
    TRY_LOX(consume(TokenType::LEFT_PAREN, ErrorType::EXPECTED_LEFT_PAREN), previous());
    std::unique_ptr<Statement> initializer = nullptr;
    if (match(TokenType::SEMICOLON)) initializer = nullptr;
    else if (match(TokenType::VAR)) initializer = TRY_LOX(variable_declaration(), previous());
    else initializer = TRY_LOX(expression_statement(), previous());
    std::unique_ptr<Expression> condition = nullptr;
    if (!check(TokenType::SEMICOLON)) condition = TRY_LOX(get_expression(), previous());
    TRY_LOX(consume(TokenType::SEMICOLON, ErrorType::EXPECTED_SEMICOLON), previous());
    std::unique_ptr<Expression> increment = nullptr;
    if (!check(TokenType::RIGHT_PAREN)) increment = TRY_LOX(get_expression(), previous());
    TRY_LOX(consume(TokenType::RIGHT_PAREN, ErrorType::EXPECTED_RIGHT_PAREN), previous());
    std::unique_ptr<Statement> body = TRY_LOX(get_statement(), previous());
    // Begin desugaring for loop
    if (increment) {
        std::vector<std::unique_ptr<Statement>> statements;
        statements.push_back(std::move(body));
        statements.push_back(std::make_unique<ExpressionStmt>(std::move(increment)));
        body = std::make_unique<Block>(std::move(statements));
    }
    if (condition == nullptr) condition = std::make_unique<Literal>(Object{true});
    body = std::make_unique<WhileStmt>(std::move(condition), std::move(body));
    if (initializer) {
        std::vector<std::unique_ptr<Statement>> statements;
        statements.push_back(std::move(initializer));
        statements.push_back(std::move(body));
        body = std::make_unique<Block>(std::move(statements));
    }
    return body;
}

// .....oooO0Oooo.....oooO0Oooo.....oooO0Oooo.....oooO0Oooo.....oooO0Oooo.....oooO0Oooo.....

auto
Parser::if_statement(
) -> ErrorOr<std::unique_ptr<Statement>> {
   TRY_LOX(consume(TokenType::LEFT_PAREN, ErrorType::EXPECTED_LEFT_PAREN), previous()); 
    std::unique_ptr<Expression> condition = TRY_LOX(get_expression(), previous());
    TRY_LOX(consume(TokenType::RIGHT_PAREN, ErrorType::EXPECTED_RIGHT_PAREN), previous());
    std::unique_ptr<Statement> then_branch = TRY_LOX(get_statement(), previous());
    if (match(TokenType::ELSE)) {
        std::unique_ptr<Statement> else_branch = TRY_LOX(get_statement(), previous());
        return ErrorOr<std::unique_ptr<Statement>>(
            std::make_unique<IfStmt>(
                std::move(condition), 
                std::move(then_branch), 
                std::move(else_branch)
            )
        );
    }
    return ErrorOr<std::unique_ptr<Statement>>(
        std::make_unique<IfStmt>(std::move(condition), std::move(then_branch), nullptr)
    );
}

// .....oooO0Oooo.....oooO0Oooo.....oooO0Oooo.....oooO0Oooo.....oooO0Oooo.....oooO0Oooo.....

auto
Parser::while_statement(
) -> ErrorOr<std::unique_ptr<Statement>> {
    TRY_LOX(consume(TokenType::LEFT_PAREN, ErrorType::EXPECTED_LEFT_PAREN), previous());
    std::unique_ptr<Expression> condition = TRY_LOX(get_expression(), previous());
    TRY_LOX(consume(TokenType::RIGHT_PAREN, ErrorType::EXPECTED_RIGHT_PAREN), previous());
    std::unique_ptr<Statement> body = TRY_LOX(get_statement(), previous());
    return ErrorOr<std::unique_ptr<Statement>>(
        std::make_unique<WhileStmt>(std::move(condition), std::move(body))
    );
}

// .....oooO0Oooo.....oooO0Oooo.....oooO0Oooo.....oooO0Oooo.....oooO0Oooo.....oooO0Oooo.....

auto
Parser::expression_statement(
) -> ErrorOr<std::unique_ptr<Statement>> {
    std::unique_ptr<Expression> expr = TRY_LOX(get_expression(), previous());
    TRY(consume(TokenType::SEMICOLON, ErrorType::EXPECTED_SEMICOLON));
    return ErrorOr<std::unique_ptr<Statement>>(std::make_unique<ExpressionStmt>(std::move(expr)));
}

// .....oooO0Oooo.....oooO0Oooo.....oooO0Oooo.....oooO0Oooo.....oooO0Oooo.....oooO0Oooo.....

auto
Parser::get_expression(
) -> ErrorOr<std::unique_ptr<Expression>> {
    return TRY_LOX(assignment(), previous());
}

// .....oooO0Oooo.....oooO0Oooo.....oooO0Oooo.....oooO0Oooo.....oooO0Oooo.....oooO0Oooo.....

auto
Parser::assignment(
) -> ErrorOr<std::unique_ptr<Expression>> {
    std::unique_ptr<Expression> left_expression = TRY_LOX(or_expression(), previous());
    if (match(TokenType::EQUAL)) {
        std::unique_ptr<Token>      equals = previous();
        std::unique_ptr<Expression> value  = TRY_LOX(assignment(), previous());
        if (left_expression->expression_type == ExpressionType::VARIABLE) {
            Token name = static_cast<Variable*>(left_expression.get())->name;
            std::unique_ptr<Assignment> result = std::make_unique<Assignment>(
                std::move(name), std::move(value)
            );
            return ErrorOr<std::unique_ptr<Expression>>(std::move(result));
        }
        return ErrorType::EXPECTED_ASSIGNMENT_TARGET;
    }
    return left_expression;
}

// .....oooO0Oooo.....oooO0Oooo.....oooO0Oooo.....oooO0Oooo.....oooO0Oooo.....oooO0Oooo.....

auto
Parser::or_expression(
) -> ErrorOr<std::unique_ptr<Expression>> {
    std::unique_ptr<Expression> left_expression = TRY_LOX(and_expression(), previous());
    while (match(TokenType::OR)) {
        std::unique_ptr<Token>      operation = previous();
        std::unique_ptr<Expression> right     = TRY_LOX(and_expression(), previous());
        left_expression = std::make_unique<Logical>(
            std::move(left_expression), std::move(operation), std::move(right)
        );
    }
    return left_expression;
}

// .....oooO0Oooo.....oooO0Oooo.....oooO0Oooo.....oooO0Oooo.....oooO0Oooo.....oooO0Oooo.....

auto 
Parser::and_expression(
) -> ErrorOr<std::unique_ptr<Expression>> {
    std::unique_ptr<Expression> left_expression = TRY_LOX(equality(), previous());
    while (match(TokenType::AND)) {
        std::unique_ptr<Token>      operation = previous();
        std::unique_ptr<Expression> right     = TRY_LOX(equality(), previous());
        left_expression = std::make_unique<Logical>(
            std::move(left_expression), std::move(operation), std::move(right)
        );
    }
    return left_expression;
}

// .....oooO0Oooo.....oooO0Oooo.....oooO0Oooo.....oooO0Oooo.....oooO0Oooo.....oooO0Oooo.....

auto
Parser::equality(
) -> ErrorOr<std::unique_ptr<Expression>> {
    std::unique_ptr<Expression> left_expression = TRY_LOX(comparison(), previous());
    while (match(TokenType::BANG_EQUAL, TokenType::EQUAL_EQUAL)) {
        std::unique_ptr<Token>      operation        = previous();
        std::unique_ptr<Expression> right_expression = TRY_LOX(comparison(), previous());
        left_expression = std::make_unique<Binary>(
            std::move(left_expression), std::move(operation), std::move(right_expression)
        );
    }
    return ErrorOr<std::unique_ptr<Expression>>(std::move(left_expression));
}

// .....oooO0Oooo.....oooO0Oooo.....oooO0Oooo.....oooO0Oooo.....oooO0Oooo.....oooO0Oooo.....

auto
Parser::comparison(
) -> ErrorOr<std::unique_ptr<Expression>> {
    std::unique_ptr<Expression> left_expression = TRY_LOX(term(), previous());
    while (match(
        TokenType::GREATER, TokenType::GREATER_EQUAL,
        TokenType::LESS, TokenType::LESS_EQUALS)
    ) {
        std::unique_ptr<Token>      operation        = previous();
        std::unique_ptr<Expression> right_expression = TRY_LOX(term(), previous()); 
        left_expression = std::make_unique<Binary>(
            std::move(left_expression), std::move(operation), std::move(right_expression)
        );
    }
    return ErrorOr<std::unique_ptr<Expression>>(std::move(left_expression));
}

// .....oooO0Oooo.....oooO0Oooo.....oooO0Oooo.....oooO0Oooo.....oooO0Oooo.....oooO0Oooo.....

auto
Parser::term(
) -> ErrorOr<std::unique_ptr<Expression>> {
    std::unique_ptr<Expression> left_expression = TRY_LOX(factor(), previous());
    while (match(TokenType::MINUS, TokenType::PLUS)) {
        std::unique_ptr<Token>      operation        = previous();
        std::unique_ptr<Expression> right_expression = TRY_LOX(unary(), previous());
        left_expression = std::make_unique<Binary>(
            std::move(left_expression), std::move(operation), std::move(right_expression)
        );
    }
    return ErrorOr<std::unique_ptr<Expression>>(std::move(left_expression));
}

// .....oooO0Oooo.....oooO0Oooo.....oooO0Oooo.....oooO0Oooo.....oooO0Oooo.....oooO0Oooo.....

auto
Parser::factor(
) -> ErrorOr<std::unique_ptr<Expression>> {
    std::unique_ptr<Expression> left_expression = TRY_LOX(unary(), previous());
    while (match(TokenType::SLASH, TokenType::STAR)) {
        std::unique_ptr<Token>      operation        = previous();
        std::unique_ptr<Expression> right_expression = TRY_LOX(factor(), previous());
        left_expression = std::make_unique<Binary>(
            std::move(left_expression), std::move(operation), std::move(right_expression)
        );
    }
    return ErrorOr<std::unique_ptr<Expression>>(std::move(left_expression));
}

// .....oooO0Oooo.....oooO0Oooo.....oooO0Oooo.....oooO0Oooo.....oooO0Oooo.....oooO0Oooo.....

auto
Parser::unary(
) -> ErrorOr<std::unique_ptr<Expression>> {
    if (match(TokenType::BANG, TokenType::MINUS)) {
        std::unique_ptr<Token> operation             = previous();
        std::unique_ptr<Expression> right_expression = TRY_LOX(unary(), previous());
        return ErrorOr<std::unique_ptr<Expression>>(
            std::make_unique<Unary>(std::move(operation), std::move(right_expression))
        );
    }
    return primary();
}

// .....oooO0Oooo.....oooO0Oooo.....oooO0Oooo.....oooO0Oooo.....oooO0Oooo.....oooO0Oooo.....

auto
Parser::primary(
) -> ErrorOr<std::unique_ptr<Expression>> {
    if (match(TokenType::FALSE)) 
        return ErrorOr<std::unique_ptr<Expression>>(std::make_unique<Literal>(Object(false)));
    if (match(TokenType::TRUE))
        return ErrorOr<std::unique_ptr<Expression>>(std::make_unique<Literal>(Object(true)));
    if (match(TokenType::NIL))
        return ErrorOr<std::unique_ptr<Expression>>(std::make_unique<Literal>(Object(nil)));
    if (match(TokenType::NUMBER, TokenType::STRING)) {
        return ErrorOr<std::unique_ptr<Expression>>( 
            std::make_unique<Literal>(previous()->literal)
        );
    }
    if (match(TokenType::IDENTIFIER))
        return ErrorOr<std::unique_ptr<Expression>>(std::make_unique<Variable>(previous()));
    if (match(TokenType::LEFT_PAREN)) {
        std::unique_ptr<Expression> expr = TRY_LOX(get_expression(), previous());
        TRY_LOX(consume(TokenType::RIGHT_PAREN, ErrorType::CLOSING_PAREN), previous());
        return ErrorOr<std::unique_ptr<Expression>>(std::move(expr));
    }
    return ErrorType::EXPECTED_EXPRESSION;
}

// .....oooO0Oooo.....oooO0Oooo.....oooO0Oooo.....oooO0Oooo.....oooO0Oooo.....oooO0Oooo.....

auto 
Parser::consume(
    TokenType token_type,
    ErrorType error_type
) -> ErrorOr<std::unique_ptr<Token>> {
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
) -> std::unique_ptr<Token> {
    if (not_end()) ++m_current;
    return previous();
}

// .....oooO0Oooo.....oooO0Oooo.....oooO0Oooo.....oooO0Oooo.....oooO0Oooo.....oooO0Oooo.....

auto
Parser::previous(
) -> std::unique_ptr<Token> {
    if (m_current > 0)
        return std::make_unique<Token>(m_tokens[m_current - 1]);
    else
        return std::make_unique<Token>(m_tokens[0]);
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
