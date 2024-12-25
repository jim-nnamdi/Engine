#ifndef DATABASE_H
#define DATABASE_H

#include <stdio.h>
#include <stdlib.h>
#include "../src/sqlite3.h"

sqlite3 *initialise_database_file(const char *db_file);
int add_nonexisting_user(sqlite3 *db, const char *email, const char *password);
int confirm_existing_user(sqlite3 *db, const char *email);

int get_all_stocks(sqlite3 *db);
int get_stock_by_asset_name(sqlite3 *db, const char *asset_name);

#endif