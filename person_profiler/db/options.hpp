#pragma once

#include <string>


void save_option(std::string const& option_name, std::string const& option_value);
std::string get_option(std::string const& option_name);