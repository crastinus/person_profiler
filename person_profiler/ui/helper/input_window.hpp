#include <ui/ui.hpp>

#include "input.hpp"
#include <functional>
#include <imgui/imgui.h>


struct input_window : public window_inst {

    std::string name_;
    std::function<void(std::string)> callback_;
    std::string text_;
    input_text input_;

    input_window(std::string  name, std::function<void(std::string)> callback) : name_("Input " + name), callback_(callback), input_(text_, "###text") {}

    char const* name() override {
        return name_.c_str();
    }

    void render() override {
        input_.render_input();
        ImGui::SameLine();
        if (ImGui::Button("OK")) {
            input_.shrink();
            callback_(text_);
            close();
        }
    }
};