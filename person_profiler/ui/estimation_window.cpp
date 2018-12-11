#include "estimation_window.hpp"
#include <imgui/imgui.h>

estimation_window::estimation_window(day_type d, measure m)
    : day_type_button_(d, [this](day_type d) { on_day_type(d); }),
    measure_button_(m, [this](measure m) { on_measure(m); }) {
}

void estimation_window::render() {

    day_type_button_.render();
    ImGui::SameLine();
    measure_button_.render();


}

void estimation_window::on_day_type(day_type d) {

    if (estimations_.empty()) {
        return;
    }

}

void estimation_window::on_measure(measure m) {

    if (estimations_.empty()) {
        return;
    }

}
