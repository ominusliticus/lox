#include "internals/ast/statements/printer.hpp"


namespace ast 
{

auto
printer(
    std::stringstream&          string_builder,
    std::shared_ptr<Expression> expression
) -> void {
    switch (expression->expression_type) {
        case ExpressionType::BINARY:
            string_builder << "(" << expression->operation->lexeme << " ";
            ast::printer(string_builder, expression->left_expression);
            string_builder << " ";
            ast::printer(string_builder, expression->right_expression);
            string_builder << ")";
            break;
        case ExpressionType::GROUPING:
            string_builder << "(group ";
            ast::printer(string_builder, expression->left_expression);
            string_builder << ")";
            break;
        case ExpressionType::LITERAL:
            string_builder << std::reinterpret_pointer_cast<Literal>(expression)->object;
            break;
        case ExpressionType::UNARY:
            string_builder << "(";
            string_builder << expression->operation->lexeme << " ";
            ast::printer(string_builder, expression->right_expression);
            string_builder << ")";
            break;
        default:
            string_builder << "Unhandled expression type\n";
            return;
    }
}

}
