#pragma once


#include <time.h>
#include <imgui/imgui.h>
#include <ui/ui.hpp>

struct date_picker : public window_inst {

    enum class state {
        days = 1,
        months = 2,
        years = 3
    };

    static constexpr char const* months[] = { "January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December" };

    state state_;

    time_t time_;

    int year;
    int month;
    int day;

    int ryear;
    int rmonth;
    int rday;

    // shift = 0 ? monday
    // shift = 1 ? thuesday
    int shift = 0;

    std::vector<int> days_;
    std::vector<int> years_;

    std::string date_;

    date_picker(time_t time = time(nullptr)) 
        : state_(state::days),
        time_(time)
    {
        struct tm t;
        gmtime_s(&t, &time_);
        ryear = year = t.tm_year;
        rmonth = month = t.tm_mon;
        rday = day = t.tm_mday;

        on_change_state();
    }

    char const* name() override {
        return "date picker";
    }

    void render() override {
        switch (state_) {
        case state::days: render_days(); break;
        case state::months: render_months(); break;
        case state::years: render_years(); break;
        }
    }

    void render_days() {
        if (ImGui::Button(date_.c_str())) {
            state_ = state::months;
            on_change_state();
            return;
        }

        int mon_idx = 0;

        int wdth = 40;

        // render header
        for (char const* day : { "mon", "thu", "wed", "thrs", "fri", "sat", "sun" }) {
            ImGui::Text(day);
            if (mon_idx != 6) {
                ImGui::SameLine(wdth*++mon_idx);
            }
        }

        ImGui::Text("");

        constexpr int row = 7;
        for (int i = 0; i < days_.size(); ++i) {
            if (days_[i] == 0) {
                continue;
            }

            if (i % row != 0) {
                ImGui::SameLine((i % row) * wdth);
            }

            if (render_int(days_[i])) {
                rday = days_[i];
            }

            
        }


    }

    void render_months() {
        if (ImGui::Button(date_.c_str())) {
            state_ = state::years;
            on_change_state();
            return;
        }

        constexpr int row = 3;
        for (int id = 0; id < sizeof(months)/sizeof(char const*); ++id) {
            if (ImGui::Button(months[id])) {
                state_ = state::days;
                rmonth = id;
                on_change_state();
                break;
            }

            if ((id+1) % row != 0) {
                ImGui::SameLine(120*((id+1)%row));
            }

        }        
    }

    void render_years() {
        constexpr int row = 3;
        int row_id = 0;
        for (int year : years_) {
            
            if (render_int(year + 1900)) {
                ryear = year;
                state_ = state::months;
                on_change_state();
                break;
            }

            if (++row_id % row != 0) {
                ImGui::SameLine();
            }

        }
    }

    void on_change_state() {
        if (state_ == state::days) {
            date_ = render_date("%Y.%m");
            generate_days();
        }  else if (state_ == state::years) {
            generate_years();
        } else if (state_ == state::months) {
            date_ = render_date("%Y");
        }
    }

    std::string render_date(char const* format) {
        struct tm t {};

        t.tm_mday = rday;
        t.tm_mon = rmonth;
        t.tm_year = ryear;

        char buffer[128] = {};
        strftime(buffer, sizeof(buffer), format, &t);

        return buffer;
    }

    //void compute_shift() {
    //    struct tm t {};
    //    t.tm_mday = 1;
    //    t.tm_mon = rmonth;
    //    t.tm_year = ryear;
    //    _mkgmtime(&t);
    //    shift = t.tm_wday;
    //}

    void generate_days() {
        
        struct tm t {};
        t.tm_mday = 1;
        t.tm_mon = rmonth;
        t.tm_year = ryear;

        time_t start_ts = _mkgmtime(&t); 
        
        int shift = t.tm_wday;
        t.tm_mon += 1;

        time_t end_ts = _mkgmtime(&t);

        days_.clear();
        for (int i = 0; i < shift; ++i) {
            days_.push_back(0);
        }
        for (int i = 0; i < (end_ts - start_ts) / 86400; ++i) {
            days_.push_back(i+1);
        }
    }

    void generate_years() {
        years_.clear();
        for (int i = ryear - 9; i < ryear + 10; ++i) {
            years_.push_back(i);
        }
    }

    bool render_int(int i) {
        char buffer[20] = {};
        sprintf(buffer, "%d", i);
        return ImGui::Button(buffer);
    }


};