#include "plan_window.hpp"
#include <common/time.hpp>
#include "day_window.hpp"

plan_window::plan_window() {
    month_timestamp_ = month();
    on_update_timestamp();
}

void plan_window::render() {
    
    if (ImGui::Button("prev")) {
        month_timestamp_ = month(month_timestamp_, -1);
        on_update_timestamp();
    }

    ImGui::SameLine();

    ImGui::Text(timestamp_text_.c_str());

    ImGui::SameLine();

    if (ImGui::Button("next")) {
        month_timestamp_ = month(month_timestamp_, 1);
        on_update_timestamp();
    }

    size_t counter = 0;
    for (auto& d : widgets_) {
        d.render();
        if (++counter%4 != 0) {
            ImGui::SameLine();
        }
    }
    

}

void plan_window::on_update_timestamp() {
    timestamp_text_ = format("%Y.%m", month_timestamp_);
    auto next_mon = month(month_timestamp_, 1);

    widgets_.clear();
    for (time_t ts = month_timestamp_; ts < next_mon; ts += 86400) {
        widgets_.push_back(date_widget("%a %d", ts, [this, ts]() {
            window <day_window>(ts, true);
        }));
    }

}
