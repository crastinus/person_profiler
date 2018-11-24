#pragma once

#include "ui.hpp"

struct estimation_window : public window_inst {

    // estimation but and measure
    virtual char const* name() override { return "estimation"; }
    virtual void render() override;

};