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

    result.temp_bool = (result.border != 0);
    result.temp_float = result.border;
    result.temp_weight = result.weight;    

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

    result.temp_bool = (result.border != 0);
    result.temp_float = result.border;
    result.temp_weight = result.weight;

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

    SQLite::Statement stmt(db(), "SELECT id, name, type, measure_group_id, active FROM measure WHERE id = :id");
    stmt.bind(":id", id);
    if (!stmt.executeStep()) {
        throw std::runtime_error("Measure with id " + std::to_string(id) + " doesn't exists");
    }

    result.id = stmt.getColumn(0);
    result.name = stmt.getColumn(1).getString();
    result.type = static_cast<measure_type>((int)stmt.getColumn(2));
    result.measure_group = stmt.getColumn(3);
    result.active = static_cast<int>(stmt.getColumn(4));

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

measure_values_graph req_current_measure_graph(std::string const& whereStmt) {

    char const* query =
        R"(SELECT mg.id  mg_id, mg.name  mg_name, mg.active mg_active,                           -- 3
       m.id  m_id, m.name  m_name, m.type  m_type, m.active m_active,                             -- 4
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
        mg.id = stmt.getColumn(0);
        mg.name = stmt.getColumn(1).getString();
        mg.active = static_cast<int>(stmt.getColumn(2));

        measure m;
        m.id = stmt.getColumn(3);
        m.name = stmt.getColumn(4).getString();
        m.type = static_cast<measure_type>((int)stmt.getColumn(5));
        m.active = static_cast<int>(stmt.getColumn(6));
        m.measure_group = mg.id;

        estimation e;
        e.id = stmt.getColumn(7);
        e.started = stmt.getColumn(8);
        e.expired = stmt.getColumn(9);
        e.weight = stmt.getColumn(10);
        e.border = stmt.getColumn(11);
        e.reverse = (int)stmt.getColumn(12);
        e.measure = m.id;
        e.next = stmt.getColumn(13);

        cache().save(mg);
        cache().save(m);
        cache().save(e);


        value v;
        v.id = stmt.getColumn(14);
        v.val = stmt.getColumn(15);
        v.day = stmt.getColumn(16);
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
    return req_current_measure_graph(concat("WHERE e.active = 1 and e.day_type_id = ", day_type_id));
}

measure_values_graph req_current_measure_graph_for_day(int day_id) {
    return req_current_measure_graph(concat("WHERE v.day_id = ", day_id));
}

measure_graph req_measure_graph() {

    char const* query = 
    R"(SELECT mg.id  mg_id, mg.name  mg_name, mg.active mg_active,                               -- 3
       m.id  m_id, m.name  m_name, m.type  m_type, m.active m_active                             -- 4
       FROM measure m
       JOIN measure_group mg on m.measure_group_id = mg.id;)";


    measure_graph result;

    SQLite::Statement stmt(db(), query);

    while (stmt.executeStep()) {
        measure_group mg;
        mg.id = stmt.getColumn(0);
        mg.name = stmt.getColumn(1).getString();
        mg.active = static_cast<int>(stmt.getColumn(2));

        measure m;
        m.id = stmt.getColumn(3);
        m.name = stmt.getColumn(4).getString();
        m.type = static_cast<measure_type>((int)stmt.getColumn(5));
        m.active = static_cast<int>(stmt.getColumn(6));
        m.measure_group = mg.id;

        cache().save(mg);
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
