#include "Database/DatabaseManager.h"

namespace {
    const string defaultDbPath = "data/krusty_krab.db";
    const string schemaSql = R"SQL(
        CREATE TABLE IF NOT EXISTS admins (
            id TEXT PRIMARY KEY,
            name TEXT NOT NULL
        );

        CREATE TABLE IF NOT EXISTS admin_restaurateurs (
            admin_id TEXT NOT NULL,
            restaurateur_id TEXT NOT NULL,
            PRIMARY KEY (admin_id, restaurateur_id)
        );

        CREATE TABLE IF NOT EXISTS customers (
            id TEXT PRIMARY KEY,
            name TEXT NOT NULL
        );

        CREATE TABLE IF NOT EXISTS customer_orders (
            customer_id TEXT NOT NULL,
            order_id TEXT NOT NULL,
            PRIMARY KEY (customer_id, order_id)
        );

        CREATE TABLE IF NOT EXISTS membership_levels (
            customer_id TEXT PRIMARY KEY,
            level INTEGER NOT NULL,
            starting_point REAL NOT NULL DEFAULT 0,
            ending_point REAL NOT NULL DEFAULT 0,
            current_points REAL NOT NULL DEFAULT 0,
            multiplier REAL NOT NULL DEFAULT 1,
            off_percentage REAL NOT NULL DEFAULT 0,
            delivery_discount REAL NOT NULL DEFAULT 0,
            lottery_ticket INTEGER NOT NULL DEFAULT 0
        );

        CREATE TABLE IF NOT EXISTS membership_level_logs (
            customer_id TEXT NOT NULL,
            changed_at INTEGER NOT NULL,
            level INTEGER NOT NULL,
            points REAL NOT NULL
        );

        CREATE TABLE IF NOT EXISTS menus (
            id TEXT PRIMARY KEY
        );

        CREATE TABLE IF NOT EXISTS menu_items (
            id TEXT PRIMARY KEY,
            menu_id TEXT NOT NULL,
            item_type TEXT NOT NULL,
            name TEXT NOT NULL,
            bio TEXT NOT NULL DEFAULT '',
            price REAL NOT NULL DEFAULT 0,
            stock_quantity REAL NOT NULL DEFAULT 0,
            preparation_minutes INTEGER NOT NULL DEFAULT 0,
            food_type TEXT NOT NULL DEFAULT ''
        );

        CREATE TABLE IF NOT EXISTS restaurants (
            id TEXT PRIMARY KEY,
            menu_id TEXT NOT NULL,
            name TEXT NOT NULL,
            address TEXT NOT NULL,
            active INTEGER NOT NULL DEFAULT 1,
            preparation_minutes INTEGER NOT NULL DEFAULT 0,
            phone TEXT NOT NULL,
            bio TEXT NOT NULL DEFAULT ''
        );

        CREATE TABLE IF NOT EXISTS restaurant_orders (
            restaurant_id TEXT NOT NULL,
            order_id TEXT NOT NULL,
            PRIMARY KEY (restaurant_id, order_id)
        );

        CREATE TABLE IF NOT EXISTS restaurateurs (
            id TEXT PRIMARY KEY,
            restaurant_id TEXT NOT NULL DEFAULT '',
            name TEXT NOT NULL
        );

        CREATE TABLE IF NOT EXISTS orders (
            id TEXT PRIMARY KEY,
            orderer_id TEXT NOT NULL,
            restaurant_id TEXT NOT NULL DEFAULT '',
            status TEXT NOT NULL DEFAULT 'InPreparation',
            total_price REAL NOT NULL DEFAULT 0
        );

        CREATE TABLE IF NOT EXISTS order_items (
            order_id TEXT NOT NULL,
            item_id TEXT NOT NULL,
            item_type TEXT NOT NULL,
            name TEXT NOT NULL,
            bio TEXT NOT NULL DEFAULT '',
            price REAL NOT NULL DEFAULT 0,
            quantity REAL NOT NULL,
            quantity_snapshot REAL NOT NULL DEFAULT 0,
            preparation_minutes INTEGER NOT NULL DEFAULT 0,
            food_type TEXT NOT NULL DEFAULT '',
            PRIMARY KEY (order_id, item_id)
        );
    )SQL";

    const string seedSql = R"SQL(
        INSERT OR IGNORE INTO admins (id, name)
        VALUES ('TestAdmin', 'TestAdmin');

        INSERT OR IGNORE INTO admin_restaurateurs (admin_id, restaurateur_id)
        VALUES ('TestAdmin', 'TestRestaurateur');

        INSERT OR IGNORE INTO customers (id, name)
        VALUES ('TestCustomer', 'TestCustomer');

        INSERT OR IGNORE INTO membership_levels (
            customer_id,
            level,
            starting_point,
            ending_point,
            current_points,
            multiplier,
            off_percentage,
            delivery_discount,
            lottery_ticket
        )
        VALUES ('TestCustomer', 0, 0.0, 100.0, 0.0, 1.0, 0.0, 0.0, 0);

        INSERT OR IGNORE INTO menus (id)
        VALUES ('TestMenu');

        INSERT OR IGNORE INTO menu_items (
            id,
            menu_id,
            item_type,
            name,
            bio,
            price,
            stock_quantity,
            preparation_minutes,
            food_type
        )
        VALUES
            ('TestDrink', 'TestMenu', 'Drink', 'Krusty Cola', 'Cold drink', 10.0, 20.0, 0, ''),
            ('TestFood', 'TestMenu', 'Food', 'Krabby Patty', 'Classic burger', 25.0, 20.0, 0, '');

        INSERT OR IGNORE INTO restaurants (
            id,
            menu_id,
            name,
            address,
            active,
            preparation_minutes,
            phone,
            bio
        )
        VALUES (
            'TestRestaurant',
            'TestMenu',
            'TestRestaurant',
            'TestCity/TestStreet/TestBuilding',
            1,
            20,
            '1234567890',
            'Seed restaurant'
        );

        INSERT OR IGNORE INTO restaurateurs (id, restaurant_id, name)
        VALUES ('TestRestaurateur', 'TestRestaurant', 'TestRestaurateur');
    )SQL";
}

static void printSQLiteError(sqlite3* connection, const string& action)
{
    cerr << "DatabaseManager::" << action << " failed: "
              << sqlite3_errmsg(connection) << std::endl;
}

DatabaseManager::DatabaseManager(const string& dbPath)
{
    const char* environmentPath = std::getenv("KRUSTY_KRAB_DB_PATH");
    if (!dbPath.empty()) {
        this->dbPath = dbPath;
    } else if (environmentPath != nullptr && environmentPath[0] != '\0') {
        this->dbPath = environmentPath;
    } else {
        this->dbPath = defaultDbPath;
    }

    int rc = sqlite3_open_v2(
        this->dbPath.c_str(),
        &dbConnection,
        SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE,
        nullptr
    );

    if (rc != SQLITE_OK) {
        printSQLiteError(dbConnection, "open");
        close();
        return;
    }

    execute("PRAGMA foreign_keys = ON;");
    bootstrap();
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

void DatabaseManager::bootstrap()
{
    if (!isOpen()) {
        return;
    }

    Transaction transaction(*this);
    if (!transaction.isActive()) {
        return;
    }

    if (!execute(schemaSql)) {
        return;
    }

    if (!execute(seedSql)) {
        return;
    }

    transaction.commit();
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
