#include "db.hpp"
/*
#include <SQLiteCpp/SQLiteCpp.h>
#include <SQLiteCpp/VariadicBind.h>
*/

#include "ddl.hpp"

#include <system/system.hpp>
#include <memory>

std::shared_ptr<SQLite::Database> db_ptr;



void create_db() {

    auto db_path = documents() + "/person_profiler.db";
    if (!file_exists(db_path)) {
        SQLite::Database d(db_path, SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE);
        d.exec(ddl);
    }

    db_ptr = std::make_shared<SQLite::Database>(db_path, SQLite::OPEN_READWRITE);
}

SQLite::Database & db()
{
    return *db_ptr;
}
