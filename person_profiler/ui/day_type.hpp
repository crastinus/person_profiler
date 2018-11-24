#pragma once

#include "ui.hpp"
#include <model/day.hpp>
#include <functional>

struct day_type_window : public window_inst {

    day_type_window(std::function<void(day_type)> callback);

    virtual char const* name() override {
        return "Day type";
    }

    virtual void render() override;

private:
    std::function<void(day_type)> callback_;
    std::vector<day_type> active_types_;

};