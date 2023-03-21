#include <sqlite3.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char **argv)
{
    sqlite3 *db;
    char *err_msg = 0;
    int rc = sqlite3_open("database.db", &db);
    if (rc != SQLITE_OK)
    {
        fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return 1;
    }

    char *sql = "CREATE TABLE logs (sender TEXT,body TEXT);";
    rc = sqlite3_exec(db, sql, 0, 0, &err_msg);
    if (rc != SQLITE_OK)
    {
        fprintf(stderr, "Failed to create table: %s\n", sqlite3_errmsg(db));
        sqlite3_free(err_msg);
        sqlite3_close(db);
        return 1;
    }

    clock_t start_time = clock();
    for (int i = 0; i < 1000; i++)
    {

        char *insert_sql = "INSERT INTO logs VALUES (?, ?);";
        sqlite3_stmt *stmt;
        rc = sqlite3_prepare_v2(db, insert_sql, -1, &stmt, 0);
        if (rc != SQLITE_OK)
        {
            fprintf(stderr, "Failed to prepare insert statement: %s\n",
                    sqlite3_errmsg(db));
            sqlite3_close(db);
            return 1;
        }

        sqlite3_bind_text(stmt, 1, "sender", -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 2, "body", -1, SQLITE_STATIC);

        rc = sqlite3_step(stmt);
        if (rc != SQLITE_DONE)
        {
            fprintf(stderr, "Failed to execute insert statement: %s\n",
                    sqlite3_errmsg(db));
        }
        sqlite3_finalize(stmt);
    }
    clock_t end_time = clock();
    double time_spent = (double)(end_time - start_time) / CLOCKS_PER_SEC;
    printf("Time spent for insert: %f seconds\n", time_spent);

    sqlite3_close(db);
    return 0;
}
