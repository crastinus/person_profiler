#include "cache.hpp"


db_cache db_cache::cache;

void db_cache::save(array_view<day> v) {
    for (auto& d : v) {
        day_cache_[d.id] = d;
    }
}

void db_cache::save(array_view<day_type> v) {
    for (auto& d : v) {
        day_type_cache_[d.id] = d;
    }
}

void db_cache::save(array_view<measure> v) {
    for (auto& d : v) {
        measure_cache_[d.id] = d;
    }
}

void db_cache::save(array_view<value> v) {
    for (auto& d : v) {
        value_cache_[d.id] = d;
    }
}

void db_cache::save(array_view<estimation> v) {
    for (auto& d : v) {
        estimation_cache_[d.id] = d;
    }
}

void db_cache::save(array_view<measure_group> v) {
    for (auto& d : v) {
        measure_group_cache_[d.id] = d;
    }
}


void db_cache::del(array_view<day> v) {
    for (auto& d : v) {
        day_cache_.erase(d.id);
    }
}

void db_cache::del(array_view<day_type> v) {
    for (auto& d : v) {
        day_type_cache_.erase(d.id);
    }
}


void db_cache::del(array_view<measure> v) {
    for (auto& d : v) {
        measure_cache_.erase(d.id);
    }
}

void db_cache::del(array_view<measure_group> v) {
    for (auto& d : v) {
        measure_group_cache_.erase(d.id);
    }
}

void db_cache::del(array_view<value> v) {
    for (auto& d : v) {
        value_cache_.erase(d.id);
    }
}

void db_cache::del(array_view<estimation> v) {
    for (auto& d : v) {
        estimation_cache_.erase(d.id);
    }
}

template<typename T>
inline T const* get_impl(int id, std::unordered_map<int, T> const& ref) {
    auto it = ref.find(id);
    if (it == ref.end()) {
        return nullptr;
    }
    return &it->second;
}

template<>
day const* db_cache::get(int id) const {
    return get_impl(id, day_cache_);
}

template<>
day_type const* db_cache::get(int id) const {
    return get_impl(id, day_type_cache_);
}

template<>
measure const* db_cache::get(int id) const {
    return get_impl(id, measure_cache_);
}

template<>
measure_group const* db_cache::get(int id) const {
    return get_impl(id, measure_group_cache_);
}

template<>
estimation const* db_cache::get(int id) const {
    return get_impl(id, estimation_cache_);
}

template<>
value const* db_cache::get(int id) const {
    return get_impl(id, value_cache_);
}

db_cache & cache(){
    return db_cache::cache;
}
