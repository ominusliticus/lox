#pragma once

#include <iostream>
#include <vector>

template<typename T>
auto
operator<<(
    std::ostream& os, 
    std::vector<T> const& vec
) -> std::ostream& {
    os << "[" << std::endl;
    for (auto const& entry : vec)
        os << '\t' << entry << std::endl;
    os << "]";
    return os;
}

template <typename... Args>
auto
print(
    Args&&... args
) -> void {
    ((std::cout << std::forward<Args>(args) << " "), ...);
    std::cout << std::endl;
}
