#pragma once
#include <string>
#include <vector>

struct input_text {
    std::vector<char> data_;
    std::string& storage_;
    std::string name_;
    float width_;
    float height_;

    input_text(std::string& ref, std::string const& name, float width_ = 0, float height_= 0);
    ~input_text();

    bool render_input();
    bool render_multiline();

    void update();

    void shrink();

private:

    bool on_change();

};

struct input_float {

    float   value_ = 0;
    double* double_ptr_ = nullptr;
    float*  value_ptr_ = nullptr;
    std::string name_;
    float       step_;

    input_float(double* d, std::string const& name, float step = 0);
    input_float(float* f, std::string const& name, float step = 0);

    void render();

private:
    void on_change();
};

struct input_int {
    int* int_ptr_ = nullptr;
    std::string name_;

    input_int(int* value, std::string const& name);

    void render();

private:
    //void on_change();
 };