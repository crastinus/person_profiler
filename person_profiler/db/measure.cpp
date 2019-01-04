#include "queries.hpp"
#include "db.hpp"
#include "cache.hpp"

#include <model/day.hpp>
#include <model/estimation.hpp>
#include <model/measure.hpp>
#include <model/value.hpp>
#include "measure.hpp"
#include <model/measure_graph.hpp>
#include <common/concat.hpp>


estimation req_estimation(int id) {

    estimation result;

    SQLite::Statement stmt(db(), "SELECT id, started, expired, active, weight, border, reverse, measure_id, next, day_type_id FROM estimation WHERE id = :id");
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
    result.day_type = (int)stmt.getColumn(9);

    result.temp_float = static_cast<float>(result.border);
    result.temp_weight = static_cast<float>(result.weight);

    return result;

}

estimation req_find_estimation(int measure_id, int day_type_id) {
    estimation result{};

    SQLite::Statement stmt(db(), "SELECT id, started, expired, active, weight, border, reverse, measure_id, next, day_type_id "
                                 "FROM estimation "
                                 "WHERE day_type_id = :day_type_id and measure_id = :measure_id and active != 0");

    stmt.bind(":day_type_id", day_type_id);
    stmt.bind(":measure_id", measure_id);
    if (!stmt.executeStep()) {
        result.day_type = day_type_id;
        result.measure = measure_id;
        return result;
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
    result.day_type = (int)stmt.getColumn(9);

    result.temp_float = static_cast<float>(result.border);
    result.temp_weight = static_cast<float>(result.weight);

    return result;
}

bool have_dependencies(estimation const& e) {
    if (e.id == 0) {
        return false;
    }

    SQLite::Statement check(db(),
        R"(select count(*) from  estimation e 
            join value v on v.estimation_id = e.id
            where e.id = :estimation_id)");
    check.bind(":estimation_id", e.id);
    if (!check.executeStep()) {
        throw std::runtime_error("Wrong query for estimation dependencies");
    }

    return static_cast<int>(check.getColumn(0)) > 0;
}
bool have_dependencies(measure const& m) {
    if (m.id == 0) {
        return false;
    }

    SQLite::Statement check(db(),
        R"(select count(*) from measure m
            join estimation e on e.measure_id = m.id
            join value v on v.estimation_id = e.id
            where m.id = :measure_id)");
    check.bind(":measure_id", m.id);
    if (!check.executeStep()) {
        throw std::runtime_error("Wrong query for measure dependencies");
    }

    return static_cast<int>(check.getColumn(0)) > 0;
}

std::unordered_set<int> disabled_measure_groups(int day_id) {
    std::unordered_set<int> result;
    if (day_id == 0) {
        return result;
    }

    SQLite::Statement stmt(db(), "SELECT measure_group_id FROM forbidden_measure_group WHERE day_id = :day_id");
    stmt.bind(":day_id", day_id);
    
    while (stmt.executeStep()) {
        result.insert(stmt.getColumn(0));
    }

    return result;
}

void disable_measure_group_for_day(int day_id, int measure_group_id) {
    SQLite::Statement stmt(db(), "INSERT INTO forbidden_measure_group(day_id, measure_group_id) VALUES(:day_id, :measure_group_id)");
    stmt.bind(":day_id", day_id);
    stmt.bind(":measure_group_id", measure_group_id);

    stmt.executeStep();
}

void enable_measure_group_for_day(int day_id, int measure_group_id) {
    SQLite::Statement stmt(db(), "DELETE FROM  forbidden_measure_group WHERE day_id = :day_id AND measure_group_id = :measure_group_id");
    stmt.bind(":day_id", day_id);
    stmt.bind(":measure_group_id", measure_group_id);

    stmt.executeStep();
}

measure req_measure(int id) {
    measure result;

    SQLite::Statement stmt(db(), "SELECT id, name, type, measure_group_id, active, comment FROM measure WHERE id = :id");
    stmt.bind(":id", id);
    if (!stmt.executeStep()) {
        throw std::runtime_error("Measure with id " + std::to_string(id) + " doesn't exists");
    }

    result.id = stmt.getColumn(0);
    result.name = stmt.getColumn(1).getString();
    result.type = static_cast<measure_type>((int)stmt.getColumn(2));
    result.measure_group = stmt.getColumn(3);
    result.active = static_cast<int>(stmt.getColumn(4));
    result.comment = stmt.getColumn(5).getString();

    return result;
}

measure_group req_measure_group(int id) {
    measure_group result;

    SQLite::Statement stmt(db(), "SELECT id, name, active, weight  FROM measure_group WHERE id = :id");
    stmt.bind(":id", id);
    if (!stmt.executeStep()) {
        throw std::runtime_error("Measure with id " + std::to_string(id) + " doesn't exists");
    }

    result.id = stmt.getColumn(0);
    result.name = stmt.getColumn(1).getString();
    result.active = (int)stmt.getColumn(2);
    result.weight = stmt.getColumn(3);

    return result;
}

static measure_values_graph req_measure_graph(std::string const& whereStmt) {

    char const* query =
        R"(SELECT mg.id  mg_id, mg.name  mg_name, mg.active mg_active, mg.weight mg_weight,       -- 4
       m.id  m_id, m.name  m_name, m.type  m_type, m.active m_active, m.comment m_comment,        -- 6
	   e.id  e_id, e.started e_st, e.expired e_exp, e.weight e_weight, e.border e_bord,
	   e.reverse e_rev, e.next e_nxt,                                                            -- 7
	   v.id v_id, v.value v_val, v.day_id v_d_id                                                 -- 3 
	   from estimation e
   JOIN measure m on e.measure_id = m.id
   JOIN measure_group mg on m.measure_group_id = mg.id
   LEFT JOIN value v on v.estimation_id = e.id)";

    measure_values_graph result;

    SQLite::Statement stmt(db(), concat(query, " ", whereStmt, ";"));

    while (stmt.executeStep()) {
        measure_group mg;

        int col_id = 0;

        mg.id = stmt.getColumn(col_id++);
        mg.name = stmt.getColumn(col_id++).getString();
        mg.active = static_cast<int>(stmt.getColumn(col_id++));
        mg.weight = stmt.getColumn(col_id++);

        measure m;
        m.id = stmt.getColumn(col_id++);
        m.name = stmt.getColumn(col_id++).getString();
        m.type = static_cast<measure_type>((int)stmt.getColumn(col_id++));
        m.active = static_cast<int>(stmt.getColumn(col_id++));
        m.comment = stmt.getColumn(col_id++).getString();
        m.measure_group = mg.id;

        estimation e;
        e.id = stmt.getColumn(col_id++);
        e.started = stmt.getColumn(col_id++);
        e.expired = stmt.getColumn(col_id++);
        e.weight = stmt.getColumn(col_id++);
        e.border = stmt.getColumn(col_id++);
        e.reverse = (int)stmt.getColumn(col_id++);
        e.measure = m.id;
        e.next = stmt.getColumn(col_id++);

        cache().save(mg);
        cache().save(m);
        cache().save(e);


        value v;
        v.id = stmt.getColumn(col_id++);
        v.val = stmt.getColumn(col_id++);
        v.day = stmt.getColumn(col_id++);
        v.estimation = e.id;

        // real time rendering value
        value_info vi;
        vi.value_name_ = m.name;
        vi.value_ = v;
        vi.type_ = m.type;
        vi.bool_value_ = static_cast<bool>(v.val);
        vi.float_value_ = static_cast<float>(v.val);

        result[m.measure_group].push_back(vi);
    }
    return result;
}

