#ifndef DATABASE_H
#define DATABASE_H

#include <stdio.h>
#include <stdlib.h>
#include "../src/sqlite3.h"

sqlite3* initialise_database_file(const char *db_file);
int add_nonexisting_user(sqlite3* db, char *email, char* password);
int confirm_existing_user(sqlite3* db, char *email);

#endif