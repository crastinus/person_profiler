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

            if (ImGui::Button(m.name.c_str()) && callback_ != nullptr) {
                callback_->update(m);
            }

            bool is_bool = (m.type == measure_type::boolean);
            bool is_numeric = (m.type == measure_type::numeric);

            ImGui::SameLine();

            if (ImGui::Checkbox("IsBool", &is_bool)) {
                m.type = (is_bool ? measure_type::boolean : measure_type::numeric);
                save(m);
            }

            ImGui::SameLine();

            if (ImGui::Checkbox("IsNumeric", &is_numeric)) {
                m.type = (is_bool ? measure_type::numeric : measure_type::boolean);
                save(m);
            }

            ImGui::SameLine();


            if (ImGui::Checkbox("Active", &m.active)) {
                save(m);
            }


            ImGui::PopID();
        }


        if (ImGui::Button("Add")) {
            window<input_window>("measure", [this, mg_id](std::string const& str) {
                on_new_measure(str, mg_id);
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

void measure_window::on_new_measure(std::string const& measure_name, measure_group_id mg_id) {
    if (graph_.find( mg_id) == graph_.end()) {
        return;
    }

    for (auto& m : graph_.at(mg_id)) {
        if (m.name == measure_name) {
            return;
        }
    }

    measure new_m;
    new_m.active = true;
    new_m.measure_group = mg_id;
    new_m.name = measure_name;
    new_m.type = measure_type::boolean;
    new_m.id =  save(new_m);

    graph_.at(mg_id).push_back(new_m);
}

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

