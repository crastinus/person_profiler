#pragma once
#include <string>
#include <time.h>
#include <imgui/imgui.h>
#include <functional>
#include <common/time.hpp>

struct date_widget {
    time_t timestamp_;
    std::string widget_value_;
    std::function<void()> callback_;

    date_widget(char const* form, time_t timestamp, std::function<void()> && callback)
        : timestamp_(timestamp), callback_(std::move(callback))
    {
        widget_value_ = format(form, timestamp);
    }

    void render() {
        if (ImGui::Button(widget_value_.c_str())) {
            callback_();
        }
    }

};