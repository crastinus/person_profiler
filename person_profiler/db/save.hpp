#pragma once

#include <model/value.hpp>
#include <model/measure_comment.hpp>

int save(value const& v);
int save(day const& d);
int save(day_type const& dt);
void save(day_type_day const& dtd);
int save(estimation const& e);
int save(measure const& m);
int save(measure_group const& mg);
void save(measure_comment const& comment);