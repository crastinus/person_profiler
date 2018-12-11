#include "estimation.hpp"
#include <db/model.hpp>//#include "..\ui\estimation.hpp"

estimation_id::operator estimation const() {
    return get_model<estimation>(id);
}

estimation_id::operator bool const() {
    return id != 0;
}

estimation_id & estimation_id::operator=(int id) {
    this->id = id;
    return *this;
}
