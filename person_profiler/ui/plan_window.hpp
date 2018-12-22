#pragma once

#include "ui.hpp"
#include <model/day.hpp>
#include "helper/date_widget.hpp"

struct plan_window : public window_inst {

    plan_window();

    char const* name() override {
        return "plan";
    }

    void render() override;

private:

    void on_update_timestamp();

    time_t                   month_timestamp_;
    std::string              timestamp_text_;
    std::vector<date_widget> widgets_;

};