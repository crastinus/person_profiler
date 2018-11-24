#pragma once

#include <string>
#include <time.h>

std::string strtime(char const* templ, time_t time);
time_t  parse_time(char const* templ, std::string const& str);