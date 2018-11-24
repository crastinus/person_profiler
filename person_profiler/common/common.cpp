#include "common.hpp"
#include <iomanip>
#include <sstream>

std::string strtime(char const* templ, time_t raw_time) {

    tm t = {};
    gmtime_s(&t, &raw_time);

    char buffer[1024] = {};
    strftime(buffer, sizeof(buffer), templ, &t);

    return buffer;
}

time_t  parse_time(char const* templ, std::string const& str) {

    tm t;
    std::istringstream is(str);
    is >> std::get_time(&t, templ);

    return  mktime(&t);

}