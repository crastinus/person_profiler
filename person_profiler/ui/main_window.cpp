#include "main_window.hpp"
#include <imgui/imgui.h>

#include "day_window.hpp"
#include "day_type_window.hpp"
#include "measure_window.hpp"
#include "estimation_window.hpp"
#include "diary_window.hpp"
#include "plan_window.hpp"
#include "statistics_window.hpp"
#include "helper/date_picker.hpp"
#include <common/concat.hpp>

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

    if (ImGui::Button("plan")) {
        window<plan_window>();
    }

    if (ImGui::Button("statistics")) {
        window<statistics_window>();
    }

    if (ImGui::Button("diary")) {
        window<diary_window>();
    }

    //if (ImGui::Button("day_picker")) {
    //    window<date_picker>(0);
    //}

    // ethernal window
    show_ = true;
}
