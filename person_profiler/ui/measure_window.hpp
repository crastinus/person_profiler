#pragma once

#include "ui.hpp"
#include <functional>
#include <model/measure.hpp>

struct measure_button;
struct measure_window : public window_inst {

    measure_window(measure_button* callback = nullptr) ;

    void render() override;
    char const* name() override {
        return "Measure";
    }

private:
    measure_button * callback_;
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

    friend struct measure;     
};