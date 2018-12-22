#pragma once

#include <imgui/imgui.h>
#include <string>

struct id_scope {

    id_scope(int id) {
        ImGui::PushID(id);
    }

    id_scope(char const* id) {
        ImGui::PushID(id);
    }

    id_scope(std::string const& id)
        : id_scope(id.c_str()) {}

    ~id_scope() {
        ImGui::PopID();
    }


};