#include "date_picker.hpp"

date_picker::date_picker(time_t time, date_picker_button * callback)
    : state_(state::days),
    time_(time),
    callback_(callback)
{
    struct tm t;
    gmtime_s(&t, &time_);
    ryear = year = t.tm_year;
    rmonth = month = t.tm_mon;
    rday = day = t.tm_mday;

    on_change_state();
}

void date_picker::render() {
    switch (state_) {
    case state::days: render_days(); break;
    case state::months: render_months(); break;
    case state::years: render_years(); break;
    }
}

 void date_picker::render_days() {
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
            on_date();
        }
    }
}

 void date_picker::render_months() {
    if (ImGui::Button(date_.c_str())) {
        state_ = state::years;
        on_change_state();
        return;
    }

    constexpr int row = 3;
    for (int id = 0; id < sizeof(months) / sizeof(char const*); ++id) {
        if (ImGui::Button(months[id])) {
            state_ = state::days;
            rmonth = id;
            on_change_state();
            break;
        }

        if ((id + 1) % row != 0) {
            ImGui::SameLine(120 * ((id + 1) % row));
        }

    }
}

 void date_picker::render_years() {
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

 void date_picker::on_change_state() {
    if (state_ == state::days) {
        date_ = render_date("%Y.%m");
        generate_days();
    }
    else if (state_ == state::years) {
        generate_years();
    }
    else if (state_ == state::months) {
        date_ = render_date("%Y");
    }
}

 std::string date_picker::render_date(char const * format) {
    struct tm t {};

    t.tm_mday = rday;
    t.tm_mon = rmonth;
    t.tm_year = ryear;

    char buffer[128] = {};
    strftime(buffer, sizeof(buffer), format, &t);

    return buffer;
}

 void date_picker::generate_days() {

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
        days_.push_back(i + 1);
    }
}

 void date_picker::generate_years() {
    years_.clear();
    for (int i = ryear - 9; i < ryear + 10; ++i) {
        years_.push_back(i);
    }
}

 bool date_picker::render_int(int i) {
    char buffer[20] = {};
    sprintf(buffer, "%d", i);
    return ImGui::Button(buffer);
}

 void date_picker::on_date() {
     struct tm t {};
     t.tm_mday = rday;
     t.tm_year = ryear;
     t.tm_mon = rmonth;
     time_ = _mkgmtime(&t);

     if (callback_ != nullptr) {
         callback_->update(time_);
     }
 }

 date_picker_button::date_picker_button(std::function<void(time_t)> func)
     : callback_(func) {
 }

 date_picker_button::~date_picker_button() {
     if (dp_) {
         dp_->close();
     }
 }

 void date_picker_button::render()  {
     if (ImGui::Button(output_string_.c_str())) {
         dp_ = window<date_picker>(time_, this);
     }
 }

 time_t date_picker_button::time() const {
     return time_;
 }

 void date_picker_button::set_time(time_t time) {
     update(time);
 }

 void date_picker_button::update(time_t time) {
     time_ = time;

     struct tm t {};
     ::gmtime_s(&t, &time_);

     char buffer[80] = {};
     strftime(buffer, sizeof(buffer), "%Y-%m-%d", &t);

     output_string_ = buffer;

     if (callback_) {
         callback_(time_);
     }

     if (dp_) {
         dp_->close();
         dp_.reset();
     }
     
 }
