#pragma once

#include <string>


struct measure_group {
    int id = 0;
    std::string name;
    bool        active;
};

struct measure_group_id {
    int id;

    operator measure_group const();

    measure_group_id& operator = (int id);

    bool operator<(measure_group_id const& other) const;
    bool operator==(measure_group_id const& other) const;
};

struct measure_group_id_hash {
    size_t operator()(measure_group_id const& id) const {
        return std::hash<int>()(id.id);
    }
};