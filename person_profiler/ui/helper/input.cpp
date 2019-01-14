#include "input.hpp"
#include <algorithm>
#include <imgui/imgui.h>

input_text::input_text(std::string& ref, std::string const& name, float width, float height)
    :  storage_(ref), name_(name), width_(width), height_(height)
{
    update();
}

input_text::~input_text() {
    shrink();
}

bool input_text::render_input() {
    if (ImGui::InputText(name_.c_str(), &*storage_.begin(), storage_.size())) {
        return on_change();
    }

    return false;
}

bool input_text::render_multiline() {
    if (ImGui::InputTextMultiline(name_.c_str(), &*storage_.begin(), storage_.size(), ImVec2(width_,height_))) {
        return on_change();
    }
    return false;
}

void input_text::update() {
    size_t prealloc = (height_ == 0 ? 1024 : 16384);
    size_t old_size = storage_.size();
    size_t new_size = (old_size == 0 ? prealloc : old_size * 2 + prealloc);
    storage_.resize(new_size);
}

void input_text::shrink() {
    storage_.resize(std::char_traits<char>::length(storage_.c_str()));
}

bool input_text::on_change() {
    size_t len = std::char_traits<char>::length(storage_.c_str());
    if (len >= storage_.size() - 2) {
        storage_.resize(storage_.size() * 2);
    }
    return true;
}

input_float::input_float(double* d, std::string const& name, float step) : value_(static_cast<float>(*d)), double_ptr_(d), value_ptr_(&value_), name_(name), step_(step) {}
input_float::input_float(float* f, std::string const& name, float step) : double_ptr_(nullptr), value_ptr_(f), name_(name), step_(step) {}

void input_float::render() {
    if (ImGui::InputFloat(name_.c_str(), value_ptr_, step_)) {
        on_change();
    }
}

void input_float::on_change() {
    if (double_ptr_ != nullptr) {
        *double_ptr_ = value_;
    }
}

input_int::input_int(int* value, std::string const& name) : int_ptr_(value), name_(name) {}

void input_int::render() {
    if (ImGui::InputInt(name_.c_str(), int_ptr_)) {

    }
}

