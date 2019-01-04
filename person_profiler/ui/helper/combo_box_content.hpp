#pragma once

#include <string>
#include <vector>
#include <algorithm>

struct combo_box_content {
    combo_box_content(std::initializer_list<std::string>&& list = {}, int default_idx = -1);

    //    compbo_box_content(){}

    ~combo_box_content();

    combo_box_content& operator=(std::vector<std::string> const& other);

    template <typename It, typename TrasformFunctionType>
    void assign(It begin, It end, TrasformFunctionType&& fun);

    template<typename It>
    void assign(It begin, It end);

    void        setup_element(std::string const& value);
    std::string current_element() const;

    int& idx();
    char const* content() const;

    bool add(std::string const& new_value);

    bool empty() const;

private:
    void create_content();

    std::vector<std::string> source_;
    int                      idx_;
    std::string              content_;
};

template<typename It>
inline void combo_box_content::assign(It begin, It end) {
    source_.assign(begin, end);
    create_content();
}

template <typename It, typename TransformFunctionType>
inline void combo_box_content::assign(It begin, It end, TransformFunctionType&& transfun) {
    source_.clear();
    std::transform(begin, end, std::back_inserter(source_), transfun);
    create_content();
}