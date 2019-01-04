#include "day_window.hpp"
#include <db/queries.hpp>
#include <db/save.hpp>
#include <imgui/imgui.h>
#include "helper/global.hpp"
#include <common/common.hpp>
#include <ui/ui.hpp>
#include "day_type_window.hpp"
#include "helper/id_scope.hpp"

static char const* format = "%Y.%m.%d";
static float date_width = 80;

day_window::day_window(time_t timestamp, bool planning) 
    : //date_input_(date_text_, "###date", 80, 20), 
    tip_input_(day_.comment, "comment", 700, 100), planning_(planning),
    next_ts_(0), prev_ts_(0), have_changes_(false) {


    //if (planning_ && day_start(timestamp) < current_day_start()) {
    //    planning_ = false;
    //}

    blocked_ = false;// (day_start(timestamp) < current_day_start()) && !planning_;

    day_ = req_day_by_ts(day_start(timestamp));
    if (day_.id == 0) {
        day_.id = save(day_);
    }
    
    disabled_measure_groups_ = disabled_measure_groups(day_.id);

    date_text_ = strtime(format, day_.day_timestamp);

    auto dt = req_day_type_by_day_id(day_.id);
    day_type_holder_ = day_type_button(dt, [this](day_type d) { on_day_type(d); });

    if (day_.id != 0) {
        graph_ = req_current_measure_graph_for_day(day_.id);
    }
    
    if (graph_.empty() && dt.id != 0) {
        graph_ = req_current_measure_graph(dt.id);
    }

    tip_input_.update();

    std::tie(prev_ts_, next_ts_) = req_prev_next_day(day_.day_timestamp);
}

day_window::~day_window() {
    if (have_changes_) {
        tip_input_.shrink();
        save(day_);
    }
}

char const * day_window::name() {
    return "day";
}

void day_window::render() {
    
    // date rendering
    if (prev_ts_ != 0 && ImGui::Button("Prev", button_size()) ) {
        window<day_window>(prev_ts_, planning_);
    }
    
    ImGui::SameLine();

    if (ImGui::Button(date_text_.c_str(), big_button_size())) {
        // TODO: Open date picker
    }

    if (next_ts_ != 0) {
        ImGui::SameLine();
    }

    if (next_ts_ != 0 && ImGui::Button("Next", button_size())) {
        window<day_window>(next_ts_, planning_);
    }

    ImGui::SameLine();
    day_type_holder_.render(blocked_);

    
    if (tip_input_.render_multiline()) {
        have_changes_ = true;
    }

    for (auto&[mg_id, vec] : graph_) {
        measure_group mg = mg_id;

        id_scope push_id(mg.id);

        bool disabled = (disabled_measure_groups_.count(mg_id.id) != 0);
        if (disabled && blocked_) {
            continue;
        }

        if (ImGui::CollapsingHeader(mg.name.c_str())) {

            // disabling logic
            if (ImGui::Checkbox("disabled", &disabled)) {
                if (disabled) {
                    disable_measure_group_for_day(day_.id, mg_id.id);
                    disabled_measure_groups_.insert(mg_id.id);
                }
                else {
                    enable_measure_group_for_day(day_.id, mg_id.id);
                    disabled_measure_groups_.erase(mg_id.id);
                }
            }

            if (disabled) {
                continue;
            }

            for (value_info& vi : vec) {
                //ImGui::Text(vi.value_name_.c_str());
                //ImGui::SameLine();
                ImGui::PushItemWidth(200);
                ImGui::SetCursorPos({ 20, ImGui::GetCursorPos().y });

                switch (vi.type_) {
                case measure_type::boolean: {
                    if (ImGui::Checkbox(vi.value_name_.c_str(), &vi.bool_value_)) {
                        vi.value_.val = static_cast<double>(vi.bool_value_);
                    }
                } break;
                case measure_type::numeric: {
                    if (ImGui::InputFloat(vi.value_name_.c_str(), &vi.float_value_, 0.0f, 0.0f, 2)) {
                        vi.value_.val = static_cast<double>(vi.float_value_);
                    }
                } break;
                }
            }
        }
    }

    ImGui::Separator();
    if (ImGui::Button("Save")) {

        // day must be save on choosing day type
        if (day_.id == 0) {
            throw std::runtime_error("day.id == 0");
        }

        for (auto& [mg_id, vec] : graph_) {
            for (auto& vi : vec) {
                vi.value_.day.id = day_.id;
                vi.value_.id = save(vi.value_);
            }
        }

        errors_.clear();
    }
}

void day_window::after_render() {
}

void day_window::on_day_type(day_type new_day_type) {

    day_type_day dtd;
    dtd.day_id = day_.id;
    dtd.day_type_id = day_type_holder_.value().id;
    save(dtd);

    graph_ = req_current_measure_graph(new_day_type.id);
}

