#pragma once

#include <SQLiteCpp/SQLiteCpp.h>


void create_db();

SQLite::Database& db();

