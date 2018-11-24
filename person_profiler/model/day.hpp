#pragma once 

#include <time.h>
#include <string>

struct day_type {
    int id = 0;
    std::string name;
    bool active;
};

struct day_type_id {
    int id;

    operator day_type const();

    day_type_id& operator = (int id);
};

struct day {
    int id = 0;
    time_t day_timestamp; // day start acctually
    std::string comment;
};


struct day_id {

    int id;

    operator day const();

    day_id& operator = (int id);
};

struct day_type_day {
    day_id day_id;
    day_type_id day_type_id;
};

time_t day_start(time_t ts);
time_t current_day_start();