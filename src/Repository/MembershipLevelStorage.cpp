#include "Repository/MembershipLevelStorage.h"

#include "Database/DatabaseManager.h"

#include <ctime>

namespace {
    const string membershipExistsSql =
        "SELECT 1 FROM membership_levels WHERE customer_id = ? LIMIT 1;";
    const string membershipSelectSql = R"SQL(
        SELECT
            starting_point,
            ending_point,
            current_points,
            multiplier,
            off_percentage,
            delivery_discount,
            lottery_ticket,
            level
        FROM membership_levels
        WHERE customer_id = ?
        LIMIT 1;
    )SQL";
    const string insertMembershipSql = R"SQL(
        INSERT INTO membership_levels (
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
        VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?);
    )SQL";
    const string updateMembershipSql = R"SQL(
        UPDATE membership_levels
        SET
            level = ?,
            starting_point = ?,
            ending_point = ?,
            current_points = ?,
            multiplier = ?,
            off_percentage = ?,
            delivery_discount = ?,
            lottery_ticket = ?
        WHERE customer_id = ?;
    )SQL";
    const string deleteMembershipSql =
        "DELETE FROM membership_levels WHERE customer_id = ?;";
    const string insertMembershipLogSql = R"SQL(
        INSERT INTO membership_level_logs (
            customer_id,
            changed_at,
            level,
            points
        )
        VALUES (?, ?, ?, ?);
    )SQL";
    const string countCustomersByLevelSql =
        "SELECT level, COUNT(*) FROM membership_levels GROUP BY level;";
    const string selectMembershipHistorySql = R"SQL(
        SELECT
            customer_id,
            changed_at,
            level,
            points
        FROM membership_level_logs
        WHERE customer_id = ?
        ORDER BY changed_at, rowid;
    )SQL";

    void printSQLiteError(sqlite3* connection, const string& action)
    {
        cerr << "MembershipLevelStorage::" << action << " failed: "
             << sqlite3_errmsg(connection) << std::endl;
    }

    bool prepareStatement(sqlite3* connection, const string& sql, sqlite3_stmt** statement)
    {
        if (connection == nullptr) {
            return false;
        }

        const int rc = sqlite3_prepare_v2(connection, sql.c_str(), -1, statement, nullptr);
        if (rc != SQLITE_OK) {
            printSQLiteError(connection, "prepare");
            return false;
        }
        return true;
    }

    bool bindText(sqlite3* connection, sqlite3_stmt* statement, int index, const string& value)
    {
        const int rc = sqlite3_bind_text(statement, index, value.c_str(), -1, SQLITE_TRANSIENT);
        if (rc != SQLITE_OK) {
            printSQLiteError(connection, "bind");
            return false;
        }
        return true;
    }

    bool membershipExists(sqlite3* connection, const CustID_tp& custID)
    {
        sqlite3_stmt* statement = nullptr;
        if (!prepareStatement(connection, membershipExistsSql, &statement)) {
            return false;
        }

        bool exists = false;
        if (bindText(connection, statement, 1, custID)) {
            const int rc = sqlite3_step(statement);
            if (rc == SQLITE_ROW) {
                exists = true;
            } else if (rc != SQLITE_DONE) {
                printSQLiteError(connection, "step");
            }
        }

        sqlite3_finalize(statement);
        return exists;
    }

    MembershipLevel defaultMembershipLevel()
    {
        return MembershipLevel(0.0, 100.0, 0.0, 1.0, 0.0, 0.0, 0, Level::Normal);
    }

    string levelToStorage(Level level)
    {
        return std::to_string(static_cast<int>(level));
    }

    Level levelFromStorage(int level)
    {
        if (level < static_cast<int>(Level::Normal)) {
            return Level::Normal;
        }
        if (level > static_cast<int>(Level::VIP)) {
            return Level::VIP;
        }
        return static_cast<Level>(level);
    }

    vector<string> membershipParams(const CustID_tp& custID, const MembershipLevel& membership)
    {
        return {
            custID,
            levelToStorage(membership.getMyLevel()),
            std::to_string(membership.getStartingPoint()),
            std::to_string(membership.getEndingPoint()),
            std::to_string(membership.getMyPoint()),
            std::to_string(membership.getMultiplier()),
            std::to_string(membership.getoffPercentage()),
            std::to_string(membership.getDeliveryDiscount()),
            std::to_string(membership.getLotteryTicket())
        };
    }

    vector<string> membershipUpdateParams(
        const CustID_tp& custID,
        const MembershipLevel& membership
    )
    {
        return {
            levelToStorage(membership.getMyLevel()),
            std::to_string(membership.getStartingPoint()),
            std::to_string(membership.getEndingPoint()),
            std::to_string(membership.getMyPoint()),
            std::to_string(membership.getMultiplier()),
            std::to_string(membership.getoffPercentage()),
            std::to_string(membership.getDeliveryDiscount()),
            std::to_string(membership.getLotteryTicket()),
            custID
        };
    }

    bool insertMembershipLog(
        DatabaseManager& database,
        const CustID_tp& custID,
        const MembershipLevel& membership
    )
    {
        return database.execute(
            insertMembershipLogSql,
            {
                custID,
                std::to_string(static_cast<long long>(std::time(nullptr))),
                levelToStorage(membership.getMyLevel()),
                std::to_string(membership.getMyPoint())
            }
        );
    }
}

