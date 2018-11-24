#include "ui.hpp"
#include <model/day.hpp>
#include "helper/input.hpp"

struct day_window : public window_inst {

    day_window(time_t timestamp = current_day_start());
    ~day_window();

    virtual char const* name() override;
    virtual void render() override;
    virtual void after_render() override;

private:

    void on_day_type(day_type new_day_type);

    std::string date_text_;
    std::string day_type_text_;

    day day_;
    day_type day_type_;

    //input_text date_input_;
    input_text tip_input_;
    
    time_t next_ts_;
    time_t prev_ts_;

    bool have_changes_;
};