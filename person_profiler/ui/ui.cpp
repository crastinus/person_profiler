#include "ui.hpp"
#include "day_window.hpp"
#include <imgui/imgui.h>
#include <imgui/opengl/run.hpp>
#include "main_window.hpp"
#include <unordered_set>

windows_storage windows_storage::storage;

void windows_storage::render_all() {
    
    for (auto it = windows.begin(); it != windows.end(); ) {

        auto win = it->second;

        // preventing closed window rendering
        if (!win->show_) {
            it = windows.erase(it);
            continue;
        }

        // save render window
        render_window(win.get());

        ++it;
    }
}

void windows_storage::close(char const * name) {
}

struct ScopeRender {
    ScopeRender(window_inst* win) {
        ImGui::Begin(win->name(), &win->show_, win->initial_size());
    }
    ~ScopeRender() {
        ImGui::End();
    }
};

void windows_storage::render_window(window_inst * win) try {
    save_rendering(win, &window_inst::before_render, &win->before_render_operation_running_);

    {
        ScopeRender render(win);
        win->render_errors();
        save_rendering(win, &window_inst::render, &win->render_operation_running_);
    }

    save_rendering(win, &window_inst::after_render, &win->after_render_operation_running_);
}
catch (std::exception& ex) {
    std::string exception = "Exception ";
    win->save_error(exception + ex.what());
}
catch (...) {
    win->save_error("Unknown exception");
}

void windows_storage::save_rendering(window_inst* win, void(window_inst::* func)(), size_t * err_cnt) {

    constexpr size_t max_errors_count = 5;

    if (*err_cnt >= max_errors_count) {
        return;
    }

    ++*err_cnt;
    (win->*func)();
    *err_cnt = 0;

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

void window_inst::render_errors() {
    constexpr size_t max_errors_count = 5;

    if (errors_.empty()) {
        return;
    }

    if (before_render_operation_running_ > 0) {
        ImGui::Text("Fatal exception in before_render");
    }

    if (render_operation_running_ > 0) {
        ImGui::Text("Fatal exception in render");
    }

    if (after_render_operation_running_ > 0) {
        ImGui::Text("Fatal exception in after_render");
    }

    if (errors_.size() > max_errors_count) {
        errors_.erase(errors_.begin(), errors_.begin() + 1);
    }

    for (std::string const& err : errors_) {
        ImGui::Text(err.c_str());
    }
}

void window_inst::save_error(std::string const & err) {
    auto it = std::find(errors_.begin(), errors_.end(), err);
    if (it != errors_.end()) {
        errors_.erase(it);
    }

    errors_.push_back(err);
}
