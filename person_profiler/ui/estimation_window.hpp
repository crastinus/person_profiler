#pragma once

#include "ui.hpp"
#include <model/estimation.hpp>
#include <vector>
#include "measure_window.hpp"
#include "day_type_window.hpp"

struct estimation_window : public window_inst {

    estimation_window(day_type d = {}, measure m = {});

    // estimation but and measure
    virtual char const* name() override { return "estimation"; }
    virtual void render() override;
    virtual ImVec2 initial_size() override { return ImVec2(270, 270); }
    
private:

    void on_day_type(day_type d);
    void on_measure(measure m);

    day_type_button day_type_button_;
    measure_button measure_button_;
    std::vector<estimation> estimations_;
};