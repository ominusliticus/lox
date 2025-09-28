#pragma once

#include <memory>
#include <vector>
 
#include "internals/object.hpp"
#include "internals/token.hpp"

#include "internals/ast/environment.hpp"
#include "internals/ast/statement.hpp"
#include "internals/ast/walkers/interpreter.hpp"
#include "internals/ast/walkers/printer.hpp"

#include "util/error.hpp"


// TDOO: Our type system is not as convenient as the Java one.
//       We need to create a class heirarchy that allows us to automate much of the visiting down
//       the road.
// TODO: Add documentation strings


class AST
{
public:
    AST() = default;

    ErrorOr<void> interpret(std::vector<std::unique_ptr<Statement>>&& statements);
    // ErrorOr<void> print(std::vector<std::unique_ptr<Statement>>&& statements);

private:
    static std::unique_ptr<Interpreter> m_interpreter;
    static std::unique_ptr<Printer>     m_printer;
};
