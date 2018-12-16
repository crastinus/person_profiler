#pragma once 


struct estimation;
struct measure;
struct value;
struct measure_group;
struct measure_values_graph;
struct measure_graph;

estimation req_estimation(int id);
estimation req_find_estimation(int measure_id, int day_type_id);
measure req_measure(int id);
value req_value(int id);
measure_group req_measure_group(int id);

bool have_dependencies(estimation const& e);
bool have_dependencies(measure const& m);

measure_values_graph req_current_measure_graph(int day_type_id);
measure_values_graph req_current_measure_graph_for_day(int day_id);
measure_graph req_measure_graph();