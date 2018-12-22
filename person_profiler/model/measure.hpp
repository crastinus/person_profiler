#pragma once

#include <string>
#include "measure_group.hpp"

enum class measure_type {
    boolean = 1,
    numeric = 2
};

struct measure {
    int id = 0;
    std::string name;
    bool active;
    measure_type type;
    measure_group_id measure_group;
};

struct measure_id {
    int id;
     
    operator measure const() const;

    measure_id& operator = (int id);
};