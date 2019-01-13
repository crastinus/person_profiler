#pragma once

#include "ui.hpp"
#include "helper/date_picker.hpp"
#include "helper/combo_box_content.hpp"

struct diary_window : public window_inst {

    diary_window();

    char const* name() override { return "Diary"; }
    void render() override;

private:

    void on_update_time(bool start_time, time_t time);

    date_picker_button start_;
    date_picker_button end_;
};