#include "Database/DatabaseManager.h"

static void printSQLiteError(sqlite3* connection, const string& action)
{
    cerr << "DatabaseManager::" << action << " failed: "
              << sqlite3_errmsg(connection) << std::endl;
}

DatabaseManager::DatabaseManager(const string& dbPath)
{
    this->dbPath = dbPath;
    int rc = sqlite3_open_v2(
        dbPath.c_str(),
        &dbConnection,
        SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE,
        nullptr
    );

    if (rc != SQLITE_OK) {
        printSQLiteError(dbConnection, "open");
        close();
    }
}

DatabaseManager::~DatabaseManager()
{
    close();
}

DatabaseManager::DatabaseManager(DatabaseManager&& other) noexcept
{
    dbConnection = other.dbConnection;
    dbPath = std::move(other.dbPath);
    inTransaction = other.inTransaction;

    other.dbConnection = nullptr;
    other.inTransaction = false;
}

DatabaseManager& DatabaseManager::operator=(DatabaseManager&& other) noexcept
{
    if (this != &other) {
        close();

        dbConnection = other.dbConnection;
        dbPath = std::move(other.dbPath);
        inTransaction = other.inTransaction;

        other.dbConnection = nullptr;
        other.inTransaction = false;
    }
    return *this;
}

bool DatabaseManager::isOpen() const
{
    return dbConnection != nullptr;
}

sqlite3* DatabaseManager::connection() const
{
    return dbConnection;
}

const string& DatabaseManager::path() const
{
    return dbPath;
}

bool DatabaseManager::prepareAndBind(
    sqlite3_stmt** statement,
    const string& sql,
    const vector<string>& params
){
    if (!isOpen()) {
        return false;
    }

    int rc = sqlite3_prepare_v2(dbConnection, sql.c_str(), -1, statement, nullptr);
    if (rc != SQLITE_OK) {
        printSQLiteError(dbConnection, "prepare");
        return false;
    }

    for (size_t i = 0; i < params.size(); ++i) {
        rc = sqlite3_bind_text(
            *statement,
            static_cast<int>(i + 1),
            params[i].c_str(),
            -1,
            SQLITE_TRANSIENT
        );
        if (rc != SQLITE_OK) {
            sqlite3_finalize(*statement);
            *statement = nullptr;
            printSQLiteError(dbConnection, "bind");
            return false;
        }
    }
    return true;
}

bool DatabaseManager::execute(const string& sql)
{
    if (!isOpen()) {
        return false;
    }

    char* errorMessage = nullptr;
    int rc = sqlite3_exec(dbConnection, sql.c_str(), nullptr, nullptr, &errorMessage);
    if (rc != SQLITE_OK) {
        cerr << "DatabaseManager::execute error: ";
        if (errorMessage != nullptr) {
            cerr << errorMessage << std::endl;
            sqlite3_free(errorMessage);
        } else {
            printSQLiteError(dbConnection, "execute");
        }
        return false;
    }
    return true;
}

bool DatabaseManager::execute(
    const string& sql,
    const vector<string>& params
){
    sqlite3_stmt* statement = nullptr;
    if (!prepareAndBind(&statement, sql, params)) {
        return false;
    }

    int rc = sqlite3_step(statement);
    sqlite3_finalize(statement);
    if (rc != SQLITE_DONE && rc != SQLITE_ROW) {
        printSQLiteError(dbConnection, "execute(prepared)");
        return false;
    }
    return true;
}

bool DatabaseManager::beginTransaction()
{
    if (inTransaction) {
        return false;
    }
    if (!execute("BEGIN IMMEDIATE;")) {
        return false;
    }
    inTransaction = true;
    return true;
}

bool DatabaseManager::commitTransaction()
{
    if (!inTransaction) {
        return false;
    }
    if (!execute("COMMIT;")) {
        return false;
    }
    inTransaction = false;
    return true;
}

bool DatabaseManager::rollbackTransaction()
{
    if (!inTransaction) {
        return false;
    }
    if (!execute("ROLLBACK;")) {
        return false;
    }
    inTransaction = false;
    return true;
}

void DatabaseManager::close()
{
    if (dbConnection) {
        if (inTransaction) {
            rollbackTransaction();
        }
        sqlite3_close(dbConnection);
        dbConnection = nullptr;
    }
}

DatabaseManager::Transaction::Transaction(DatabaseManager& manager)
    : db(manager), active(manager.beginTransaction()) {}

DatabaseManager::Transaction::~Transaction()
{
    if (active && !finalized && !committedFlag) {
        rollback();
    }
}

bool DatabaseManager::Transaction::commit()
{
    if (!active || finalized) {
        return false;
    }
    committedFlag = db.commitTransaction();
    if (!committedFlag) {
        return false;
    }
    finalized = true;
    active = false;
    return true;
}

bool DatabaseManager::Transaction::rollback()
{
    if (!active || finalized) {
        return false;
    }
    bool rolledBack = db.rollbackTransaction();
    if (!rolledBack) {
        return false;
    }
    finalized = true;
    active = false;
    committedFlag = false;
    return true;
}

bool DatabaseManager::Transaction::isActive() const
{
    return active;
}

bool DatabaseManager::Transaction::committed() const
{
    return committedFlag;
}
