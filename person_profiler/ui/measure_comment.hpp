#pragma once

#include "ui.hpp"
#include <model/measure_comment.hpp>
#include "helper/input.hpp"

struct measure_comment_window : public window_inst {

    measure_comment_window( measure_id measure, day_id day);

    virtual char const* name() override { return "Measure comment"; }
    virtual void render() override;
    virtual ImVec2 initial_size() const override { return ImVec2(1024, 640); }
    
private:

    measure_comment comment;
    input_text comment_text;
};