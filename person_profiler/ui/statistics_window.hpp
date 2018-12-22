#pragma once

#include "ui.hpp"


struct statistics_window : public window_inst {
    char const* name () override {
        return "statistics";
    }

    void render() override;
};