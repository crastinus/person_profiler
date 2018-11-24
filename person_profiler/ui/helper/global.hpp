#pragma once
#include <imgui/imgui.h>

inline float button_width() {
    return 60;
}

inline float split_width() {
    return 5;
}

inline ImVec2 button_size() {
    return ImVec2(button_width(), 0);
}

inline ImVec2 big_button_size() {
    return ImVec2(button_width()*2, 0);
}

//// returns size of button and forward position on button size
//inline ImVec2 bpos(float& position) {
//    position += button_width() + split_width();
//    return ImVec2(button_width(), 0);
//}
