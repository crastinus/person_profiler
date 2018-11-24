#pragma once

#include "day.hpp"
#include "estimation.hpp"

struct value {
    int id = 0;
    double val;
    day_id day;
    estimation_id estimation;
};