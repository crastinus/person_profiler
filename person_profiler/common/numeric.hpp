#pragma once

#include <stdexcept>
#include "concat.hpp"

template<typename ResultType, typename SourceType>
inline ResultType numeric_cast(SourceType&& src) {

    if (std::is_same<ResultType, SourceType>::value) {
        return src;
    }

    ResultType result = static_cast<ResultType>(src);
    if (result != src) {
        throw std::runtime_error(concat("Cannot convert to ", typeid(ResultType).name() ," from ", src));
    }
    return result;
}