MembershipLevel MembershipLevelStorage::getMembershipLevel(CustID_tp custID)
{
    DatabaseManager database;
    if (!database.isOpen()) {
        return defaultMembershipLevel();
    }

    sqlite3_stmt* statement = nullptr;
    if (!prepareStatement(database.connection(), membershipSelectSql, &statement)) {
        return defaultMembershipLevel();
    }

    MembershipLevel membership = defaultMembershipLevel();
    if (bindText(database.connection(), statement, 1, custID)) {
        const int rc = sqlite3_step(statement);
        if (rc == SQLITE_ROW) {
            membership = MembershipLevel(
                sqlite3_column_double(statement, 0),
                sqlite3_column_double(statement, 1),
                sqlite3_column_double(statement, 2),
                sqlite3_column_double(statement, 3),
                sqlite3_column_double(statement, 4),
                sqlite3_column_double(statement, 5),
                static_cast<size_t>(sqlite3_column_int(statement, 6)),
                levelFromStorage(sqlite3_column_int(statement, 7))
            );
        } else if (rc != SQLITE_DONE) {
            printSQLiteError(database.connection(), "step");
        }
    }

    sqlite3_finalize(statement);
    return membership;
}

map<Level, size_t> MembershipLevelStorage::countCustomersByLevel()
{
    DatabaseManager database;
    map<Level, size_t> counts{
        {Level::Normal, 0},
        {Level::Silver, 0},
        {Level::Gold, 0},
        {Level::VIP, 0}
    };

    if (!database.isOpen()) {
        return counts;
    }

    sqlite3_stmt* statement = nullptr;
    if (!prepareStatement(database.connection(), countCustomersByLevelSql, &statement)) {
        return counts;
    }

    int rc = SQLITE_OK;
    while ((rc = sqlite3_step(statement)) == SQLITE_ROW) {
        Level level = levelFromStorage(sqlite3_column_int(statement, 0));
        counts[level] = static_cast<size_t>(sqlite3_column_int64(statement, 1));
    }
    if (rc != SQLITE_DONE) {
        printSQLiteError(database.connection(), "step");
        for (auto& count : counts) {
            count.second = 0;
        }
    }

    sqlite3_finalize(statement);
    return counts;
}

vector<MembershipLevelLogEntry> MembershipLevelStorage::levelHistory(CustID_tp custID)
{
    DatabaseManager database;
    vector<MembershipLevelLogEntry> history;

    if (!database.isOpen()) {
        return history;
    }

    sqlite3_stmt* statement = nullptr;
    if (!prepareStatement(database.connection(), selectMembershipHistorySql, &statement)) {
        return history;
    }

    if (bindText(database.connection(), statement, 1, custID)) {
        int rc = SQLITE_OK;
        while ((rc = sqlite3_step(statement)) == SQLITE_ROW) {
            MembershipLevelLogEntry entry;
            const unsigned char* customerID = sqlite3_column_text(statement, 0);
            entry.customerID = customerID != nullptr ? reinterpret_cast<const char*>(customerID) : "";
            entry.changedAt = static_cast<long long>(sqlite3_column_int64(statement, 1));
            entry.level = levelFromStorage(sqlite3_column_int(statement, 2));
            entry.points = sqlite3_column_double(statement, 3);
            history.push_back(entry);
        }
        if (rc != SQLITE_DONE) {
            printSQLiteError(database.connection(), "step");
            history.clear();
        }
    }

    sqlite3_finalize(statement);
    return history;
}

bool MembershipLevelStorage::hasMembershipLevel(CustID_tp custID)
{
    DatabaseManager database;
    if (!database.isOpen()) {
        return false;
    }

    return membershipExists(database.connection(), custID);
}

bool MembershipLevelStorage::addMembershipLevel(CustID_tp custID, const MembershipLevel& ml)
{
    DatabaseManager database;
    if (!database.isOpen()) {
        return false;
    }

    DatabaseManager::Transaction transaction(database);
    if (!transaction.isActive()) {
        return false;
    }

    if (!database.execute(insertMembershipSql, membershipParams(custID, ml))) {
        return false;
    }

    if (!insertMembershipLog(database, custID, ml)) {
        return false;
    }

    return transaction.commit();
}

bool MembershipLevelStorage::delMembershipLevel(CustID_tp custID)
{
    DatabaseManager database;
    if (!database.isOpen()) {
        return false;
    }

    if (!membershipExists(database.connection(), custID)) {
        return false;
    }

    return database.execute(deleteMembershipSql, {custID});
}

bool MembershipLevelStorage::updateMembershipLevel(CustID_tp custID, const MembershipLevel& ml)
{
    DatabaseManager database;
    if (!database.isOpen()) {
        return false;
    }

    if (!membershipExists(database.connection(), custID)) {
        return false;
    }

    DatabaseManager::Transaction transaction(database);
    if (!transaction.isActive()) {
        return false;
    }

    if (!database.execute(updateMembershipSql, membershipUpdateParams(custID, ml))) {
        return false;
    }

    if (!insertMembershipLog(database, custID, ml)) {
        return false;
    }

    return transaction.commit();
}
