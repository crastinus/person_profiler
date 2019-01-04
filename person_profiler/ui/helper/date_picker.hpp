#pragma once


#include <time.h>
#include <imgui/imgui.h>
#include <ui/ui.hpp>
#include <functional>

struct date_picker_button;

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

    date_picker_button* callback_;

    std::string date_;

    date_picker(time_t time = time(nullptr), date_picker_button* callback = nullptr);

    char const* name() override {
        return "date picker";
    }

    void render() override;

    void render_days();

    void render_months();

    void render_years();

    void on_change_state();

    std::string render_date(char const* format);

    void generate_days();

    void generate_years();

    void render_prev();
    void render_next();

    bool render_int(int i);

    void on_date();
 };

struct date_picker_button {

    date_picker_button(int id, std::function<void(time_t)> func);
    ~date_picker_button();

    void render();

    time_t time() const;
    void set_time(time_t time);
    

private:

    void update(time_t time);

    int id_;
    time_t time_;
    std::string output_string_;
    std::function<void(time_t)> callback_;

    std::shared_ptr<date_picker> dp_;

    friend struct date_picker;

};