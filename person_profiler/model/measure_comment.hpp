#pragma once

#include "measure.hpp"
#include "day.hpp"

struct measure_comment {
    std::string comment_text;
    measure_id measure;
    day_id     day;
};
