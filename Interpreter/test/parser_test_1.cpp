#include <cassert>

#include "internals/ast.hpp"

auto
main(
    [[maybe_unused]] int    argc,
    [[maybe_unused]] char** argv
) -> int {
    auto expr = std::make_shared<Binary>(
        std::make_shared<Unary>(
            std::make_shared<Token>(TokenType::MINUS, "-", Object(), 1, 1),
            std::make_shared<Literal>(Object(123.0))
        ),
        std::make_shared<Token>(TokenType::STAR, "*", Object(), 1, 1),
        std::make_shared<Grouping>(
            std::make_shared<Literal>(Object(45.67))
        )
    );
    ExpressionVisitor visitor;
    std::stringstream ss;
    visitor.printer(ss, std::static_pointer_cast<Expression>(expr));
    assert("(* (- 123) (group 45.67))" == ss.str());
    return 0;
}
