#pragma once

#include <model/day.hpp>
#include <model/estimation.hpp>
#include <model/measure.hpp>
#include <model/value.hpp>

#include <unordered_map>
#include <system/array_view.hpp>

struct db_cache {

    void save(array_view<day> v);
    void save(array_view<day_type> v);
    void save(array_view<measure> v);
    void save(array_view<value> v);
    void save(array_view<estimation> v);
    void save(array_view<measure_group> v);

    void del(array_view<day> v);
    void del(array_view<day_type> v);
    void del(array_view<measure> v);
    void del(array_view<value> v);
    void del(array_view<estimation> v);
    void del(array_view<measure_group> v);

    template<typename T>
    T const* get(int id) const;
    
private:

    std::unordered_map<int, day> day_cache_;
    std::unordered_map<int, day_type> day_type_cache_;
    std::unordered_map<int, measure> measure_cache_;
    std::unordered_map<int, measure_group> measure_group_cache_;
    std::unordered_map<int, value> value_cache_;
    std::unordered_map<int, estimation> estimation_cache_;

    static db_cache cache;
    friend db_cache& cache();
};

db_cache& cache();