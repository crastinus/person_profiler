#include "estimation_window.hpp"
#include <imgui/imgui.h>
#include <db/measure.hpp>
#include <db/save.hpp>
#include <common/concat.hpp>

estimation_window::estimation_window(day_type d, measure m)
    : day_type_button_(d, [this](day_type d) { on_day_type(d); }),
    measure_button_(m, [this](measure m) { on_measure(m); }) {
}

estimation_window::~estimation_window() {

}
void estimation_window::save_form() {
    if (!(have_changes_ || add_new_)) {
        return;
    }

    if (have_dependencies_) {
        // front estimation might be deleted
        auto estim = req_find_estimation(measure_button_.value().id, day_type_button_.value().id);
        estim.active = false;
        save(estim);

        for (auto& e : estimations_) {
            e.id = 0;
        }
    }

    int prev_id = 0;
    bool reverse = estimations_.front().reverse;
    float prev_weight = (estimations_.front().reverse ? std::numeric_limits<int>::max() : 0);
    float prev_border = 0;

    // checking
    for (estimation const& e : estimations_) {
        measure m = e.measure;
        if (m.type == measure_type::numeric && prev_border >= e.temp_float) {
            throw std::logic_error(concat("Border value ", prev_border, " greater then ", e.temp_float));
        }

        if (reverse && prev_weight <= e.temp_weight) {
            throw std::logic_error(concat("Prev weight (reversed) ", prev_border, " less then ", e.temp_weight));
        } 
        
        if (!reverse && prev_weight >= e.temp_weight) {
            throw std::logic_error(concat("Prev weight ", prev_border, " greater then ", e.temp_weight));
        }

        prev_border = e.temp_float;
        prev_weight = e.temp_weight;
    }

    auto mtype = measure_button_.value().type;
    for (estimation& e : estimations_) {
        e.active = false;
        e.border = (mtype == measure_type::boolean ? 0 : e.temp_float);
        e.weight = e.temp_weight;
    }

    estimations_.front().active = true;

    // saving
    for (auto it = estimations_.rbegin(); it != estimations_.rend(); ++it) {
        it->next = prev_id;
        prev_id = save(*it);
    }

    errors_.clear();
}

void estimation_window::render() {

    day_type_button_.render();
    ImGui::SameLine();
    measure_button_.render();

    if (day_type_button_.value().id == 0 ||
        measure_button_.value().id == 0) {
        return;
    }

    if (ImGui::Checkbox("reverse", &reverse_)) {
        for (auto& e : estimations_) {
            e.reverse = reverse_;
        }
        emit_changes();
    }

    int counter = 0;

    measure const& m = measure_button_.value();
    for (auto it = estimations_.begin(); it != estimations_.end(); ++it) {
      
        ImGui::PushID(++counter);

        ImGui::Separator();

        ImGui::PushItemWidth(200);
        switch (m.type) {
        case measure_type::boolean: {
            //if (ImGui::Checkbox("border", &it->temp_bool)) {
            //    emit_changes();
            //}
        } break;
        case measure_type::numeric: {
            if (ImGui::InputFloat("border", &it->temp_float, 0.0f, 0.0f, 2)) {
                if (it->temp_float < 0) {
                    it->temp_float = static_cast<float>(it->border);
                }
                else {
                    emit_changes();
                }
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

        ImGui::SameLine();
        if (ImGui::InputFloat("weight", &it->temp_weight, 0.0f, 0.0f, 2)) {
            if (it->temp_weight < 0) {
                it->temp_weight = static_cast<float>(it->weight);
            }
            else {
                emit_changes();
            }
        }
        
        ImGui::PopID();
    }

    ImGui::Separator();

    // creating new node at end
    if (m.type != measure_type::boolean && ImGui::Button("Add")) {
        estimation val(estimations_.back());
        val.id = 0;
        val.border = val.border + (val.reverse ? -1 : 1);
        val.temp_float = static_cast<float>(val.border);
        val.measure = measure_button_.value().id;
        val.day_type = day_type_button_.value().id;
        estimations_.push_back(val);
        emit_new();
        
        ImGui::SameLine();
    }

    if (ImGui::Button("Save")) {
        save_form();
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

    have_dependencies_ = have_dependencies(estimations_.back());
    reverse_ = estimations_.back().reverse;

    while (estimations_.back().next != 0) {
        estimations_.push_back(estimations_.back().next);
    }

}
