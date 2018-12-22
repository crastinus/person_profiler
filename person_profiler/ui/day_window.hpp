#include "ui.hpp"
#include <model/day.hpp>
#include "day_type_window.hpp"
#include "helper/input.hpp"
#include <model/measure_graph.hpp>
#include <unordered_set>

struct day_window : public window_inst {

    day_window(time_t timestamp = current_day_start(), bool planning = false);
    ~day_window();

    virtual char const* name() override;
    virtual void render() override;
    virtual void after_render() override;

private:

    void on_day_type(day_type new_day_type);

    std::string date_text_;

    day day_;
    day_type_button day_type_holder_;

    measure_values_graph graph_;

    bool blocked_;
    bool planning_;

    //input_text date_input_;
    input_text tip_input_;
    
    time_t next_ts_;
    time_t prev_ts_;

    std::unordered_set<int> disabled_measure_groups_;

    bool have_changes_;
};