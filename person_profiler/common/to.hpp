#pragma once

#include <string>

template<typename T>
T to(char const* str);

template<typename T>
inline T to(std::string const& str) {
    return to<T>(str.c_str());
}