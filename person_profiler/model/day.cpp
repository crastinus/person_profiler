
#include <time.h>
#include "day.hpp"
#include <db/model.hpp>
#include "..\ui\day.hpp"

// never change this code
time_t day_start(time_t ts) {
    constexpr time_t seconds = 24 * 60 * 60;
    return (ts / seconds) * seconds; 
}

time_t current_day_start() {
    return day_start(time(nullptr));
}

day_id::operator day const() {
    return get_model<day>(id);
}

day_id & day_id::operator=(int id) {
    this->id = id;
    return *this;
}


day_type_id::operator day_type const() {
    return get_model<day_type>(id);
}

day_type_id & day_type_id::operator=(int id) {
    this->id = id;
    return *this;
}
