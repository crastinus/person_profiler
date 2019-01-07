#include "statistics_window.hpp"
#include <db/cache.hpp>
#include <db/measure.hpp>
#include <db/model.hpp>
#include <db/options.hpp>
#include <model/measure_graph.hpp>
#include <common/osstream.hpp>
#include <common/time.hpp>
#include <common/split.hpp>
#include <common/join_through.hpp>


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
    end_(1, [this](time_t t) {this->on_update_time(false,t); })    
{
    start_.set_time(time(nullptr));
    end_.set_time(time(nullptr));
    restore_showed_measures();
}

void statistics_window::render() {

    constexpr size_t interval = 150;

    start_.render();
    ImGui::SameLine();
    end_.render();

    auto start_x = ImGui::GetCursorScreenPos().x;

    float alignment = 0;
    size_t counter = 0;

    for (auto&[mg_id, vals] : result_info_) {
        (void)vals;
        
        // check measure groups for showing
        bool show = (allowed_measure_groups_.count(mg_id.id) > 0);
        if (ImGui::Checkbox(measure_group_names_[mg_id.id].c_str(), &show)) {
            if (show) {
                allowed_measure_groups_.insert(mg_id.id);
            }
            else {
                allowed_measure_groups_.erase(mg_id.id);
            }
            save_showed_measures();
        }

        auto end_x = ImGui::GetItemRectMax().x;

        alignment += interval;
        if (++counter == result_info_.size() || alignment < end_x - start_x || alignment > interval*2  ) {
            alignment = 0;
        }
        else {
            ImGui::SameLine(alignment);
        }
    }

    if (measure_group_names_.empty()) {
        return;
    }

    std::unordered_map<int, float> meas_id_to_pos;
    
    counter = 0;

    ImGui::SetCursorPosX(50);
    for (int meas_id : allowed_measure_groups_) {

        float start_pos = ImGui::GetCursorPos().x;
        ImGui::Text(measure_group_names_.at(meas_id).c_str());
        float end_pos = ImGui::GetItemRectMax().x - ImGui::GetCursorScreenPos().x;

        meas_id_to_pos[meas_id] = (end_pos + start_pos) / 2;
        if (counter++ != allowed_measure_groups_.size()-1) {
            ImGui::SameLine(end_pos + 30);
        }
        
    }
    
    for (day const& d : days_) {
        ImGui::PushID(d.id);
        ImGui::Text(format("%m.%d", d.day_timestamp).c_str());
        
        for (int meas_id : allowed_measure_groups_) {

            ImGui::PushID(meas_id);
            
            auto& dict = result_info_.at({ meas_id });
            if (dict.find(d.id) != dict.end()) {

                ImGui::SameLine(meas_id_to_pos.at(meas_id));

                auto& si = dict.at(d.id);
                ImGui::ColorButton("###resultinfo", ImVec4(1 - si.result_estimation_, si.result_estimation_, 0, 0));
                if (ImGui::BeginPopupContextItem("Legend item")) {
                    ImGui::Text(si.legend_.c_str());
                    ImGui::EndPopup();
                }         
            }
             ImGui::PopID();
        }
        ImGui::PopID();
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

    current_mg_id_ = 0;

    std::unordered_set<int> day_ids;

    result_info_.clear();
    for (auto&[mg_id, vec] : graph_) {

        measure_group_names_[mg_id.id] = get_model<measure_group>(mg_id.id).name;

        std::unordered_map<int, std::vector<value>> day_to_meas;

        for (auto& value : vec) {
            day_to_meas[value.value_.day.id].push_back(value.value_);
            day_ids.insert(value.value_.day.id);
        }
        
        for (auto&[day_id, vals] : day_to_meas) {
            day_group_stat& stat = result_info_[mg_id][day_id];

            auto max_weight = accumulate(vals, [](value const& v) {
                return cache().get<estimation>(v.estimation.id)->weight;
            });

            stat.result_estimation_ = 0;

            osstream os(stat.legend_);
            os << std::fixed;
            os.precision(2);

            double result_weight = 0.0;

            size_t count = vals.size();

            // legend and data for every day of every estimation

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

            stat.result_estimation_ = static_cast<float>(result_weight / max_weight);
            stat.values_ = std::move(vals);
        }
    }

    // fill days
    for (auto id : day_ids) {
        days_.push_back(get_model<day>(id));
    }

    std::sort(days_.begin(), days_.end(), [](day const& lhs, day const& rhs) {
        return lhs.day_timestamp < rhs.day_timestamp;
    });

    // clean old allowed measure groups
    for (auto mg_it = allowed_measure_groups_.begin(); mg_it != allowed_measure_groups_.end(); ) {
        if (!measure_group_names_.empty() && measure_group_names_.find(*mg_it) == measure_group_names_.end()) {
            mg_it = allowed_measure_groups_.erase(mg_it);
            continue;
        }
        ++mg_it;
    }

    save_showed_measures();
}

void statistics_window::save_showed_measures() {
    save_option("statistics_measure_group_ids", join_through(allowed_measure_groups_, ","));
}

void statistics_window::restore_showed_measures() {
    std::string mg_ids_str = get_option("statistics_measure_group_ids");

    allowed_measure_groups_.clear();
    for (int mg_id : split_to<int>(mg_ids_str,",")) {
        allowed_measure_groups_.insert(mg_id);
    }

}
