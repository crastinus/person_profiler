#include "queries.hpp"
#include "db.hpp"

#include <model/day.hpp>
#include <model/estimation.hpp>
#include <model/measure.hpp>
#include <model/value.hpp>

estimation req_estimation(int id) {

    estimation result;

    SQLite::Statement stmt(db(), "SELECT id, started, expired, active, weight, border, reverse, measure_id, next FROM estimation WHERE id = :id");
    stmt.bind(":id", id);
    if (!stmt.executeStep()) {
        throw std::runtime_error("Estimation with id " + std::to_string(id) + " doesn't exists");
    }

    result.id = stmt.getColumn(0);
    result.started = stmt.getColumn(1);
    result.expired = stmt.getColumn(2);
    result.active = (int)stmt.getColumn(3);
    result.weight = stmt.getColumn(4);
    result.border = stmt.getColumn(5);
    result.reverse = (int)stmt.getColumn(6);
    result.measure = (int)stmt.getColumn(7);
    result.next = stmt.getColumn(8);

    return result;

}
measure req_measure(int id) {
    measure result;

    SQLite::Statement stmt(db(), "SELECT id, name, type, measure_group_id FROM measure WHERE id = :id");
    stmt.bind(":id", id);
    if (!stmt.executeStep()) {
        throw std::runtime_error("Measure with id " + std::to_string(id) + " doesn't exists");
    }

    result.id = stmt.getColumn(0);
    result.name = stmt.getColumn(1).getString();
    result.type = static_cast<measure_type>((int)stmt.getColumn(2));
    result.measure_group = stmt.getColumn(3);

    return result;
}

measure_group req_measure_group(int id) {
    measure_group result;

    SQLite::Statement stmt(db(), "SELECT id, name, active  FROM measure_group WHERE id = :id");
    stmt.bind(":id", id);
    if (!stmt.executeStep()) {
        throw std::runtime_error("Measure with id " + std::to_string(id) + " doesn't exists");
    }

    result.id = stmt.getColumn(0);
    result.name = stmt.getColumn(1).getString();
    result.active = (int)stmt.getColumn(2);

    return result;
}


value req_value(int id) {
    value result;

    SQLite::Statement stmt(db(), "SELECT id, value, day_id, estimation_id FROM value WHERE id = :id");
    stmt.bind(":id", id);
    if (!stmt.executeStep()) {
        throw std::runtime_error("Value with id " + std::to_string(id) + " doesn't exists");
    }

    result.id = stmt.getColumn(0);
    result.val = stmt.getColumn(1);
    result.day = stmt.getColumn(2);
    result.estimation = stmt.getColumn(3);

    return result;
}
