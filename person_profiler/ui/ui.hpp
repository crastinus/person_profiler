#pragma once
#include <string>
#include <unordered_map>
#include <memory>

struct window_inst {
    
    bool show_ = true;
    bool closed_ = false;

    virtual char const* name() = 0;
    virtual void render() = 0;
    virtual void after_render() {}

    void close() {
        closed_ = true;
    }

    bool is_closed() {
        return closed_;
    }
};

struct windows_storage {
private:
    std::unordered_map<std::string, std::shared_ptr<window_inst>> windows;
    static windows_storage storage;
public:
    
    template<typename ConcreteWindow, typename ... Args>
    void create(Args&& ... args);

    void render_all();   

    void close(char const* name);

    static windows_storage& instance();
};

template<typename ConcreteWindow, typename ...Args>
inline void windows_storage::create(Args&& ... args) {
    auto win = std::make_shared<ConcreteWindow>(std::forward<Args>(args)...);
    windows[win->name()] = win;
}

template<typename ConcreteWindow, typename ... Args>
inline void window(Args&& ... args) {
    windows_storage::instance().create<ConcreteWindow>(std::forward<Args>(args)...);
}

void run_windows();