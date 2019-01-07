#pragma once

#include "ui.hpp"
#include <functional>
#include <model/measure.hpp>
#include <model/measure_graph.hpp>
#include "helper/input.hpp"

struct measure_button;
struct measure_window : public window_inst {

    measure_window(measure_button* callback = nullptr) ;

    void render() override;
    char const* name() override {
        return "Measure";
    }

    virtual ImVec2 initial_size() const override { return ImVec2(900, 400); }

private:

    void on_new_measure(measure m, measure_group_id mg_id);

    void on_new_measure_group(std::string const& measure_group_name);

    // show all measures or active. Default show active
    bool show_all_ = false;

    measure_button * callback_;

    measure_graph graph_;
};

struct measure_edit_window : public window_inst {
    measure_edit_window(measure m);
    measure_edit_window(int measure_group_id, std::vector<measure> measures_in_group, std::function<void(measure)> callback);
    void render() override;
    char const* name() override {
        return "Measure edit window";
    }

private:

    void save();

    measure measure_;
    input_text name_;
    input_text comment_;

    std::function<void(measure)> callback_;

    bool have_dependencies_ = false;

    std::vector<measure> measures_in_group_;
};

struct measure_button {
    measure_button(measure m = {}, std::function<void(measure)> callback = {});
    ~measure_button();

    void render();

    measure const& value() const;
private:

    void update_text(measure const& m);
    void update(measure m);

    measure value_;
    std::function<void(measure)> callback_;
    std::string text_;
    std::shared_ptr<measure_window> win_;

    friend struct measure_window;     
};