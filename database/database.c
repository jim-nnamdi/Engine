#include "database.h"
#include "../includes/interface.h"

sqlite3* initialise_database_file(const char *db_file) {
    sqlite3* _database_object;
    int _database_open = sqlite3_open(db_file, &_database_object);
    if (!_database_object) 
        sqlite3_errmsg(&_database_object); 
        sqlite3_close(&_database_object); return NULL;
    return _database_object;

}

int add_nonexisting_user(sqlite3* db, char *email, char* password){
    sqlite3_stmt* stmt; int result_stmt;
    result_stmt = sqlite3_prepare_v2(db, _add_new_trader, -1 , &stmt, NULL);
    if (result_stmt != SQLITE_OK) sqlite3_errmsg(db); return EXIT_FAILURE;

    sqlite3_bind_text(stmt, 1, email, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, password, -1, SQLITE_STATIC);
    result_stmt = sqlite3_step(stmt);
    if (result_stmt != SQLITE_DONE) sqlite3_errmsg(db);
    sqlite3_finalize(stmt);
    return result_stmt == SQLITE_DONE ? SQLITE_OK : result_stmt;
}

int confirm_existing_user(sqlite3* db, char *email) {
    sqlite3_stmt* stmt; int cuser_stmt;
    cuser_stmt = sqlite3_prepare_v2(db, _get_if_trader_exists, -1, &stmt, NULL);
    if (cuser_stmt != SQLITE_OK) sqlite3_errmsg(db); sqlite3_close(db); return EXIT_FAILURE;

    sqlite3_bind_text(stmt, 1, email, -1, SQLITE_STATIC);
    cuser_stmt = sqlite3_step(stmt);
    if (cuser_stmt != SQLITE_DONE) sqlite3_errmsg(db); sqlite3_close(db); return;
    sqlite3_finalize(stmt);
    return cuser_stmt == SQLITE_DONE ? SQLITE_OK : cuser_stmt;
}