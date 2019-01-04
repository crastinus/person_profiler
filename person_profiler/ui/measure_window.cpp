#include "measure_window.hpp"
#include <imgui/imgui.h>
#include <db/measure.hpp>
#include <db/model.hpp>
#include <db/save.hpp>
#include "helper/input_window.hpp"

measure_window::measure_window(measure_button * callback)
    : callback_(callback) {
    graph_ = req_measure_graph();
}

void measure_window::render() {

    ImGui::Checkbox("Show all", &show_all_);
    for (auto& [mg_id, vec] : graph_) {
        measure_group mg = mg_id;

        bool show = (mg.active || show_all_);
        if (!show || !ImGui::CollapsingHeader(mg.name.c_str())) {
            continue;
        }

        ImGui::PushID(mg.name.c_str());

        for (auto& m : vec) {
            if (!(m.active || show_all_)) {
                continue;
            }

            ImGui::PushID(m.id);

            if (ImGui::Button(m.name.c_str())) {
                if (callback_ != nullptr) {
                    callback_->update(m);
                }
                else {
                    window<measure_edit_window>(m);
                }
            }

            ImGui::PopID();
        }


        if (ImGui::Button("Add")) {
            window<measure_edit_window>(mg_id.id, vec, [this, mg_id](measure m) {
                graph_.at(mg_id).push_back(m);
            });
        }
        
        ImGui::Separator();

        ImGui::PopID();

    }

    if (ImGui::Button("Add")) {
        window<input_window>("measure_group", [this](std::string const& str) {
            on_new_measure_group(str);
        });
    }
}

//void measure_window::on_new_measure(std::string const& measure_name, measure_group_id mg_id) {
//    if (graph_.find( mg_id) == graph_.end()) {
//        return;
//    }
//
//    for (auto& m : graph_.at(mg_id)) {
//        if (m.name == measure_name) {
//            return;
//        }
//    }
//
//    measure new_m;
//    new_m.active = true;
//    new_m.measure_group = mg_id;
//    new_m.name = measure_name;
//    new_m.type = measure_type::boolean;
//    new_m.id =  save(new_m);
//
//    graph_.at(mg_id).push_back(new_m);
//}

void measure_window::on_new_measure_group(std::string const& measure_group_name) {

    for (auto&[mg_id, _] : graph_) {
        measure_group m = mg_id;
        if (m.name == measure_group_name) {
            return;
        }
    }

    measure_group mg;
    mg.name = measure_group_name;
    mg.active = true;
    mg.id = save(mg);

    measure_group_id id;
    id.id = mg.id;
    graph_[id].clear();

}



/*
BUTTON CALLBACK
*/

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
    value_ = m;
    update_text(m);
    if (callback_) {
        callback_(m);
    }

    win_->close();
    win_.reset();

}

measure_edit_window::measure_edit_window(measure m) 
    : measure_(m), name_(measure_.name, "Name"), 
    comment_(measure_.comment, "Comment")  {
    have_dependencies_ = false; // (measure_.id != 0 && have_dependencies(measure_));
}

measure_edit_window::measure_edit_window(int measure_group_id, std::vector<measure> measures_in_group, std::function<void(measure)> callback)
    : measure_edit_window(measure{})
{
    measure_.type = measure_type::boolean;
    measure_.measure_group = measure_group_id;
    measure_.active = true;
    measures_in_group_ = measures_in_group;
    callback_ = callback;
}

void measure_edit_window::render() {
    if (!have_dependencies_) {
        bool is_bool = (measure_.type == measure_type::boolean);
        bool is_numeric = (measure_.type == measure_type::numeric);

        if (ImGui::Checkbox("IsBool", &is_bool)) {
            measure_.type = (is_bool ? measure_type::boolean : measure_type::numeric);
        }

        ImGui::SameLine();

        if (ImGui::Checkbox("IsNumeric", &is_numeric)) {
            measure_.type = (is_bool ? measure_type::numeric : measure_type::boolean);
        }

        name_.render_input();
        
    }
    else {
        ImGui::Text(measure_.type == measure_type::boolean ? "boolean" : "numeric"); ImGui::SameLine();
        ImGui::Text(measure_.name.c_str());
    }

    comment_.render_multiline();

    if (ImGui::Checkbox("Active", &measure_.active)) {
    }

    if (ImGui::Button("Save")) {
        save();
    }

}

void measure_edit_window::save() {

    if (measure_.name.empty()) {
        throw std::runtime_error("Name is empty");
    }

    for (auto& m : measures_in_group_) {
        if (m.name == measure_.name) {
            throw std::runtime_error("Measure with this name allready exits");
        }
    }

    measure_.id = ::save(measure_);
    if (callback_) {
        callback_(measure_);
    }

    errors_.clear();
}
