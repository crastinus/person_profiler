#pragma once

#include <vector>
#include <string>

enum class split_opts {
    allow_blank = 0,
    skip_blank = 1
};

std::vector<std::string> split_any(std::string const& source_str, char const* separators, split_opts options = split_opts::allow_blank);

template<typename Type>
std::vector<Type> split_to(std::string const& source_str, char const* separators);
