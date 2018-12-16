#include "day_type_window.hpp"
#include <db/day.hpp>
#include <imgui/imgui.h>
#include "helper/global.hpp"
#include "helper/input_window.hpp"
#include <db/save.hpp>

day_type_window::day_type_window(day_type_button* callback) 
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
            callback_->update(t);
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
            callback_->update(t);
            close();
        });

    }
}

day_type_button::day_type_button(day_type d, std::function<void(day_type)> callback) : day_type_(d), callback_(callback) {
    update_text(d);
}

day_type_button::~day_type_button() {
    if (dt_win_) {
        dt_win_->close();
    }
}

void day_type_button::render(bool blocked) {

    if (blocked) {
        ImGui::Text(text_.c_str());
        return;
    }

    if (ImGui::Button(text_.c_str(), big_button_size())) {
        dt_win_ = window<day_type_window>(this);
    }
}

day_type const & day_type_button::value() const {
    return day_type_;
}

void day_type_button::update_text(day_type const & d) {
    if (d.id == 0) {
        text_ = "type:None";
    }
    else {
        text_ = "type:" + d.name;
    }
}

void day_type_button::update(day_type d) {
    day_type_ = d;
    update_text(d);
    if (callback_) {
        callback_(d);
    }

    dt_win_->close();
    dt_win_.reset();
}
