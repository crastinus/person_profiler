#pragma once

#include "ui.hpp"
#include "helper/date_picker.hpp"

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
};