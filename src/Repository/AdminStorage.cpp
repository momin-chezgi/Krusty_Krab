#include "Repository/AdminStorage.h"

#include "Database/DatabaseManager.h"

namespace {
    const string adminExistsSql = "SELECT 1 FROM admins WHERE id = ? LIMIT 1;";
    const string adminSelectSql = "SELECT name FROM admins WHERE id = ?;";
    const string allAdminsSql = "SELECT id, name FROM admins ORDER BY rowid;";
    const string restaurateurSelectSql =
        "SELECT restaurateur_id FROM admin_restaurateurs WHERE admin_id = ? ORDER BY rowid;";
    const string insertAdminSql = "INSERT INTO admins (id, name) VALUES (?, ?);";
    const string updateAdminSql = "UPDATE admins SET name = ? WHERE id = ?;";
    const string deleteAdminSql = "DELETE FROM admins WHERE id = ?;";
    const string deleteAdminRestaurateursSql =
        "DELETE FROM admin_restaurateurs WHERE admin_id = ?;";
    const string insertAdminRestaurateurSql =
        "INSERT INTO admin_restaurateurs (admin_id, restaurateur_id) VALUES (?, ?);";

    void printSQLiteError(sqlite3* connection, const string& action)
    {
        cerr << "AdminStorage::" << action << " failed: "
             << sqlite3_errmsg(connection) << std::endl;
    }

    bool prepareStatement(sqlite3* connection, const string& sql, sqlite3_stmt** statement)
    {
        if (connection == nullptr) {
            return false;
        }

        int rc = sqlite3_prepare_v2(connection, sql.c_str(), -1, statement, nullptr);
        if (rc != SQLITE_OK) {
            printSQLiteError(connection, "prepare");
            return false;
        }
        return true;
    }

    bool bindText(sqlite3* connection, sqlite3_stmt* statement, int index, const string& value)
    {
        int rc = sqlite3_bind_text(statement, index, value.c_str(), -1, SQLITE_TRANSIENT);
        if (rc != SQLITE_OK) {
            printSQLiteError(connection, "bind");
            return false;
        }
        return true;
    }

    string sqliteText(const unsigned char* text)
    {
        return text != nullptr
            ? string(static_cast<const char*>(static_cast<const void*>(text)))
            : "";
    }

    bool adminExists(sqlite3* connection, const AdminID_tp& adminID)
    {
        sqlite3_stmt* statement = nullptr;
        if (!prepareStatement(connection, adminExistsSql, &statement)) {
            return false;
        }

        bool exists = false;
        if (bindText(connection, statement, 1, adminID)) {
            int rc = sqlite3_step(statement);
            if (rc == SQLITE_ROW) {
                exists = true;
            } else if (rc != SQLITE_DONE) {
                printSQLiteError(connection, "step");
            }
        }

        sqlite3_finalize(statement);
        return exists;
    }

    bool loadAdminName(sqlite3* connection, const AdminID_tp& adminID, string& name)
    {
        sqlite3_stmt* statement = nullptr;
        if (!prepareStatement(connection, adminSelectSql, &statement)) {
            return false;
        }

        bool found = false;
        if (bindText(connection, statement, 1, adminID)) {
            int rc = sqlite3_step(statement);
            if (rc == SQLITE_ROW) {
                const unsigned char* text = sqlite3_column_text(statement, 0);
                name = sqliteText(text);
                found = true;
            } else if (rc != SQLITE_DONE) {
                printSQLiteError(connection, "step");
            }
        }

        sqlite3_finalize(statement);
        return found;
    }

    bool loadRestaurateurIDs(
        sqlite3* connection,
        const AdminID_tp& adminID,
        vector<ManagerID_tp>& restaurateurIDs
    )
    {
        sqlite3_stmt* statement = nullptr;
        if (!prepareStatement(connection, restaurateurSelectSql, &statement)) {
            return false;
        }

        bool success = false;
        if (bindText(connection, statement, 1, adminID)) {
            int rc = SQLITE_OK;
            while ((rc = sqlite3_step(statement)) == SQLITE_ROW) {
                const unsigned char* text = sqlite3_column_text(statement, 0);
                restaurateurIDs.emplace_back(sqliteText(text));
            }

            if (rc == SQLITE_DONE) {
                success = true;
            } else {
                printSQLiteError(connection, "step");
            }
        }

        sqlite3_finalize(statement);
        return success;
    }

