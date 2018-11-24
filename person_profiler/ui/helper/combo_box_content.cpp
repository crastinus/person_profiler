#include "combo_box_content.hpp"
#include <set>

combo_box_content::combo_box_content(std::initializer_list<std::string>&& list, int default_idx)
    : source_(list)
    , idx_(default_idx) {
    create_content();
}

combo_box_content::~combo_box_content() {}

combo_box_content& combo_box_content::operator=(std::vector<std::string> const& other) {
    std::string current = current_element();
    source_ = other;

    create_content();

    idx_ = -1;
    for (int index = 0; index < source_.size(); ++index) {
        if (source_[index] == current) {
            idx_ = index;
            break;
        }
    }

    return *this;
}

void combo_box_content::setup_element(std::string const& value) {
    for (int index = 0; index < source_.size(); ++index) {
        if (source_[index] == value) {
            idx_ = index;
            return;
        }
    }
}

std::string combo_box_content::current_element() const {
    if (idx_ == -1) {
        return "";
    }

    return source_.at(idx_);
}

int& combo_box_content::idx() {
    return idx_;
}

char const* combo_box_content::content() const {
    return content_.c_str();
}

bool combo_box_content::add(std::string const & new_value) {
    for (std::string const& src : source_) {
        if (src == new_value) {
            return false;
        }
    }

    source_.push_back(new_value);
    create_content();
    return true;
}

void combo_box_content::create_content() {
    content_.clear();
    for (std::string const& sym : source_) {
        content_ += sym;
        content_ += '\0';
    }
    content_ += '\0';
}

bool combo_box_content::empty() {
    return content_.empty();
}