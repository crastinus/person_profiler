#include "measure_window.hpp"
#include <imgui/imgui.h>


measure_window::measure_window(measure_button * callback)
    : callback_(callback) {}

void measure_window::render() {
}

measure_button::measure_button(measure m, std::function<void(measure)> callback) 
  : value_(m), callback_(callback) {
    update_text(value_);
}

measure_button::~measure_button() {
    if (win_) {
        win_->close();
    }
}

void measure_button::render() {
    if(ImGui::Button(text_.c_str())) {
        win_ = window<measure_window>(this);
    }
}

measure const & measure_button::value() const {
    return value_;
}

void measure_button::update_text(measure const & m) {
    if (m.id == 0) {
        text_ = "measure:None";
    }
    else {
        text_ = "measure:" + m.name;
    }
}

void measure_button::update(measure m) {
    update_text(m);
    if (callback_) {
        callback_(m);
    }

    win_->close();
    win_.reset();

}

