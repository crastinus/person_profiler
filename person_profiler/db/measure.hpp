#pragma once 

#include <unordered_set>
#include <map>

struct estimation;
struct measure;
struct value;
struct measure_group;
struct measure_values_graph;
struct measure_graph;
struct measure_comment;

estimation req_estimation(int id);
estimation req_find_estimation(int measure_id, int day_type_id);
measure req_measure(int id);
value req_value(int id);
measure_group req_measure_group(int id);
measure_comment req_measure_comment(int measure_id, int day_id);

// mapping day to map of (measure_id, measure_comment)
using measure_comment_graph = std::map<time_t, std::map<int, measure_comment>>;

// \brief request to measure comment
// \param first_day, last_day - days range
// \return measure graph
measure_comment_graph req_measure_comment_graph(time_t first_day, time_t last_day);

bool have_dependencies(estimation const& e);
bool have_dependencies(measure const& m);

std::unordered_set<int> disabled_measure_groups(int day_id);
void disable_measure_group_for_day(int day_id, int measure_group_id);
void enable_measure_group_for_day(int day_id, int measure_group_id);

measure_values_graph req_current_measure_graph(int day_type_id);
measure_values_graph req_current_measure_graph_for_day(int day_id);
measure_values_graph req_measure_graph_for(time_t first_day, time_t last_day);

measure_graph req_measure_graph();