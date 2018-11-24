#pragma once

#include <string>
#include <memory>
#include <vector>

struct settings_vals {
    std::string db_path_;
};

struct settings_impl {

    settings_impl(std::shared_ptr<settings_vals> vals) : instance_(vals) {}

    std::string const& db_path() const { return instance_->db_path_; }

    settings_vals const& get() { return *instance_; }

private:
    std::shared_ptr<settings_vals> instance_;
};

std::string try_to_find_settings();
std::vector<std::string> possible_paths();


void save_settings_file(std::string const& path, settings_vals const& values);

settings_impl settings();
void create_settings(std::string const& file_path);
