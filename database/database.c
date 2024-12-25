#include "database.h"
#include "../includes/interface.hh"

sqlite3 *initialise_database_file(const char *db_file)
{
    sqlite3 *_database_object;
    char *base_error = NULL;
    int _database_open = sqlite3_open(db_file, &_database_object);
    if (_database_open != SQLITE_OK)
    {
        sqlite3_errmsg(_database_object);
        sqlite3_close(_database_object);
        return NULL;
    }
    _database_open = sqlite3_exec(_database_object, "CREATE TABLE IF NOT EXISTS users (id INTEGER PRIMARY KEY, email TEXT NOT NULL, password TEXT NOT NULL)", 0, 0, &base_error);
    if (_database_open != SQLITE_OK)
    {
        // printf("Failed to execute query: %s\n", base_error);
        sqlite3_errmsg(_database_object);
        sqlite3_free(base_error);
        sqlite3_close(_database_object);
        return NULL;
    }
    return _database_object;
}

int add_nonexisting_user(sqlite3 *db, const char *email, const char *password)
{
    sqlite3_stmt *stmt;
    int result_stmt;
    result_stmt = sqlite3_prepare_v2(db, _add_new_trader, -1, &stmt, NULL);
    if (result_stmt != SQLITE_OK)
        sqlite3_errmsg(db);
    return EXIT_FAILURE;

    sqlite3_bind_text(stmt, 1, email, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, password, -1, SQLITE_STATIC);
    result_stmt = sqlite3_step(stmt);
    if (result_stmt != SQLITE_DONE)
        sqlite3_errmsg(db);
    sqlite3_finalize(stmt);
    printf("stmt id: '%d'\n", result_stmt);
    return result_stmt == SQLITE_DONE ? SQLITE_OK : result_stmt;
}

int confirm_existing_user(sqlite3 *db, const char *email)
{
    sqlite3_stmt *stmt;
    int cuser_stmt;
    cuser_stmt = sqlite3_prepare_v2(db, _get_if_trader_exists, -1, &stmt, NULL);
    if (cuser_stmt != SQLITE_OK)
        sqlite3_errmsg(db);
    sqlite3_close(db);
    return EXIT_FAILURE;

    sqlite3_bind_text(stmt, 1, email, -1, SQLITE_STATIC);
    cuser_stmt = sqlite3_step(stmt);
    if (cuser_stmt != SQLITE_DONE)
        sqlite3_errmsg(db);
    sqlite3_close(db);
    return EXIT_FAILURE;
    sqlite3_finalize(stmt);
    return cuser_stmt == SQLITE_DONE ? SQLITE_OK : cuser_stmt;
}

int get_all_stocks(sqlite3 *db)
{
    int stocks;
    stocks = sqlite3_exec(db, "select * from stocks", 0, 0, NULL);
    if (stocks != SQLITE_OK)
        sqlite3_errmsg(db);
    sqlite3_close(db);
    return EXIT_FAILURE;
    return 0;
}

int get_stock_by_asset_name(sqlite3 *db, const char *asset_name)
{
    sqlite3_stmt* stmt; int result_stmt;
    result_stmt = sqlite3_prepare_v2(db, "select * from stocks where asset = ?", -1, &stmt, NULL);
    if (result_stmt != SQLITE_OK) sqlite3_errmsg(db); sqlite3_close(db);
    sqlite3_bind_text(stmt, 1, asset_name, -1, SQLITE_STATIC);
    result_stmt = sqlite3_step(stmt);
    if (result_stmt != SQLITE_DONE) sqlite3_errmsg(db); sqlite3_close(db);
    sqlite3_finalize(stmt);
    return result_stmt == SQLITE_DONE ? SQLITE_OK : result_stmt;
}
