
#include <settings/settings.hpp>
#include "combo_box_content.hpp"
#include <imgui/imgui.h>
#include "window_render.hpp"


static std::string path = try_to_find_settings();
static combo_box_content settings_path_content;
static bool initial = false;
static bool show_settings = false;
static settings_vals setts;

bool force_render_settings() {
    if (path.empty()) {
        initial = true;
        show_settings = true;
    } 
    if (settings_path_content.empty()) {
        auto values = possible_paths();
        settings_path_content.assign(values.begin(), values.end());
    }
    return path.empty() || initial;
}


void render_path_choosing() {
        
}


void render_settings() {

    renderer ren("settings", &show_settings);

    if (!ren) {
        return;
    }

    ImGui::Combo("path", &settings_path_content.idx(), settings_path_content.content());
    if (settings_path_content.current_element() != path) {
        bool path_not_set = settings_path_content.idx() == -1 && !path.empty();
        bool new_path_set = settings_path_content.idx() != -1;

        if (path_not_set) {
            settings_path_content.setup_element(path);
        } else if (new_path_set) {
            path = settings_path_content.current_element();
        } else {
            assert(false);
        }
        create_settings(path);
        setts = settings().get();
        return;
    }

    ImGui::InputText("dp_path")




    
}
