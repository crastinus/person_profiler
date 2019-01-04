#pragma once

#include "ui.hpp"
#include "helper/date_picker.hpp"
#include "helper/combo_box_content.hpp"
#include "model/measure_graph.hpp"

struct day_group_stat {
    day         day_;
    std::string day_text_;
    std::vector<value> values_;
    std::string legend_;
    double result_estimation_; // result estimation of current day
};

struct statistics_window : public window_inst {

    statistics_window();

    char const* name () override {
        return "statistics";
    }

    void render() override;

private:

    void on_update_time(bool start_time, time_t time);

    date_picker_button start_;
    date_picker_button end_;

    measure_values_graph graph_;
    mg_graph<std::vector<day_group_stat>> result_info_;

    int current_mg_id_ = 0;
    combo_box_content measure_groups_;
};