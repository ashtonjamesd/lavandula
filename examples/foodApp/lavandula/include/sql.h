#ifndef sql_h
#define sql_h

#include <stdbool.h>

#define DB_PARAMS(...) ((DbParam[]){ __VA_ARGS__ })

#define PARAM_INT(x)    (DbParam){ DB_PARAM_INT,    .value.i = x }
#define PARAM_TEXT(x)   (DbParam){ DB_PARAM_TEXT,   .value.s = x }
#define PARAM_BOOL(x)   (DbParam){ DB_PARAM_BOOL,   .value.b = x }
#define PARAM_INT64(x) (DbParam){ DB_PARAM_INT64, .value.i64 = x }
#define PARAM_DOUBLE(x) (DbParam){ DB_PARAM_DOUBLE, .value.d = x }
#define PARAM_NULL      (DbParam){ DB_PARAM_NULL,   .value.i = 0 }

typedef enum {
    DB_PARAM_NULL,
    DB_PARAM_INT,
    DB_PARAM_INT64,
    DB_PARAM_DOUBLE,
    DB_PARAM_TEXT,
    DB_PARAM_BOOL,
} DbParamType;

typedef struct {
    DbParamType type;

    union {
        int i;
        long long i64;
        double d;
        const char *s;
        bool b;
    } value;
} DbParam;


typedef void (*RowCallback)(int colCount, char **colNames, char **colValues, void *userData);

typedef struct {
    int colCount;
    char **colNames;
    char **colValues;
} DbRow;

typedef struct {
    int rowCount;
    DbRow *rows;
} DbResult;

typedef enum {
    SQLITE,
} SqlDbType;

typedef struct {
    SqlDbType type;

    // if type is SQLITE, connection is sqlite3*
    void *connection;
} DbContext;

DbContext *createSqlLite3DbContext(char *dbPath);

bool dbExec(DbContext *db, const char *query, const DbParam *params, int paramCount);
DbResult *dbQueryRows(DbContext *db, const char *query, DbParam *params, int paramCount);

#endif