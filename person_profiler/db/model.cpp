#include "model.hpp"
#include <model/day.hpp>
#include <model/estimation.hpp>
#include <model/measure.hpp>
#include <model/value.hpp>
#include "queries.hpp"
#include "cache.hpp"

template<typename ModelType>
inline ModelType get_model_impl(int id, ModelType(*queryFunction)(int)) {
    ModelType const* v = db_cache().get<ModelType>(id);
    if (v == nullptr) {
        ModelType result = queryFunction(id);
        db_cache().save(array_view<ModelType>(result));
        return result;
    }
    return *v;
}

template<>
value get_model<value>(int id) {
    return get_model_impl<value>(id, &req_value);
}

template<>
measure get_model<measure>(int id) {
    return get_model_impl<measure>(id, &req_measure);
}

template<>
measure_group get_model<measure_group>(int id) {
    return get_model_impl<measure_group>(id, &req_measure_group);
}


template<>
estimation get_model<estimation>(int id) {
    return get_model_impl<estimation>(id, &req_estimation);
}

template<>
day get_model<day>(int id) {
    return get_model_impl<day>(id, &req_day);
}

template<>
day_type get_model<day_type>(int id) {
    return get_model_impl<day_type>(id, &req_day_type);
}