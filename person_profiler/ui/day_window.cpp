#include "day_window.hpp"
#include <db/queries.hpp>
#include <db/save.hpp>
#include <imgui/imgui.h>
#include "helper/global.hpp"
#include <common/common.hpp>
#include <ui/ui.hpp>
#include "day_type_window.hpp"

static char const* format = "%Y.%m.%d";
static float date_width = 80;

day_window::day_window(time_t timestamp) 
    : //date_input_(date_text_, "###date", 80, 20), 
    tip_input_(day_.comment, "comment", 700, 100),
    next_ts_(0), prev_ts_(0), have_changes_(false) {

    blocked_ = (day_start(timestamp) != current_day_start());

    day_ = req_day_by_ts(day_start(timestamp));
    if (day_.id == 0) {
        save(day_);
    }

    date_text_ = strtime(format, day_.day_timestamp);

    auto dt = req_day_type_by_day_id(day_.id);
    day_type_holder_ = day_type_button(dt, [this](day_type d) { on_day_type(d); });

    if (day_.id != 0) {
        graph_ = req_current_measure_graph_for_day(day_.id);
    }

    //date_input_.update();
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
        window<day_window>(prev_ts_);
    }
    
    ImGui::SameLine();

    if (ImGui::Button(date_text_.c_str(), big_button_size())) {
        // TODO: Open date picker
    }

    if (next_ts_ != 0) {
        ImGui::SameLine();
    }

    if (next_ts_ != 0 && ImGui::Button("Next", button_size())) {
        window<day_window>(next_ts_);
    }

    ImGui::SameLine();
    day_type_holder_.render(blocked_);

    
    if (tip_input_.render_multiline()) {
        have_changes_ = true;
    }

    for (auto&[mg_id, vec] : graph_) {
        measure_group mg = mg_id;

        ImGui::PushID(mg.id);

        if (ImGui::CollapsingHeader(mg.name.c_str())) {
            for (value_info& vi : vec) {
                ImGui::Text(vi.value_name_.c_str());
                ImGui::SameLine();
                ImGui::PushItemWidth(200);

                switch (vi.type_) {
                case measure_type::boolean: {
                    if (ImGui::Checkbox("value", &vi.bool_value_)) {
                    }
                } break;
                case measure_type::numeric: {
                    if (ImGui::InputFloat("value", &vi.float_value_, 0.0f, 0.0f, 2)) {
                    }
                } break;
                }
            }
        }

        ImGui::PopID();
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

