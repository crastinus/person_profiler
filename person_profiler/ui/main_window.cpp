#include "main_window.hpp"
#include <imgui/imgui.h>

#include "day_window.hpp"
#include "day_type_window.hpp"
#include "measure_window.hpp"
#include "estimation_window.hpp"


void main_window::render() {

    if (ImGui::Button("day")) {
        window<day_window>();
    }

    if (ImGui::Button("measure")) {
        window<measure_window>();
    }

    if (ImGui::Button("estimation")) {
        window<estimation_window>();
    }

    // ethernal window
    show_ = true;
}
