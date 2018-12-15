#include "estimation_window.hpp"
#include <imgui/imgui.h>
#include <db/measure.hpp>

estimation_window::estimation_window(day_type d, measure m)
    : day_type_button_(d, [this](day_type d) { on_day_type(d); }),
    measure_button_(m, [this](measure m) { on_measure(m); }) {
}

estimation_window::~estimation_window()
{
}

void estimation_window::render() {

    day_type_button_.render();
    ImGui::SameLine();
    measure_button_.render();

    if (day_type_button_.value().id == 0 ||
        measure_button_.value().id == 0) {
        return;
    }


    measure const& m = measure_button_.value();
    for (auto it = estimations_.begin(); it != estimations_.end(); ++it) {
      
        ImGui::PushID("border");
        ImGui::PushID("remove");

        switch (m.type) {
        case measure_type::boolean: {
            if (ImGui::Checkbox("border", &it->temp_bool)) {
                emit_changes();
            }
        } break;
        case measure_type::numeric: {
            if (ImGui::InputFloat("border", &it->temp_float)) {
                emit_changes();
            }
            ImGui::SameLine();
            if (ImGui::Button("remove") && estimations_.size() > 1) {
                it = estimations_.erase(it);
                emit_changes();
                return;
            }
        }
        break;
        }

        ImGui::PopID();
        ImGui::PopID();
    }

    // creating new node at end
    if (m.type != measure_type::boolean && ImGui::Button("Add")) {
        estimation val(estimations_.back());
        val.id = 0;
        val.border = val.border + (val.reverse ? -1 : 1);
        val.temp_float = val.border;
        estimations_.push_back(val);
        emit_new();
    }
}

void estimation_window::before_render() {
    if (estimations_.empty() && 
        day_type_button_.value().id != 0 &&
        measure_button_.value().id != 0) {
        init_estimations();
    }
}

void estimation_window::on_day_type(day_type d) {
    estimations_.clear();
}
 
void estimation_window::on_measure(measure m) {
    estimations_.clear();
}

void estimation_window::emit_changes() {
    have_changes_ = true;
}

void estimation_window::emit_new() {
    add_new_ = true;
}

void estimation_window::init_estimations() {

    estimations_.push_back(req_find_estimation(measure_button_.value().id, day_type_button_.value().id));

    while (estimations_.back().next != 0) {
        estimations_.push_back(estimations_.back().next);
    }

}
