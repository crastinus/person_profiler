#pragma once

#include "ui.hpp"
#include <model/estimation.hpp>
#include <vector>
#include "measure_window.hpp"
#include "day_type_window.hpp"

struct estimation_window : public window_inst {

    estimation_window(day_type d = {}, measure m = {});
    ~estimation_window();

    // estimation but and measure
    virtual char const* name() override { return "estimation"; }
    virtual void render() override;
    virtual ImVec2 initial_size() const override { return ImVec2(270, 270); }
    virtual void before_render() override;
    
private:

    void on_day_type(day_type d);
    void on_measure(measure m);

    void emit_changes();
    void emit_new();

    void save_form();

    void init_estimations();

    bool have_changes_ = false;
    bool add_new_ = false;
    bool have_dependencies_ = false;
    bool reverse_ = false;

    day_type_button day_type_button_;
    measure_button measure_button_;
    std::vector<estimation> estimations_;
};