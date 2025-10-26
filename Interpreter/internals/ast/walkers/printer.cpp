#include "internals/ast/walkers/printer.hpp"
#include "internals/ast/walkers/function.hpp"
#include "internals/ast/walkers/return.hpp"

#include "internals/object.hpp"
#include "internals/token.hpp"

#include "internals/ast/environment.hpp"

#include "internals/ast/expression.hpp"
#include "internals/ast/expressions/assignment.hpp"
#include "internals/ast/expressions/binary.hpp"
#include "internals/ast/expressions/call.hpp"
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
#include "internals/ast/statements/fun_decl_stmt.hpp"
#include "internals/ast/statements/print_stmt.hpp"
#include "internals/ast/statements/return_stmt.hpp"
#include "internals/ast/statements/statement_type.hpp"
#include "internals/ast/statements/var_decl_stmt.hpp"
#include "internals/ast/statements/while_stmt.hpp"

#include "util/try.hpp"

auto 
Printer::interpret(
	Assignment* expression
) -> ErrorOr<Object> {
    this->print("Assignment");
    increment_indent();
    this->print(expression->name.lexeme);
    TRY(expression->value->visit(this));
    decrement_indent();
	return Object(nil);
}

// .....oooO0Oooo.....oooO0Oooo.....oooO0Oooo.....oooO0Oooo.....oooO0Oooo.....oooO0Ooo.....

auto 
Printer::interpret(
	Binary* expression
) -> ErrorOr<Object> {
    this->print("Binary");
    increment_indent();
    TRY(expression->left_expression->visit(this));
    this->print(expression->operation->lexeme);
    TRY(expression->right_expression->visit(this));
    decrement_indent();
	return Object(nil);
}

// .....oooO0Oooo.....oooO0Oooo.....oooO0Oooo.....oooO0Oooo.....oooO0Oooo.....oooO0Ooo.....

auto 
Printer::interpret(
	Call* expression
) -> ErrorOr<Object> {
    this->print("Call");
    increment_indent();
    TRY(expression->callee->visit(this));
    this->print("Arguments");
    increment_indent();
    for (auto const& argument : expression->arguments)
        TRY(argument->visit(this));
    decrement_indent();
    decrement_indent();
	return Object(nil);
}

// .....oooO0Oooo.....oooO0Oooo.....oooO0Oooo.....oooO0Oooo.....oooO0Oooo.....oooO0Ooo.....

auto 
Printer::interpret(
	Grouping* expression
) -> ErrorOr<Object> {
    this->print("Grouping");
    increment_indent();
    TRY(expression->left_expression->visit(this));
    decrement_indent();
	return Object(nil);
}

// .....oooO0Oooo.....oooO0Oooo.....oooO0Oooo.....oooO0Oooo.....oooO0Oooo.....oooO0Ooo.....

auto 
Printer::interpret(
	Literal* expression
) -> ErrorOr<Object> {
    this->print("Literal");
    increment_indent();
    this->print(expression->object);
    decrement_indent();
	return Object(nil);
}

// .....oooO0Oooo.....oooO0Oooo.....oooO0Oooo.....oooO0Oooo.....oooO0Oooo.....oooO0Ooo.....

auto 
Printer::interpret(
	Logical* expression
) -> ErrorOr<Object> {
    this->print("Logical");
    increment_indent();
    TRY(expression->left_expression->visit(this));
    this->print(expression->operation->lexeme);
    TRY(expression->right_expression->visit(this));
    decrement_indent();
	return Object(nil);
}

// .....oooO0Oooo.....oooO0Oooo.....oooO0Oooo.....oooO0Oooo.....oooO0Oooo.....oooO0Ooo.....

auto 
Printer::interpret(
	Unary* expression
) -> ErrorOr<Object> {
    this->print("Unary");
    increment_indent();
    this->print(expression->operation->lexeme);
    TRY(expression->right_expression->visit(this));
    decrement_indent();
	return Object(nil);
}

