#pragma once

#include <string>


struct measure_group {
    int id = 0;
    std::string name;
    bool        active;
};

struct measure_group_id {
    int id;

    operator measure_group const();

    measure_group_id& operator = (int id);
};