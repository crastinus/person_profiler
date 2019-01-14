#pragma once

#include "ui.hpp"
#include "helper/date_picker.hpp"
#include "helper/combo_box_content.hpp"
#include <db/measure.hpp>
#include <model/measure_comment.hpp>

struct diary_window : public window_inst {

    diary_window();

    char const* name() override { return "Diary"; }
    void render() override;
    virtual ImVec2 initial_size() const override { return ImVec2(1024, 400); }

private:

    void on_update_time(bool start_time, time_t time);

    measure_comment_graph mc_graph_;
    std::map<std::string, measure>  measure_by_name_;

    combo_box_content measure_combo_content_;

    date_picker_button start_;
    date_picker_button end_;
};