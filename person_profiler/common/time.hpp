#pragma once

#include <time.h>
#include <string>

inline std::string format(char const* fm, time_t value) {
    struct tm t;
    gmtime_s(&t, &value);

    char buffer[80] = {};
    strftime(buffer, sizeof(buffer), fm, &t);
    return buffer;
}

inline time_t month(time_t timestamp = time(0), int shift = 0) {
    struct tm t;
    gmtime_s(&t, &timestamp);

    struct tm t2 = {};
    t2.tm_mday = 1;
    t2.tm_year = t.tm_year;
    t2.tm_mon = t.tm_mon + shift;

    return _mkgmtime(&t2);
}

inline time_t year(time_t timestamp = time(0), int shift = 0) {
    struct tm t;
    gmtime_s(&t, &timestamp);

    struct tm t2 = {};
    t2.tm_mday = 1;
    t2.tm_year = t.tm_year + shift;
    t2.tm_mon = t.tm_mon ;

    return _mkgmtime(&t2);
}
