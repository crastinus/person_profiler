#include "statistics_window.hpp"
#include <db/cache.hpp>
#include <db/measure.hpp>
#include <db/model.hpp>
#include <model/measure_graph.hpp>
#include <common/osstream.hpp>
#include <common/time.hpp>


// proper accumulate
// TODO: move somewhere
template<typename ContainerType, typename FunctorType>
inline auto accumulate(ContainerType const& cont, FunctorType&& f) {
    using result_t = decltype(f(*cont.begin()));
    result_t result{};

    for (auto& v : cont) {
        result += f(v);
    }

    return result;
}

statistics_window::statistics_window()
    :start_(0,[this](time_t t) {this->on_update_time(true,t); }),
    end_(1, [this](time_t t) {this->on_update_time(false,t); }),
    measure_groups_({})
{
    start_.set_time(time(nullptr));
    end_.set_time(time(nullptr));
}

void statistics_window::render() {
    start_.render();
    ImGui::SameLine();
    end_.render();

    if (!measure_groups_.empty() && ImGui::Combo("measure group", &measure_groups_.idx(), measure_groups_.content())) {
        for (auto&[mg_id, _] : result_info_) {
            (void)_;
            measure_group mg = mg_id;
            if (mg.name == measure_groups_.current_element()) {
                current_mg_id_ = mg_id.id;
                break;
            }
        }
    }

    if (current_mg_id_ == 0) {
        return;
    }

    auto& vals = result_info_.at({ current_mg_id_ });
    for (auto& si : vals) {
        
        ImGui::PushID(si.day_.id);

        ImGui::Text(si.day_text_.c_str());
        ImGui::SameLine();

        ImGui::ColorButton("###resultinfo", ImVec4(1 - si.result_estimation_, si.result_estimation_, 0, 0));
        if (ImGui::BeginPopupContextItem("Legend item")) {
            ImGui::Text(si.legend_.c_str());
            ImGui::EndPopup();
        }

        ImGui::PopID();
        //ImGui::Text(std::to_string(si.result_estimation_).c_str());
        //ImGui::SameLine();
        //ImGui::Text(si.legend_.c_str());
    }

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

    if (start == end || start < 0 || end < 0) {
        return;
    }

    graph_ = req_measure_graph_for(start, end);
    measure_groups_.assign(graph_.begin(), graph_.end(), [](auto& p) {
        return ((measure_group)p.first).name;
    });

    current_mg_id_ = 0;

    result_info_.clear();
    for (auto&[mg_id, vec] : graph_) {

        std::unordered_map<int, std::vector<value>> day_to_meas;
        for (auto& value : vec) {
            day_to_meas[value.value_.day.id].push_back(value.value_);
        }

        for (auto&[day_id, vals] : day_to_meas) {
            day_group_stat& stat = result_info_[mg_id].emplace_back();

            auto max_weight = accumulate(vals, [](value const& v) {
                return cache().get<estimation>(v.estimation.id)->weight;
            });

            stat.day_ = get_model<day>(day_id);
            stat.result_estimation_ = 0;

            osstream os(stat.legend_);
            os << std::fixed;
            os.precision(2);

            double result_weight = 0.0;

            size_t count = vals.size();

            os << "(";
            for (auto& val : vals) {
                estimation e = val.estimation;
                measure m = e.measure;

                bool reverse = static_cast<bool>(e.reverse);
                
                switch (m.type) {
                case measure_type::boolean: {

                    bool value = static_cast<bool>(val.val); 
                    int res = (reverse ^ value);

                    result_weight += res * e.weight;

                    os << res << "*" << e.weight;

                } break;
                case measure_type::numeric: {

                    double diff = (reverse ? e.border - val.val : val.val);

                    os << "(" << diff << "/" << e.border << ")*" << e.weight;

                    result_weight += (diff / e.border) * e.weight;                    

                }};
                os << "[" << m.name << "]";

                if (--count != 0) {
                    os << " + \n";
                }

            }
            os << ")\n/" << max_weight << " = " << (result_weight / max_weight);

            stat.result_estimation_ = result_weight / max_weight;
            stat.values_ = std::move(vals);
            stat.day_text_ = format("%m.%d", stat.day_.day_timestamp);
        }

        // sort by day date
        auto& stats = result_info_[mg_id];
        std::sort(stats.begin(), stats.end(), [](day_group_stat const& lhs, day_group_stat const& rhs) {
                return lhs.day_.day_timestamp < rhs.day_.day_timestamp;
        });   
    }
}