// .....oooO0Oooo.....oooO0Oooo.....oooO0Oooo.....oooO0Oooo.....oooO0Oooo.....oooO0Ooo.....

auto 
Printer::interpret(
	Variable* expression
) -> ErrorOr<Object> {
    this->print("Variable");
    increment_indent();
    this->print(expression->name.lexeme);
    decrement_indent();
	return Object(nil);
}

// .....oooO0Oooo.....oooO0Oooo.....oooO0Oooo.....oooO0Oooo.....oooO0Oooo.....oooO0Ooo.....

auto 
Printer::interpret(
	Block* statements
) -> ErrorOr<void> {
    this->print("Block");
    increment_indent();
    this->print("Statements");
    increment_indent();
    for (auto const& statement : statements->statements)
        TRY(statement->visit(this));
    decrement_indent();
    decrement_indent();
	return {};
}

// .....oooO0Oooo.....oooO0Oooo.....oooO0Oooo.....oooO0Oooo.....oooO0Oooo.....oooO0Ooo.....

auto 
Printer::interpret(
	IfStmt* statement
) -> ErrorOr<void> {
    this->print("If");
    increment_indent();
    TRY(statement->condition->visit(this));
    TRY(statement->then_branch->visit(this));
    if (statement->else_branch) TRY(statement->else_branch->visit(this));
    decrement_indent();
	return {};
}

// .....oooO0Oooo.....oooO0Oooo.....oooO0Oooo.....oooO0Oooo.....oooO0Oooo.....oooO0Ooo.....

auto 
Printer::interpret(
	ExpressionStmt* statement
) -> ErrorOr<void> {
    this->print("Expression Stmt");
    increment_indent();
    TRY(statement->expression->visit(this));
    decrement_indent();
	return {};
}

// .....oooO0Oooo.....oooO0Oooo.....oooO0Oooo.....oooO0Oooo.....oooO0Oooo.....oooO0Ooo.....

auto 
Printer::interpret(
	FunDeclStmt* statement
) -> ErrorOr<void> {
    this->print("Function Declaration");
    increment_indent();
    this->print(statement->name->lexeme);
    this->print("Parameters");
    increment_indent();
    for (auto const& parameter : statement->parameters)
        this->print(parameter->lexeme);
    decrement_indent();
    this->print("Statements");
    increment_indent();
    for (auto const& stmt : statement->body)
        TRY(stmt->visit(this));
    decrement_indent();
    decrement_indent();
	return {};
}

// .....oooO0Oooo.....oooO0Oooo.....oooO0Oooo.....oooO0Oooo.....oooO0Oooo.....oooO0Ooo.....

auto 
Printer::interpret(
	PrintStmt* statement
) -> ErrorOr<void> {
    this->print("Print");
    increment_indent();
    TRY(statement->expression->visit(this));
    decrement_indent();
	return {};
}

// .....oooO0Oooo.....oooO0Oooo.....oooO0Oooo.....oooO0Oooo.....oooO0Oooo.....oooO0Ooo.....

auto 
Printer::interpret(
	VarDeclStmt* statement
) -> ErrorOr<void> {
    this->print("Variable Declaration");
    increment_indent();
    this->print(statement->name.lexeme);
    if (statement->expression) TRY(statement->expression->visit(this));
    decrement_indent();
	return {};
}

// .....oooO0Oooo.....oooO0Oooo.....oooO0Oooo.....oooO0Oooo.....oooO0Oooo.....oooO0Ooo.....

auto 
Printer::interpret(
	WhileStmt* statement
) -> ErrorOr<void> {
    this->print("While");
    increment_indent();
    TRY(statement->condition->visit(this));
    TRY(statement->body->visit(this));
    decrement_indent();
	return {};
}

// .....oooO0Oooo.....oooO0Oooo.....oooO0Oooo.....oooO0Oooo.....oooO0Oooo.....oooO0Ooo.....

auto 
Printer::interpret(
	[[maybe_unused]] ReturnStmt* statement
) -> ErrorOr<void> {
    this->print("Return");
	return {};
}
