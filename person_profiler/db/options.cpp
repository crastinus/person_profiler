#include "options.hpp"
#include "db.hpp"

void save_option(std::string const& option_name, std::string const& option_value) {

    SQLite::Statement insert(db(), "INSERT OR REPLACE INTO  options(name, value) VALUES (:name, :value)");
    insert.bind(":value", option_value);
    insert.bind(":name", option_name);
    insert.exec();
}

std::string get_option(std::string const& option_name) {
    SQLite::Statement select(db(), "SELECT value FROM options WHERE name = :name");
    select.bind(":name", option_name);
    if (!select.executeStep()) {
        return "";
    }

    return select.getColumn(0).getString();
}