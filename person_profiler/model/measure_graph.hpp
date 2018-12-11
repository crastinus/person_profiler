#pragma once

#include "estimation.hpp"
#include "value.hpp"
#include <vector>
#include <unordered_map>

// For making proper sugar functions
struct measure_graph : public std::unordered_map<measure_group_id, std::vector<value>, measure_group_id_hash> {};