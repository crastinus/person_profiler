#include "day.hpp"
#include <model/day.hpp>
#include "db.hpp"

day req_day(int id) {

    day result;

    SQLite::Statement stmt(db(), "SELECT id, day, comment FROM day WHERE id = :id");
    stmt.bind(":id", id);
    if (!stmt.executeStep()) {
        throw std::runtime_error("Day with id " + std::to_string(id) + " doesn't exists");
    }

    result.id = stmt.getColumn(0);
    result.day_timestamp = stmt.getColumn(1);
    result.comment = stmt.getColumn(2).getString();

    return result;
}

day_type req_day_type(int id) {

    day_type result;

    SQLite::Statement stmt(db(), "SELECT id, name, active FROM day_type WHERE id = :id");
    stmt.bind(":id", id);
    if (!stmt.executeStep()) {
        throw std::runtime_error("Day type with id " + std::to_string(id) + " doesn't exists");
    }

    result.id = stmt.getColumn(0);
    result.name = stmt.getColumn(1).getString();
    result.active = static_cast<int>(stmt.getColumn(2));

    return result;
}

std::vector<day_type> active_day_types() {

    std::vector<day_type> result;

    SQLite::Statement stmt(db(), "SELECT id, name, active FROM day_type WHERE active = 1");
    while  (stmt.executeStep()) {
        result.emplace_back();
        result.back().id = stmt.getColumn(0);
        result.back().name = stmt.getColumn(1).getString();
        result.back().active = true;
    }

    return result;
}


day req_day_by_ts(time_t ts) {
    day result;

    SQLite::Statement stmt(db(), "SELECT id, day, comment FROM day WHERE day = :ts");
    stmt.bind(":ts", ts);
    if (!stmt.executeStep()) {
        return { 0, ts, "" };
    }

    result.id = stmt.getColumn(0);
    result.day_timestamp = stmt.getColumn(1);
    result.comment = stmt.getColumn(2).getString();

    return result;
}

day_type req_day_type_by_day_id(int id) {

    day_type result;

    SQLite::Statement stmt(db(), R"(SELECT dt.id, dt.name, dt.active FROM day 
                                       JOIN day_type_day dtd ON day.id = dtd.day_id
                                       JOIN day_type dt ON dtd.day_type_id = dt.id 
                                       WHERE day.id =  :id)");
    stmt.bind(":id", id);
    if (!stmt.executeStep()) {
        return day_type{0, "None"};
    }

    result.id = stmt.getColumn(0);
    result.name = stmt.getColumn(1).getString();
    result.active = static_cast<int>(stmt.getColumn(2));

    return result;
}

std::pair<time_t, time_t> req_prev_next_day(time_t ts) {

    time_t next = 0, prev = 0;

    // no way to figure out this by one query
    SQLite::Statement prev_stmt(db(), "SELECT max(day) FROM day WHERE day < :ts");
    SQLite::Statement next_stmt(db(), "SELECT min(day) FROM day WHERE day > :ts");
    prev_stmt.bind(":ts", ts);
    next_stmt.bind(":ts", ts);

    if (prev_stmt.executeStep()) {
        prev = prev_stmt.getColumn(0);
    }

    if (next_stmt.executeStep()) {
        next = next_stmt.getColumn(0);
    }

    return std::make_pair(prev, next);
}
