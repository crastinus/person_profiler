#include "diary_window.hpp"

diary_window::diary_window()
    :start_(0, [this](time_t t) {this->on_update_time(true, t); }),
    end_(1, [this](time_t t) {this->on_update_time(false, t); }) {
    start_.set_time(time(nullptr));
    end_.set_time(time(nullptr));
}

void diary_window::render() {
    start_.render();
    ImGui::SameLine();
    end_.render();
}

void diary_window::on_update_time(bool start_time, time_t time) {
}
