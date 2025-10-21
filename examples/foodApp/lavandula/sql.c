#include <sqlite3.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>

#include "include/sql.h"

DbContext *createSqlLite3DbContext(char *dbPath) {
    DbContext *context = malloc(sizeof(DbContext));
    if (!context) {
        fprintf(stderr, "Fatal: out of memory\n");
        exit(EXIT_FAILURE);
    }

    context->type = SQLITE;

    sqlite3 *db;
    int rc = sqlite3_open(dbPath, &db);
    if (rc != SQLITE_OK) {
        printf("Cannot open database: %s\n", sqlite3_errmsg(db));
        return NULL;
    }

    context->connection = db;

    return context;
}

bool dbExec(DbContext *db, const char *query, const DbParam *params, int paramCount) {
    sqlite3_stmt *stmt;

    if (sqlite3_prepare_v2((sqlite3 *)db->connection, query, -1, &stmt, NULL) != SQLITE_OK) {
        fprintf(stderr, "Failed to prepare statement: %s\n", sqlite3_errmsg((sqlite3 *)db->connection));
        return false;
    }
    
    for (int i = 0; i < paramCount; i++) {
        switch (params[i].type) {
            case DB_PARAM_NULL:
                sqlite3_bind_null(stmt, i + 1);
                break;
            case DB_PARAM_INT:
                sqlite3_bind_int(stmt, i + 1, params[i].value.i);
                break;
            case DB_PARAM_INT64:
                sqlite3_bind_int64(stmt, i + 1, params[i].value.i64);
                break;
            case DB_PARAM_DOUBLE:
                sqlite3_bind_double(stmt, i + 1, params[i].value.d);
                break;
            case DB_PARAM_TEXT:
                sqlite3_bind_text(stmt, i + 1, params[i].value.s, -1, SQLITE_TRANSIENT);
                break;
            case DB_PARAM_BOOL:
                sqlite3_bind_int(stmt, i + 1, params[i].value.b ? 1 : 0);
                break;
        }
    }
    
    int rc = sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    
    if (rc != SQLITE_DONE && rc != SQLITE_ROW) {
        fprintf(stderr, "SQL error: %s\n", sqlite3_errmsg((sqlite3 *)db->connection));
        return false;
    }
    
    return true;
}

DbResult *dbQueryRows(DbContext *db, const char *query, DbParam *params, int paramCount) {
    sqlite3_stmt *stmt;

    if (sqlite3_prepare_v2((sqlite3 *)db->connection, query, -1, &stmt, NULL) != SQLITE_OK) {
        fprintf(stderr, "Failed to prepare query: %s\n", sqlite3_errmsg((sqlite3 *)db->connection));
        return NULL;
    }

    for (int i = 0; i < paramCount; i++) {
        switch (params[i].type) {
            case DB_PARAM_NULL:
                sqlite3_bind_null(stmt, i + 1);
                break;
            case DB_PARAM_INT:
                sqlite3_bind_int(stmt, i + 1, params[i].value.i);
                break;
            case DB_PARAM_INT64:
                sqlite3_bind_int64(stmt, i + 1, params[i].value.i64);
                break;
            case DB_PARAM_DOUBLE:
                sqlite3_bind_double(stmt, i + 1, params[i].value.d);
                break;
            case DB_PARAM_TEXT:
                sqlite3_bind_text(stmt, i + 1, params[i].value.s, -1, SQLITE_TRANSIENT);
                break;
            case DB_PARAM_BOOL:
                sqlite3_bind_int(stmt, i + 1, params[i].value.b ? 1 : 0);
                break;
        }
    }

    int colCount = sqlite3_column_count(stmt);
    int capacity = 10;
    int rowCount = 0;
    DbRow *rows = malloc(sizeof(DbRow) * capacity);

    if (!rows) {
        fprintf(stderr, "Fatal: out of memory\n");
        exit(EXIT_FAILURE);
    }

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        if (rowCount >= capacity) {
            capacity *= 2;
            rows = realloc(rows, sizeof(DbRow) * capacity);

            if (!rows) {
                fprintf(stderr, "Fatal: out of memory\n");
                exit(EXIT_FAILURE);
            }
        }

        DbRow *row = &rows[rowCount];
        row->colCount = colCount;
        row->colNames = malloc(sizeof(char*) * colCount);
        row->colValues = malloc(sizeof(char*) * colCount);

        if (!row->colNames || !row->colValues) {
            fprintf(stderr, "Fatal: out of memory\n");
            exit(EXIT_FAILURE);
        }

        for (int i = 0; i < colCount; i++) {
            const char *name = sqlite3_column_name(stmt, i);
            const unsigned char *val = sqlite3_column_text(stmt, i);

            row->colNames[i] = strdup(name);
            row->colValues[i] = val ? strdup((const char *)val) : strdup("NULL");
        }

        rowCount++;
    }

    sqlite3_finalize(stmt);

    DbResult *result = malloc(sizeof(DbResult));
    
    if (!result) {
        fprintf(stderr, "Fatal: out of memory\n");
        exit(EXIT_FAILURE);
    }

    result->rowCount = rowCount;
    result->rows = rows;

    return result;
}

bool dbClose(DbContext *db) {
    if (db->type == SQLITE) {
        sqlite3_close((sqlite3 *)db->connection);
    }

    return true;
}