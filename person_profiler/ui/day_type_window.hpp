#pragma once

#include "ui.hpp"
#include <model/day.hpp>
#include <functional>

struct day_type_button;

struct day_type_window : public window_inst {

    day_type_window(day_type_button* callback = nullptr);

    virtual char const* name() override {
        return "Day type";
    }

    virtual void render() override;

private:
    day_type_button* callback_;
    std::vector<day_type> active_types_;

};

// sugar for calling day_type_window
struct day_type_button {

    day_type_button(day_type d = {}, std::function<void(day_type)> callback = {});
    ~day_type_button();

    void render(bool blocked =false);

    day_type const& value() const;

private:

    void update_text(day_type const& d);
    void update(day_type d);


    day_type day_type_;
    std::string text_;
    std::function<void(day_type)> callback_;
    std::shared_ptr<day_type_window> dt_win_;

    friend struct day_type_window;

};