    bool loadAdmin(
        sqlite3* connection,
        const AdminID_tp& adminID,
        string& name,
        vector<ManagerID_tp>& restaurateurIDs
    )
    {
        if (!loadAdminName(connection, adminID, name)) {
            return false;
        }
        restaurateurIDs.clear();
        return loadRestaurateurIDs(connection, adminID, restaurateurIDs);
    }

    bool replaceAdminRestaurateurLinks(DatabaseManager& database, const AdminOfSystem& admin)
    {
        const AdminID_tp adminID = admin.getID();
        if (!database.execute(deleteAdminRestaurateursSql, {adminID})) {
            return false;
        }

        for (const auto& restaurateurID : admin.getRestaurateurIDs()) {
            if (!database.execute(insertAdminRestaurateurSql, {adminID, restaurateurID})) {
                return false;
            }
        }
        return true;
    }
}

AdminOfSystem AdminStorage::giveAdmin(AdminID_tp adminID) const
{
    DatabaseManager database;
    if (!database.isOpen()) {
        return AdminOfSystem({}, "");
    }

    string name;
    vector<ManagerID_tp> restaurateurIDs;
    if (!loadAdmin(database.connection(), adminID, name, restaurateurIDs)) {
        return AdminOfSystem({}, "");
    }

    return AdminOfSystem(adminID, restaurateurIDs, name);
}

bool AdminStorage::isValidAdmin(AdminID_tp adminID) const
{
    DatabaseManager database;
    if (!database.isOpen()) {
        return false;
    }
    return adminExists(database.connection(), adminID);
}

bool AdminStorage::saveAdmin(const AdminOfSystem& newAdmin)
{
    DatabaseManager database;
    if (!database.isOpen()) {
        return false;
    }

    DatabaseManager::Transaction transaction(database);
    if (!transaction.isActive()) {
        return false;
    }

    const AdminID_tp adminID = newAdmin.getID();
    if (!database.execute(insertAdminSql, {adminID, newAdmin.getName()})) {
        return false;
    }

    if (!replaceAdminRestaurateurLinks(database, newAdmin)) {
        return false;
    }

    return transaction.commit();
}

bool AdminStorage::deleteAdmin(AdminID_tp adminID)
{
    DatabaseManager database;
    if (!database.isOpen()) {
        return false;
    }

    if (!adminExists(database.connection(), adminID)) {
        return false;
    }

    DatabaseManager::Transaction transaction(database);
    if (!transaction.isActive()) {
        return false;
    }

    if (!database.execute(deleteAdminRestaurateursSql, {adminID})) {
        return false;
    }

    if (!database.execute(deleteAdminSql, {adminID})) {
        return false;
    }

    return transaction.commit();
}

bool AdminStorage::updateAdmin(AdminID_tp adminID, const AdminOfSystem& updatingAdmin)
{
    DatabaseManager database;
    if (!database.isOpen()) {
        return false;
    }

    if (!adminExists(database.connection(), adminID)) {
        return false;
    }

    DatabaseManager::Transaction transaction(database);
    if (!transaction.isActive()) {
        return false;
    }

    if (!database.execute(updateAdminSql, {updatingAdmin.getName(), adminID})) {
        return false;
    }

    if (!replaceAdminRestaurateurLinks(database, updatingAdmin)) {
        return false;
    }

    return transaction.commit();
}

map<AdminID_tp, AdminOfSystem> AdminStorage::giveAllAdmins() const
{
    DatabaseManager database;
    map<AdminID_tp, AdminOfSystem> admins;
    if (!database.isOpen()) {
        return admins;
    }

    sqlite3_stmt* statement = nullptr;
    if (!prepareStatement(database.connection(), allAdminsSql, &statement)) {
        return admins;
    }

    int rc = SQLITE_OK;
    while ((rc = sqlite3_step(statement)) == SQLITE_ROW) {
        const unsigned char* idText = sqlite3_column_text(statement, 0);
        const unsigned char* nameText = sqlite3_column_text(statement, 1);

        AdminID_tp adminID = sqliteText(idText);
        string name = sqliteText(nameText);
        vector<ManagerID_tp> restaurateurIDs;
        if (!loadRestaurateurIDs(database.connection(), adminID, restaurateurIDs)) {
            admins.clear();
            rc = SQLITE_DONE;
            break;
        }

        admins.emplace(adminID, AdminOfSystem(adminID, restaurateurIDs, name));
    }

    if (rc != SQLITE_DONE) {
        printSQLiteError(database.connection(), "step");
        admins.clear();
    }

    sqlite3_finalize(statement);
    return admins;
}
