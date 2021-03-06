
#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")


#include "db/db.hpp"
#include <iostream>
#include <db/save.hpp>
#include "db/queries.hpp"
#include <ui/ui.hpp>

void init_static_data() {
    create_db();
}

int main() {
    init_static_data();
    
    run_windows();
    return 0;
}

/*
try {
    init_static_data();

    ////auto m = req_measure(1);
    //auto v = req_value(1);
    ////auto e = req_estimation(1);
    ////auto d = req_day(1);
    //day d = v.day;
    //estimation e = v.estimation;

    measure_group mg;

    mg.active = true;
    mg.name = "Sport";
    mg.id = save(mg);
    
    measure m;
    //m.id = 1;
    m.name = "qwer";
    m.type = measure_type::integer;
    m.measure_group = mg.id;
    m.id = save(m);

    estimation est;
    //est.id = 1;
    est.active = true;
    est.expired = std::numeric_limits<time_t>::max();
    est.started = current_day_start();
    est.measure = m.id;
    est.reverse = false;
    est.weight = 20;
    est.border = 100;
    est.id = save(est);
    
    day d;
    d.day_timestamp = current_day_start();
    d.comment = "Very bad day";
    d.id = save(d);

    value v;
   // v.id = 1;
    v.day = 1;
    v.estimation = 1;
    v.val = 60;
    v.id = save(v);
}
catch (SQLite::Exception& ex) {
    std::cerr << ex.what() << "\n";
}
*/