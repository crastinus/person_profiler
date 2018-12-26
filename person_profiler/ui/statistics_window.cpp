#include "statistics_window.hpp"
#include <db/cache.hpp>
#include <db/measure.hpp>
#include <model/measure_graph.hpp>

statistics_window::statistics_window()
    :start_([this](time_t t) {this->on_update_time(true,t); }),
    end_([this](time_t t) {this->on_update_time(false,t); })
{
    start_.set_time(time(nullptr));
    end_.set_time(time(nullptr));
}

void statistics_window::render() {
    start_.render();
    ImGui::SameLine();
    end_.render();

    
}

void statistics_window::on_update_time(bool start_time, time_t time) {
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

    if (start == end) {
        return;
    }

    auto res = req_measure_graph_for(start, end);

}
