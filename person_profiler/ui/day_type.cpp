#include "day_type.hpp"
#include <db/day.hpp>
#include <imgui/imgui.h>
#include "helper/global.hpp"
#include "helper/input_window.hpp"
#include <db/save.hpp>

day_type_window::day_type_window(std::function<void(day_type)> callback) 
    : callback_(callback) {
    active_types_ = active_day_types();
}

void day_type_window::render()  {
    // TODO: Make multiples modes
    size_t idx = 0;
    for (auto const& t : active_types_) {
        if (idx % 2 == 1) {
            ImGui::SameLine();
        }
        if (ImGui::Button(t.name.c_str(), button_size())) {
            callback_(t);
            close();
        }
        ++idx;
    }

    if (ImGui::Button("Add", button_size())) {
        window<input_window>("Day Type Name", [this](std::string name) {
            day_type t{};
            t.name = name;
            t.active = true;
            t.id = save(t);
            callback_(t);
            close();
        });

    }
}
