#pragma once

#include "ui.hpp"
#include "helper/date_picker.hpp"
#include "helper/combo_box_content.hpp"
#include "model/measure_graph.hpp"
#include <map>
#include <set>

struct day_group_stat {
    std::vector<value> values_;
    std::string legend_;
    double result_estimation_; // result estimation of current day
};

struct statistics_window : public window_inst {

    statistics_window();

    char const* name () override {
        return "statistics";
    }

    virtual ImVec2 initial_size() const override { return ImVec2(270, 500); }

    void render() override;

private:

    void on_update_time(bool start_time, time_t time);

    void save_showed_measures();
    void restore_showed_measures();

    date_picker_button start_;
    date_picker_button end_;
    
    measure_values_graph graph_;

    using day_group_dict = std::unordered_map<int ,day_group_stat>;
    mg_graph<day_group_dict> result_info_;

    std::vector<day> days_;
    std::map<int, std::string> measure_group_names_;
    std::set<int> allowed_measure_groups_;

    int current_mg_id_ = 0;
};