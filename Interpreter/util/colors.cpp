#include "util/colors.hpp"

auto
operator<<(
    std::ostream& out,
    Color const& color
) -> std::ostream& {
    switch(color) {
        case Color::RED:     out << "\033[31m"; break;
        case Color::GREEN:   out << "\033[32m"; break;
        case Color::YELLOW:  out << "\033[33m"; break;
        case Color::BLUE:    out << "\033[34m"; break;
        case Color::MAGNETA: out << "\033[35m"; break;
        case Color::CYAN:    out << "\033[36m"; break;
        case Color::WHITE:   out << "\033[37m"; break;
        case Color::DEFAULT: out << "\033[0m";  break;
    }
    return out;
}
