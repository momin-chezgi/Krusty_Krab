#include "Repository/RestaurateurStorage.h"

#include "Database/DatabaseManager.h"

namespace {
    const string restaurateurExistsSql =
        "SELECT 1 FROM restaurateurs WHERE id = ? LIMIT 1;";
    const string restaurateurSelectSql =
        "SELECT restaurant_id, name FROM restaurateurs WHERE id = ? LIMIT 1;";
    const string allRestaurateursSql =
        "SELECT id, restaurant_id, name FROM restaurateurs ORDER BY rowid;";
    const string insertRestaurateurSql =
        "INSERT INTO restaurateurs (id, restaurant_id, name) VALUES (?, ?, ?);";
    const string updateRestaurateurSql =
        "UPDATE restaurateurs SET restaurant_id = ?, name = ? WHERE id = ?;";
    const string updateRestaurateurNameSql =
        "UPDATE restaurateurs SET name = ? WHERE id = ?;";
    const string deleteRestaurateurSql = "DELETE FROM restaurateurs WHERE id = ?;";

    void printSQLiteError(sqlite3* connection, const string& action)
    {
        cerr << "RestaurateurStorage::" << action << " failed: "
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

    string sqliteText(const unsigned char* text)
    {
        return text != nullptr
            ? string(static_cast<const char*>(static_cast<const void*>(text)))
            : "";
    }

    map<ManagerID_tp, ManagerID_tp>& loadedObjectAliases()
    {
        static map<ManagerID_tp, ManagerID_tp> aliases;
        return aliases;
    }

    bool restaurateurExists(sqlite3* connection, const ManagerID_tp& restaurateurID)
    {
        sqlite3_stmt* statement = nullptr;
        if (!prepareStatement(connection, restaurateurExistsSql, &statement)) {
            return false;
        }

        bool exists = false;
        if (bindText(connection, statement, 1, restaurateurID)) {
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

    ManagerID_tp storageIDFor(sqlite3* connection, const Restaurateur& restaurateur)
    {
        const ManagerID_tp objectID = restaurateur.getID();
        if (restaurateurExists(connection, objectID)) {
            return objectID;
        }

        auto alias = loadedObjectAliases().find(objectID);
        if (alias != loadedObjectAliases().end()
            && restaurateurExists(connection, alias->second)) {
            return alias->second;
        }

        return "";
    }

    bool loadRestaurateur(
        sqlite3* connection,
        const ManagerID_tp& restaurateurID,
        Restaurateur& restaurateur
    )
    {
        sqlite3_stmt* statement = nullptr;
        if (!prepareStatement(connection, restaurateurSelectSql, &statement)) {
            return false;
        }

        bool found = false;
        if (bindText(connection, statement, 1, restaurateurID)) {
            const int rc = sqlite3_step(statement);
            if (rc == SQLITE_ROW) {
                const unsigned char* restaurantText = sqlite3_column_text(statement, 0);
                const unsigned char* nameText = sqlite3_column_text(statement, 1);

                const RestID_tp restaurantID = sqliteText(restaurantText);
                const string name = sqliteText(nameText);

                restaurateur = Restaurateur(restaurateurID, restaurantID, name);
                found = true;
            } else if (rc != SQLITE_DONE) {
                printSQLiteError(connection, "step");
            }
        }

        sqlite3_finalize(statement);
        return found;
    }

    string selectTextColumn(
        const ManagerID_tp& restaurateurID,
        const string& sql,
        const string& fallback
    )
    {
        DatabaseManager database;
        if (!database.isOpen()) {
            return fallback;
        }

        sqlite3_stmt* statement = nullptr;
        if (!prepareStatement(database.connection(), sql, &statement)) {
            return fallback;
        }

        string value = fallback;
        if (bindText(database.connection(), statement, 1, restaurateurID)) {
            const int rc = sqlite3_step(statement);
            if (rc == SQLITE_ROW) {
                const unsigned char* text = sqlite3_column_text(statement, 0);
                value = sqliteText(text);
            } else if (rc != SQLITE_DONE) {
                printSQLiteError(database.connection(), "step");
            }
        }

        sqlite3_finalize(statement);
        return value;
    }

    void eraseAliasesFor(const ManagerID_tp& restaurateurID)
    {
        auto& aliases = loadedObjectAliases();
        for (auto it = aliases.begin(); it != aliases.end();) {
            if (it->first == restaurateurID || it->second == restaurateurID) {
                it = aliases.erase(it);
            } else {
                ++it;
            }
        }
    }
}

string RestaurateurStorage::getName(ManagerID_tp restaurateurID) const
{
    return selectTextColumn(restaurateurID, "SELECT name FROM restaurateurs WHERE id = ?;", "NotFound");
}

RestID_tp RestaurateurStorage::getRestaurantID(ManagerID_tp restaurateurID) const
{
    return selectTextColumn(
        restaurateurID,
        "SELECT restaurant_id FROM restaurateurs WHERE id = ?;",
        "NotFound"
    );
}

bool RestaurateurStorage::setName(ManagerID_tp restaurateurID, string newName)
{
    DatabaseManager database;
    if (!database.isOpen()) {
        return false;
    }

    if (!restaurateurExists(database.connection(), restaurateurID)) {
        return false;
    }

    if (newName.empty() || newName == " ") {
        return true;
    }

    return database.execute(updateRestaurateurNameSql, {newName, restaurateurID});
}

bool RestaurateurStorage::updateRestaurateur(const Restaurateur& updatingRestaurateur)
{
    DatabaseManager database;
    if (!database.isOpen()) {
        return false;
    }

    const ManagerID_tp restaurateurID = storageIDFor(database.connection(), updatingRestaurateur);
    if (restaurateurID.empty()) {
        return false;
    }

    return database.execute(
        updateRestaurateurSql,
        {
            updatingRestaurateur.getRestaurantID(),
            updatingRestaurateur.getName(),
            restaurateurID
        }
    );
}

bool RestaurateurStorage::isValidRestaurateur(ManagerID_tp restaurateurID)
{
    DatabaseManager database;
    if (!database.isOpen()) {
        return false;
    }

    return restaurateurExists(database.connection(), restaurateurID);
}

Restaurateur RestaurateurStorage::giveRestaurateur(ManagerID_tp restaurateurID)
{
    DatabaseManager database;
    if (!database.isOpen()) {
        return Restaurateur("NotFound");
    }

    Restaurateur restaurateur;
    if (!loadRestaurateur(database.connection(), restaurateurID, restaurateur)) {
        return Restaurateur("NotFound");
    }

    return restaurateur;
}

bool RestaurateurStorage::saveRestaurateur(const Restaurateur& newRestaurateur)
{
    DatabaseManager database;
    if (!database.isOpen()) {
        return false;
    }

    const ManagerID_tp restaurateurID = newRestaurateur.getID();
    if (restaurateurID.empty() || restaurateurExists(database.connection(), restaurateurID)) {
        return false;
    }

    const bool saved = database.execute(
        insertRestaurateurSql,
        {
            restaurateurID,
            newRestaurateur.getRestaurantID(),
            newRestaurateur.getName()
        }
    );

    if (saved) {
        loadedObjectAliases()[newRestaurateur.getID()] = restaurateurID;
    }
    return saved;
}

bool RestaurateurStorage::deleteRestaurateur(ManagerID_tp restaurateurID)
{
    DatabaseManager database;
    if (!database.isOpen()) {
        return false;
    }

    if (!restaurateurExists(database.connection(), restaurateurID)) {
        return false;
    }

    const bool deleted = database.execute(deleteRestaurateurSql, {restaurateurID});
    if (deleted) {
        eraseAliasesFor(restaurateurID);
    }
    return deleted;
}

map<ManagerID_tp, Restaurateur> RestaurateurStorage::giveAllRestaurateurs() const
{
    DatabaseManager database;
    map<ManagerID_tp, Restaurateur> restaurateurs;
    if (!database.isOpen()) {
        return restaurateurs;
    }

    sqlite3_stmt* statement = nullptr;
    if (!prepareStatement(database.connection(), allRestaurateursSql, &statement)) {
        return restaurateurs;
    }

    int rc = SQLITE_OK;
    while ((rc = sqlite3_step(statement)) == SQLITE_ROW) {
        const unsigned char* idText = sqlite3_column_text(statement, 0);
        const unsigned char* restaurantText = sqlite3_column_text(statement, 1);
        const unsigned char* nameText = sqlite3_column_text(statement, 2);

        const ManagerID_tp restaurateurID = sqliteText(idText);
        const RestID_tp restaurantID = sqliteText(restaurantText);
        const string name = sqliteText(nameText);

        Restaurateur restaurateur(restaurateurID, restaurantID, name);
        restaurateurs.emplace(restaurateurID, restaurateur);
    }

    if (rc != SQLITE_DONE) {
        printSQLiteError(database.connection(), "step");
        restaurateurs.clear();
    }

    sqlite3_finalize(statement);
    return restaurateurs;
}
