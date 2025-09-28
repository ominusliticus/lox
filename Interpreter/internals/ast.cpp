#include <memory>
#include <sstream>
#include <utility>
#include <variant>
#include <vector>
 
#include "util/print.hpp"
#include "util/try.hpp"

#include "internals/ast.hpp"

// TODO:
//  3. Align on identifier name
//  4. Alight on equal signs

std::unique_ptr<Interpreter> AST::m_interpreter = std::make_unique<Interpreter>();

auto
AST::interpret(
    std::vector<std::unique_ptr<Statement>>&& statements
) -> ErrorOr<void> {
    for (auto& statement : statements)
        TRY(m_interpreter->execute(statement.get()));
    return {};
}
