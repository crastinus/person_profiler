#include "trim.hpp"
#include <algorithm>

// trim from start
std::string &ltrim(std::string &s) {
    s.erase(s.begin(), std::find_if(s.begin(), s.end(),  [](char c) { return !::isspace(c); }));
    return s;
}

// trim from end
std::string &rtrim(std::string &s) {
    while (!s.empty() && ::isspace(s.back())) {
        s.pop_back();
    }
    return s;
}

// trim from both ends
std::string &trim(std::string &s) {
    return ltrim(rtrim(s));
}