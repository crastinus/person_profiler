#include "save.hpp"
#include "db.hpp"
#include "cache.hpp"
#include "measure.hpp"


int save(value const& v) {
    if (v.id == 0) {
        SQLite::Statement value_insert(db(), "INSERT INTO value(value, day_id, estimation_id) VALUES (:value, :day_id, :estimation_id)");
        value_insert.bind(":value", v.val);
        value_insert.bind(":day_id", v.day.id);
        value_insert.bind(":estimation_id", v.estimation.id);
        value_insert.exec();

        SQLite::Statement get_max_int(db(), "SELECT max(id) from value");
        get_max_int.executeStep();
        return get_max_int.getColumn(0);
    }

    cache().del(v);

    // update without ids
    SQLite::Statement value_update(db(), "UPDATE value SET value = :value, estimation_id = :estimation_id WHERE id = :id");
    value_update.bind(":value", v.val);
    value_update.bind(":id", v.id);
    value_update.bind(":estimation_id", v.estimation.id);
    value_update.exec();
    return v.id;
}

int save(day const& d) {

    if (d.id == 0) {
        SQLite::Statement day_insert(db(), "INSERT INTO day(day, comment) VALUES(:date, :comment)");
        day_insert.bind(":date", d.day_timestamp);
        day_insert.bind(":comment", d.comment.c_str());
        day_insert.exec();

        SQLite::Statement get_max_day_id(db(), "SELECT max(id) from day");
        get_max_day_id.executeStep();
        return get_max_day_id.getColumn(0);
    }

    cache().del(d);

    SQLite::Statement day_upd(db(), "UPDATE day SET comment = :comment WHERE id = :id");
    day_upd.bind(":id", d.id);
    day_upd.bind(":comment", d.comment.c_str());
    day_upd.exec();

    // nothing can change
    return d.id;
}

int save(day_type const & d) {

    if (d.id == 0) {
        SQLite::Statement day_insert(db(), "INSERT INTO day_type(name, active) VALUES(:name, :active)");
        day_insert.bind(":name", d.name.c_str());
        day_insert.bind(":active", d.active);
        day_insert.exec();

        SQLite::Statement get_max_day_id(db(), "SELECT max(id) from day_type");
        get_max_day_id.executeStep();
        return get_max_day_id.getColumn(0);
    }

    cache().del(d);

    SQLite::Statement day_upd(db(), "UPDATE day_type SET name = :name, active=:active WHERE id = :id");
    day_upd.bind(":id", d.id);
    day_upd.bind(":name", d.name.c_str());
    day_upd.bind(":active", d.active);
    day_upd.exec();

    // nothing can change
    return d.id;
}

void save(day_type_day const & dtd) {
    SQLite::Statement check(db(), "select count(*) from day_type_day where day_id = :day_id");
    check.bind(":day_id", dtd.day_id.id);
    check.executeStep();

    int count = check.getColumn(0);

    std::string result_query = (count == 0
        ? "INSERT INTO day_type_day(day_id, day_type_id) VALUES(:day_id, :day_type_id)"
        : "UPDATE day_type_day SET day_type_id = :day_type_id WHERE day_id = :day_id");
    
    SQLite::Statement stmt(db(), result_query);
    stmt.bind(":day_id", dtd.day_id.id);
    stmt.bind(":day_type_id", dtd.day_type_id.id);
    stmt.exec();
}

int save(estimation const& e) {

    if (e.id == 0) {
        SQLite::Statement estim_insert(db(), "INSERT INTO estimation(started, expired, active, weight, border, reverse, measure_id, next, day_type_id) VALUES(:started, :expired, :active, :weight, :border, :reverse, :measure_id, :next, :day_type_id)");
        estim_insert.bind(":started", e.started);
        estim_insert.bind(":expired", e.expired);
        estim_insert.bind(":active", (int)e.active);
        estim_insert.bind(":weight", e.weight);
        estim_insert.bind(":border", e.border);
        estim_insert.bind(":reverse", e.reverse);
        estim_insert.bind(":measure_id", e.measure.id);
        estim_insert.bind(":next", e.next.id);
        estim_insert.bind(":day_type_id", e.day_type.id);
        estim_insert.exec();

        SQLite::Statement get_max_id(db(), "SELECT max(id) from estimation");
        get_max_id.executeStep();
        return get_max_id.getColumn(0);
    }

    cache().del(e);

    // cannot update next element. It crushes all sequence
    SQLite::Statement estim_update(db(), "UPDATE estimation SET started = :started, expired = :expired, active = :active, weight = :weight, border = :border, reverse = :reverse WHERE id = :id");
    estim_update.bind(":started", e.started);
    estim_update.bind(":expired", e.expired);
    estim_update.bind(":active", (int)e.active);
    estim_update.bind(":weight", e.weight);
    estim_update.bind(":border", e.border);
    estim_update.bind(":reverse", e.reverse);
    estim_update.bind(":id", e.id);
    estim_update.exec();

    return e.id;
}

int save(measure const& m) {

    if (m.id == 0) {
        SQLite::Statement meas_insrt(db(), "INSERT INTO measure(name, type, measure_group_id,  comment) VALUES(:name, :type, :measure_group_id, :comment)");
        meas_insrt.bind(":name", m.name.c_str());
        meas_insrt.bind(":type", static_cast<int>(m.type));
        meas_insrt.bind(":measure_group_id", m.measure_group.id);
        meas_insrt.bind(":comment", m.comment.c_str());
        meas_insrt.exec();

        SQLite::Statement get_max_id(db(), "SELECT max(id) from measure");
        get_max_id.executeStep();
        return get_max_id.getColumn(0);
    }

    cache().del(m);

    if (!m.active && !have_dependencies(m)) {

        SQLite::Statement delq(db(), "DELETE FROM measure WHERE id = :id");
        SQLite::Statement delqe(db(), "DELETE FROM estimation WHERE measure_id = :id");

        delqe.bind(":id", m.id);
        delq.bind(":id", m.id);

        delqe.exec();
        delq.exec();
        return 0;
    }

    // disabled type update
    SQLite::Statement meas_updt(db(), "UPDATE measure SET name = :name, comment = :comment WHERE id = :id");
    meas_updt.bind(":name", m.name.c_str());
    meas_updt.bind(":comment", m.comment.c_str());
    meas_updt.bind(":id", m.id);
    meas_updt.exec();


    // nothing can change
    return m.id;
}

int save(measure_group const & mg) {

    if (mg.id == 0) {
        SQLite::Statement insert(db(), "INSERT INTO measure_group(name, weight, active) VALUES(:name,:weight, :active)");
        insert.bind(":name", mg.name.c_str());
        insert.bind(":active", mg.active);
        insert.bind(":weight", mg.weight);
        insert.exec();

        SQLite::Statement get_max_id(db(), "SELECT max(id) from measure_group");
        get_max_id.executeStep();
        return get_max_id.getColumn(0);
    }

    cache().del(mg);

    SQLite::Statement update(db(), "UPDATE measure_group SET name = :name, active = :active, weight = :weight WHERE id = :id");
    update.bind(":id", mg.id);
    update.bind(":name", mg.name.c_str());
    update.bind(":weight", mg.weight);
    update.bind(":active", mg.active);
    update.exec();

    // nothing can change
    return mg.id;
}
