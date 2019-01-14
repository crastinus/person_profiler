#include "diary_window.hpp"
#include "helper/id_scope.hpp"
#include <db/cache.hpp>
#include <common/time.hpp>
#include <common/concat.hpp>
#include <common/to.hpp>
#include <set>
#include <db/options.hpp>
#include <sstream>

static char const* diary_range = "diary_range_option";
static char const* last_choosen_measure = "diary_last_choosen_measure_option";

diary_window::diary_window()
    : measure_combo_content_({}),
    start_(0, [this](time_t t) {this->on_update_time(true, t); }),
    end_(1, [this](time_t t) {this->on_update_time(false, t); }) {

    time_t start_ts = time(nullptr), end_ts = time(nullptr);

    auto range = get_option(diary_range);
    if (!range.empty()) {
        std::istringstream is(range);
        is >> start_ts;
        is >> end_ts;
    }

    start_.set_time(start_ts);
    end_.set_time(end_ts);
}

void diary_window::render() {
    start_.render();
    ImGui::SameLine();
    end_.render();
    
    if (!measure_combo_content_.empty() && ImGui::Combo("measure", &measure_combo_content_.idx(), measure_combo_content_.content())) {
       auto& m = measure_by_name_.at(measure_combo_content_.current_element());
       save_option(last_choosen_measure, concat(m.id));
    }

    if (measure_combo_content_.idx() == -1) {
        return;
    }

    auto& measure = measure_by_name_.at(measure_combo_content_.current_element());
    for (auto&[ts, m2comment] : mc_graph_) {
        auto comment_it = m2comment.find(measure.id);
        if (comment_it == m2comment.end()) {
            continue;
        }

        auto& comment = comment_it->second;

        id_scope day_id_scope(comment.day.id);

        ImGui::Text(format("%m.%d", ts).c_str());
        ImGui::SameLine();
        ImGui::Text(comment.comment_text.c_str());

    }
}

void diary_window::on_update_time(bool start_time, time_t time) {

    auto start = start_.time();
    auto end = end_.time();
    if (start > end) {
        if (start_time) {
            end_.set_time(time);
        }
        else {
            start_.set_time(time);
        }
    }

    save_option(diary_range, concat(start, " ", end));

    if (start == end || start < 0 || end < 0) {
        return;
    }

    mc_graph_ = req_measure_comment_graph(start, end);
    std::set<int> measure_ids;

    for (auto&[ts, m2comment] : mc_graph_) {
        for (auto&[mid, comment] : m2comment) {
            measure_ids.insert(mid);
        }
    }

    for (int mid : measure_ids) {
        auto m = cache().get<measure>(mid);
        measure_by_name_[m->name] = *m;
    }

    measure_combo_content_.assign(measure_by_name_.begin(), measure_by_name_.end(), [](auto const& p) {return p.first; });

    auto last_measure_id_str = get_option(last_choosen_measure);
    if (!last_measure_id_str.empty()) {
        int measure_id = to<int>(last_measure_id_str);
        auto m = cache().get<measure>(measure_id);
        measure_combo_content_.setup_element(m->name);
    }


}
