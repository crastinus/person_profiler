#include "measure.hpp"
#include <db/model.hpp>


measure_id::operator measure const() {
    return get_model<measure>(id);
}

measure_id & measure_id::operator=(int id) {
    this->id = id;
    return *this;
}
