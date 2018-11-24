#pragma once

#include <time.h>
#include <utility>
#include <vector>

struct day;
struct day_type;

day req_day(int id);

// return's day or day with id=0 if not exists
day req_day_by_ts(time_t ts);

day_type req_day_type_by_day_id(int id);

// next and previous days
std::pair<time_t, time_t> req_prev_next_day(time_t ts);

day_type req_day_type(int id);

std::vector<day_type> active_day_types();
