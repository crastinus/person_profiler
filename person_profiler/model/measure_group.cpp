#include "measure_group.hpp"
#include <db/model.hpp>


measure_group_id::operator measure_group const() {
    return get_model<measure_group>(id);
}

measure_group_id & measure_group_id::operator=(int id) {
    this->id = id;
    return *this;
}