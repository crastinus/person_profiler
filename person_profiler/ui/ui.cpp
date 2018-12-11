#include "ui.hpp"
#include "day_window.hpp"
#include <imgui/imgui.h>
#include <imgui/opengl/run.hpp>
#include "main_window.hpp"

windows_storage windows_storage::storage;

void windows_storage::render_all() {
    
    for (auto it = windows.begin(); it != windows.end(); ) {

        auto win = it->second;

        // preventing closed window rendering
        if (win->is_closed()) {
            it = windows.erase(it);
            continue;
        }

        ImGui::Begin(win->name(), &win->show_, win->initial_size());
        win->render();
        ImGui::End();

        win->after_render();

        ++it;
    }
}

void windows_storage::close(char const * name) {
}

windows_storage & windows_storage::instance() {
    return storage;
}

#include "helper/input.hpp"

//struct test_window : public window_inst {
//
//    int value_1 = 100;
//    double value2 = 0.1;
//    float value3 = 0.42;
//    std::string text = "hello,there";
//
//    input_text in_txt;
//    input_int in_int;
//    input_float in_doub, in_flt;
//
//    test_window() : in_txt(text, "text"), in_int(&value_1, "int"), in_doub(&value2, "double"), in_flt(&value3, "float") {}
//
//    virtual char const* name() {
//        return "test_window";
//    }
//
//    virtual void render() {
//        in_txt.render_input();
//        in_int.render();
//        in_doub.render();
//        in_flt.render();
//    }
//
//};

void run_windows() {
   // window<test_window>();
    window<day_window>();
    window<main_window>();
    imgui_run([](void*){
        windows_storage::instance().render_all();
    });
}
