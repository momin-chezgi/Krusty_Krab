#pragma once

#ifdef __cplusplus
extern "C" {
#endif

typedef struct sqlite3 sqlite3;
typedef struct sqlite3_stmt sqlite3_stmt;

typedef long long sqlite3_int64;
typedef unsigned long long sqlite3_uint64;

typedef void(*sqlite3_destructor_type)(void*);

#define SQLITE_OK 0
#define SQLITE_ERROR 1
#define SQLITE_BUSY 5
#define SQLITE_MISUSE 21

#define SQLITE_ROW 100
#define SQLITE_DONE 101

#define SQLITE_OPEN_READONLY 0x00000001
#define SQLITE_OPEN_READWRITE 0x00000002
#define SQLITE_OPEN_CREATE 0x00000004

#define SQLITE_TRANSIENT ((void(*)(void*))-1)
#define SQLITE_STATIC ((void(*)(void*))0)

int sqlite3_open(const char *filename, sqlite3 **ppDb);
int sqlite3_open_v2(const char* filename, sqlite3 **ppDb, int flags, const char* zVfs);
int sqlite3_close(sqlite3*);
int sqlite3_exec(sqlite3*, const char*, int (*callback)(void*,int,char**,char**), void*, char**);
const char* sqlite3_errmsg(sqlite3*);
void sqlite3_free(void*);

int sqlite3_prepare_v2(sqlite3*, const char*, int, sqlite3_stmt**, const char**);
int sqlite3_bind_text(sqlite3_stmt*, int, const char*, int, sqlite3_destructor_type);
int sqlite3_bind_double(sqlite3_stmt*, int, double);
int sqlite3_bind_int(sqlite3_stmt*, int, int);
int sqlite3_bind_int64(sqlite3_stmt*, int, sqlite3_int64);
int sqlite3_step(sqlite3_stmt*);
int sqlite3_reset(sqlite3_stmt*);
int sqlite3_clear_bindings(sqlite3_stmt*);
int sqlite3_finalize(sqlite3_stmt*);
const unsigned char* sqlite3_column_text(sqlite3_stmt*, int iCol);
double sqlite3_column_double(sqlite3_stmt*, int iCol);
sqlite3_int64 sqlite3_column_int64(sqlite3_stmt*, int iCol);
int sqlite3_column_int(sqlite3_stmt*, int iCol);

#ifdef __cplusplus
}
#endif
