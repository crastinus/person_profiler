#pragma once

#include "string_streambuf.hpp"
#include <ostream>

template <typename Container>
inline std::string join_through(Container const& container, char const* separator) {
    std::string result;

    // long string size might be anything
    // container might not have a size function
    result.reserve(256);

    utils::string_streambuf sb(result);
    std::ostream            os(&sb);

    for (auto const& value : container) {
        if (&value != &*container.begin()) {
            os << separator;
        }
        os << value;
    }

    return result;
}