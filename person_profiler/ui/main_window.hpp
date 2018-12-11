#pragma once

#include "ui.hpp"

struct main_window : public window_inst {
    char const * name() override {
        return "Main";
    }

    void render() override;
};