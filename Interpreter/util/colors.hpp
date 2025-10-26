#pragma once

#include <iostream>

enum class Color {
    RED,
    GREEN,
    YELLOW,
    BLUE,
    MAGNETA,
    CYAN,
    WHITE,
    DEFAULT
};

std::ostream& operator<<(std::ostream& out, Color const& color);
