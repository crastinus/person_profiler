#include "split.hpp"
#include "to.hpp"
#include "trim.hpp"
#include <algorithm>

std::vector<std::string>  split_any(std::string const& src, char const* separators, split_opts opts) {
    std::vector<std::string> result;
    result.reserve(src.size() / 8);

    auto it = src.begin();
    auto end = src.end();

    bool skip_empty = (opts == split_opts::skip_blank);

    auto matches = [](char findable, char const* seps) {
        while (*seps != '\0') {
            if (findable == *seps) {
                return true;
            }
            ++seps;
        }
        return false;
    };

    while (it != end) {

        // this one must be delete every possible empty string

        auto first_it = it;
        auto last_it = it;
        while (last_it != end) {
            if (matches(*last_it, separators)) {
                break;
            }
            ++last_it;
        }

        it = (last_it == end ? end : last_it + 1);

        if (first_it == last_it && skip_empty) {
            continue;
        }

        result.emplace_back(first_it, last_it);
    }

    return result;
}

template<typename Type>
std::vector<Type> split_to(std::string const& source_str, char const* separators) {
    std::vector<Type> result;

    auto src = split_any(source_str, separators, split_opts::skip_blank);
    result.reserve(src.size());

    for (std::string& value : src) {
        result.push_back(to<Type>(trim(value)));
    }

    return result;
}

template std::vector<double> split_to<double>(std::string const&, char const*);
template std::vector<int> split_to<int>(std::string const&, char const*);
template std::vector<unsigned> split_to<unsigned>(std::string const&, char const*);


