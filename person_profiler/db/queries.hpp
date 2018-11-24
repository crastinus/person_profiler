#pragma once 

#include "day.hpp"

struct estimation;
struct measure;
struct value;
struct measure_group;

estimation req_estimation(int id);
measure req_measure(int id);
value req_value(int id);
measure_group req_measure_group(int id);
