#include "to.hpp"
#include "concat.hpp"
#include "numeric.hpp"

template<typename ResultType, typename FunctionResultType>
ResultType to_number(FunctionResultType(cast)(char const*, char**, int), char const* str) {
    char* end;
    ResultType result = numeric_cast<ResultType>(cast(str, &end, 10));
    if (*end != '\0') {
        throw std::runtime_error(concat("Cannot convert string \"", str, "\" to type ", typeid(ResultType).name()));
    }
    return result;
}

template<>
int to<int>(char const* str) {
    return  to_number<int>(std::strtol, str);
}

template<>
unsigned to<unsigned>(char const* str) {
    return  to_number<unsigned>(std::strtoul, str);
}

template<>
unsigned short to<unsigned short>(char const* str) {
    return  to_number<unsigned short>(std::strtoul, str);
}


template<>
long to<long>(char const* str) {
    return to_number<long>(std::strtol, str);
}

template<>
long long to<long long>(char const* str) {
    return to_number<long long>(std::strtoll, str);
}

template<>
unsigned long to<unsigned long>(char const* str) {
    return to_number<unsigned long>(std::strtoul, str);
}

template<>
unsigned long long to<unsigned long long>(char const* str) {
    return to_number<unsigned long long>(std::strtoull, str);
}

template<>
double to<double>(char const* str) {
    return to_number<double, double>([](char const* str, char** result, int) {
        return std::strtod(str, result);
    }, str);
}
