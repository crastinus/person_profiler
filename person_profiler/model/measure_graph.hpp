#pragma once

#include "estimation.hpp"
#include "value.hpp"
#include <vector>
#include <unordered_map>

struct value_info {
    std::string value_name_;
    measure_type  type_;
    value value_;
    bool bool_value_;
    float float_value_;
};

// For making proper sugar functions
struct measure_values_graph : public std::unordered_map<measure_group_id, std::vector<value_info>, measure_group_id_hash> {};
struct measure_graph : public std::unordered_map<measure_group_id, std::vector<measure>, measure_group_id_hash> {};

template<typename T>
using mg_graph = std::unordered_map<measure_group_id, T, measure_group_id_hash>;