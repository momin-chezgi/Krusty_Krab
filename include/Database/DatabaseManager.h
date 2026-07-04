#pragma once

#include <iostream>
#include <string>
#include <vector>

#include <sqlite3.h>

using std::string;
using std::cerr;
using std::vector;

class DatabaseManager {
public:
    explicit DatabaseManager(const string& dbPath = "");
    ~DatabaseManager();

    DatabaseManager(const DatabaseManager&) = delete;
    DatabaseManager& operator=(const DatabaseManager&) = delete;
    DatabaseManager(DatabaseManager&& other) noexcept;
    DatabaseManager& operator=(DatabaseManager&& other) noexcept;

    bool isOpen() const;
    sqlite3* connection() const;
    const string& path() const;

    bool execute(const string& sql);
    bool execute(const string& sql, const vector<string>& params);

    bool beginTransaction();
    bool commitTransaction();
    bool rollbackTransaction();

    class Transaction {
    public:
        explicit Transaction(DatabaseManager& manager);
        ~Transaction();

        bool commit();
        bool rollback();

        bool isActive() const;
        bool committed() const;

        Transaction(const Transaction&) = delete;
        Transaction& operator=(const Transaction&) = delete;
    private:
        DatabaseManager& db;
        bool active{false};
        bool finalized{false};
        bool committedFlag{false};
    };

private:
    void bootstrap();
    void close();
    bool prepareAndBind(
        sqlite3_stmt** statement,
        const string& sql,
        const vector<string>& params
    );

    sqlite3* dbConnection{nullptr};
    string dbPath;
    bool inTransaction{false};
};
