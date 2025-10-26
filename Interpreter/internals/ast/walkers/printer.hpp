#pragma once

#include "internals/object.hpp"

#include "internals/ast/environment.hpp"
#include "internals/ast/expression.hpp"
#include "internals/ast/statement.hpp"

#include "internals/ast/walkers/interpreter.hpp"

#include "util/error.hpp"
#include "util/print.hpp"

class [[nodiscard]] Printer : public Interpreter {
public:
    Printer() = default;

    virtual ErrorOr<Object> interpret(Assignment* expression) override;
    virtual ErrorOr<Object> interpret(Binary* expression) override;
    virtual ErrorOr<Object> interpret(Call* expression) override;
    virtual ErrorOr<Object> interpret(Grouping* expression) override;
    virtual ErrorOr<Object> interpret(Literal* expression) override;
    virtual ErrorOr<Object> interpret(Logical* expression) override;
    virtual ErrorOr<Object> interpret(Unary* expression) override;
    virtual ErrorOr<Object> interpret(Variable* expression) override;

    virtual ErrorOr<void> interpret(Block* statements) override;
    virtual ErrorOr<void> interpret(IfStmt* statement) override;
    virtual ErrorOr<void> interpret(ExpressionStmt* statement) override;
    virtual ErrorOr<void> interpret(FunDeclStmt* statement) override;
    virtual ErrorOr<void> interpret(PrintStmt* statement) override;
    virtual ErrorOr<void> interpret(VarDeclStmt* statement) override;
    virtual ErrorOr<void> interpret(WhileStmt* statement) override;
    virtual ErrorOr<void> interpret(ReturnStmt* statement) override;

private:
    int m_indentation = 0; 

    void increment_indent() { ++m_indentation; }
    void decrement_indent() { --m_indentation; }

    template<typename... Args>
    void print(Args&&... args);
};

template<typename... Args>
auto
Printer::print(
    Args&&... args
) -> void {
    for (int i = 0; i < m_indentation; ++i)
        std::cout << "  ";
    ::print(std::forward<Args>(args)...);
}