measure_values_graph req_current_measure_graph(int day_type_id) {
    return req_measure_graph(concat("AND v.day_id = 0 WHERE e.active = 1 and e.day_type_id = ", day_type_id));
}

measure_values_graph req_current_measure_graph_for_day(int day_id) {
    return req_measure_graph(concat("WHERE v.day_id = ", day_id));
}

measure_values_graph req_measure_graph_for(time_t first_day, time_t last_day) {
    std::string query_tail = concat("JOIN day d ON d.id = v.day_id ",
                                    "WHERE d.day BETWEEN ", first_day, " AND ", last_day);
    return req_measure_graph(query_tail);
}

measure_graph req_measure_graph() {

    char const* query = 
    R"(SELECT mg.id  mg_id, mg.name  mg_name, mg.active mg_active,                               -- 3
       m.id  m_id, m.name  m_name, m.type  m_type, m.active m_active, m.comment m_comm           -- 5
       FROM measure_group mg 
       LEFT JOIN measure m on m.measure_group_id = mg.id
       WHERE mg.active = 1)";


    measure_graph result;

    SQLite::Statement stmt(db(), query);

    while (stmt.executeStep()) {
        measure_group mg;
        mg.id = stmt.getColumn(0);
        mg.name = stmt.getColumn(1).getString();
        mg.active = static_cast<int>(stmt.getColumn(2));

        cache().save(mg);

        int m_id = stmt.getColumn(3);
        if (m_id == 0) {
            result[{mg.id}] = {};
            continue;
        }

        measure m;
        m.id = m_id;
        m.name = stmt.getColumn(4).getString();
        m.type = static_cast<measure_type>((int)stmt.getColumn(5));
        m.active = static_cast<int>(stmt.getColumn(6));
        m.comment = stmt.getColumn(7).getString();
        m.measure_group = mg.id;


        cache().save(m);
        
        result[m.measure_group].push_back(m);
    }

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
