#pragma once

#include <string> 
#include "measure.hpp"

struct estimation;


struct estimation_id {
    int id;

    operator estimation const();
    operator bool const();

    estimation_id& operator = (int id);
};

struct estimation {
    int id = 0;
    time_t started; // day of start and end
    time_t expired;
    bool   active; // estimation is current root for measure
    double weight; // weight of this measure in day
    double border; // from what value measure will apply
    bool   reverse; // true - boolean values will invert, numeric values change direction to 0
    measure_id measure;
    estimation_id next{ 0 };
};

