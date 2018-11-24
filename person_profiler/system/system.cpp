#include "system.hpp"

#include <windows.h>
#include <shlobj_core.h>

// CSIDL_PERSONAL CSIDL_LOCAL_APPDATA is useful
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

std::string documents() {
    return systemPath(CSIDL_PERSONAL);
}

bool file_exists(std::string const& name) {
    if (FILE *file = fopen(name.c_str(), "r")) {
        fclose(file);
        return true;
    }
    else {
        return false;
    }
}