#include "settings.hpp"
#include <json/json.hpp>
#include <memory>
#include <fstream>

#include <windows.h>
#include <shlobj_core.h>
#include "..\ui\settings.hpp"
/*
template<typename ValueType>
auto parse_value(nlohmann::json& v);

template<> auto parse_value<std::string>(nlohmann::json& v) { return static_cast<std::string>(v); }
*/
template<typename ValueType>
void read_json(nlohmann::json& js, char const* name, ValueType& value) {
    if (js[name].is_null()) {
        value = {};
        return;
    }

    value = js[name];

}

static std::shared_ptr<settings_vals> instance;
void create_settings(std::string const& file_path) {
    std::ifstream in(file_path);
    if (!in.good()) {
        throw std::runtime_error("Settings file doesn't found");
    }

    std::string content = std::string(std::istreambuf_iterator<char>(in), std::istreambuf_iterator<char>());

    nlohmann::json value;
    value.parse(content);
    if (value.is_null()) {
        throw std::runtime_error("Wrong settings file");
    }
    
    auto new_instance = std::make_shared<settings_vals>();
    read_json(value, "db", new_instance->db_path_);
    instance = new_instance;    
}

void save_settings_file(std::string const & path, settings_vals const& values) {
    std::ofstream out(path);

    nlohmann::json value;
    value["db"] = values.db_path_;
    out << value;
}


settings_impl settings() {
    return settings_impl(instance);
}


static std::string systemPath(int value) {
    char Folder[1024] = {};
    HRESULT hr = SHGetFolderPathA(nullptr, value, 0, SHGFP_TYPE_CURRENT, Folder);
    if (SUCCEEDED(hr))
    {
        //char str[1024];
        //wcstombs(str, Folder, 1023);
        return Folder;
    }
    else return "";

}

std::vector<std::string> possible_paths() {
    std::string fname = "\\person_profiler.json";
    return {
        systemPath(CSIDL_PERSONAL) + fname,
        systemPath(CSIDL_LOCAL_APPDATA) + fname,
        "." + fname
    };

}


inline bool file_exists(const std::string& name) {
    if (FILE *file = fopen(name.c_str(), "r")) {
        fclose(file);
        return true;
    }
    else {
        return false;
    }
}

// empty path if unable to find settings
std::string try_to_find_settings() {

    for (std::string const& path : possible_paths()) {
        if (file_exists(path)) {
            return path;
        }
    }

    return "";
}

