#pragma once

#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

struct Delimiter {
    std::string delimiter = " ";
};

template<typename T>
inline auto
operator<<(
    std::ostream& os, 
    std::vector<T> const& vec
) -> std::ostream& {
    os << "[" << std::endl;
    for (auto const& entry : vec)
        os << "  " << entry << std::endl;
    os << "]";
    return os;
}

template<typename Key, typename Value>
inline auto
operator<<(
    std::ostream& os,
    std::unordered_map<Key, Value> map
) -> std::ostream& {
    os << "[" << std::endl;
    for (auto const& [key, value] : map)
        os << "  " << key << ": " << value << std::endl;
    os  << "]";
    return os;
}


template <typename TArg, typename... Args>
inline auto
print(
    TArg first,
    Args&&... args
) -> void {
    if constexpr (std::is_same_v<TArg, Delimiter>)
        ((std::cout << std::forward<Args>(args) << first.delimiter), ...);
    else {
        std::cout << std::forward<TArg>(first) << " ";
        ((std::cout << std::forward<Args>(args) << " "), ...);
    }
    std::cout << std::endl;
}

inline auto
newline(
) -> void {
    std::cout << std::endl;
}