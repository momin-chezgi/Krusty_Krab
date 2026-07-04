#include "Repository/RestaurantStorage.h"

#include "Database/DatabaseManager.h"

namespace {
    const string restaurantExistsSql = "SELECT 1 FROM restaurants WHERE id = ? LIMIT 1;";
    const string restaurantSelectSql = R"SQL(
        SELECT menu_id, name, address, active, preparation_minutes, phone, bio
        FROM restaurants
        WHERE id = ?
        LIMIT 1;
    )SQL";
    const string restaurantOrdersSql =
        "SELECT order_id FROM restaurant_orders WHERE restaurant_id = ? ORDER BY rowid;";
    const string allRestaurantsSql = "SELECT id FROM restaurants ORDER BY rowid;";
    const string activeRestaurantsSql =
        "SELECT id FROM restaurants WHERE active = 1 ORDER BY rowid;";
    const string insertRestaurantSql = R"SQL(
        INSERT INTO restaurants (
            id,
            menu_id,
            name,
            address,
            active,
            preparation_minutes,
            phone,
            bio
        )
        VALUES (?, ?, ?, ?, ?, ?, ?, ?);
    )SQL";
    const string updateRestaurantSql = R"SQL(
        UPDATE restaurants
        SET menu_id = ?,
            name = ?,
            address = ?,
            active = ?,
            preparation_minutes = ?,
            phone = ?,
            bio = ?
        WHERE id = ?;
    )SQL";
    const string deleteRestaurantSql = "DELETE FROM restaurants WHERE id = ?;";
    const string deleteRestaurantOrdersSql =
        "DELETE FROM restaurant_orders WHERE restaurant_id = ?;";
    const string insertRestaurantOrderSql =
        "INSERT INTO restaurant_orders (restaurant_id, order_id) VALUES (?, ?);";
    const string restaurantOrderExistsSql = R"SQL(
        SELECT 1
        FROM restaurant_orders
        WHERE restaurant_id = ? AND order_id = ?
        LIMIT 1;
    )SQL";
    const string deleteRestaurantOrderSql =
        "DELETE FROM restaurant_orders WHERE restaurant_id = ? AND order_id = ?;";

    void printSQLiteError(sqlite3* connection, const string& action)
    {
        cerr << "RestaurantStorage::" << action << " failed: "
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

    bool restaurantExists(sqlite3* connection, const RestID_tp& restaurantID)
    {
        sqlite3_stmt* statement = nullptr;
        if (!prepareStatement(connection, restaurantExistsSql, &statement)) {
            return false;
        }

        bool exists = false;
        if (bindText(connection, statement, 1, restaurantID)) {
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

    bool restaurantOrderExists(
        sqlite3* connection,
        const RestID_tp& restaurantID,
        const OrderID_tp& orderID
    )
    {
        sqlite3_stmt* statement = nullptr;
        if (!prepareStatement(connection, restaurantOrderExistsSql, &statement)) {
            return false;
        }

        bool exists = false;
        if (bindText(connection, statement, 1, restaurantID)
            && bindText(connection, statement, 2, orderID)) {
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

    bool loadRestaurantOrders(
        sqlite3* connection,
        const RestID_tp& restaurantID,
        vector<OrderID_tp>& orderIDs
    )
    {
        sqlite3_stmt* statement = nullptr;
        if (!prepareStatement(connection, restaurantOrdersSql, &statement)) {
            return false;
        }

        bool success = false;
        if (bindText(connection, statement, 1, restaurantID)) {
            int rc = SQLITE_OK;
            while ((rc = sqlite3_step(statement)) == SQLITE_ROW) {
                const unsigned char* text = sqlite3_column_text(statement, 0);
                orderIDs.emplace_back(sqliteText(text));
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

    bool loadRestaurant(sqlite3* connection, const RestID_tp& restaurantID, Restaurant& restaurant)
    {
        sqlite3_stmt* statement = nullptr;
        if (!prepareStatement(connection, restaurantSelectSql, &statement)) {
            return false;
        }

        bool found = false;
        if (bindText(connection, statement, 1, restaurantID)) {
            const int rc = sqlite3_step(statement);
            if (rc == SQLITE_ROW) {
                const unsigned char* menuText = sqlite3_column_text(statement, 0);
                const unsigned char* nameText = sqlite3_column_text(statement, 1);
                const unsigned char* addressText = sqlite3_column_text(statement, 2);
                const int active = sqlite3_column_int(statement, 3);
                const int preparationMinutes = sqlite3_column_int(statement, 4);
                const unsigned char* phoneText = sqlite3_column_text(statement, 5);
                const unsigned char* bioText = sqlite3_column_text(statement, 6);

                const MenuID_tp menuID = sqliteText(menuText);
                const string name = sqliteText(nameText);
                const string address = sqliteText(addressText);
                const string phone = sqliteText(phoneText);
                const string bio = sqliteText(bioText);

                vector<OrderID_tp> orderIDs;
                if (loadRestaurantOrders(connection, restaurantID, orderIDs)) {
                    Restaurant loaded(
                        restaurantID,
                        menuID,
                        name,
                        address,
                        phone,
                        bio,
                        static_cast<size_t>(preparationMinutes),
                        orderIDs
                    );
                    if (active) {
                        loaded.activate();
                    } else {
                        loaded.deactivate();
                    }
                    restaurant = loaded;
                    found = true;
                }
            } else if (rc != SQLITE_DONE) {
                printSQLiteError(connection, "step");
            }
        }

        sqlite3_finalize(statement);
        return found;
    }

    bool replaceRestaurantOrders(
        DatabaseManager& database,
        const RestID_tp& restaurantID,
        const Restaurant& restaurant
    )
    {
        if (!database.execute(deleteRestaurantOrdersSql, {restaurantID})) {
            return false;
        }

        for (const auto& orderID : restaurant.getOrderHistoryIDs()) {
            if (!database.execute(insertRestaurantOrderSql, {restaurantID, orderID})) {
                return false;
            }
        }

        return true;
    }

    string selectTextColumn(
        const RestID_tp& restaurantID,
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
        if (bindText(database.connection(), statement, 1, restaurantID)) {
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

    bool updateRestaurantColumn(
        const RestID_tp& restaurantID,
        const string& sql,
        const string& value
    )
    {
        DatabaseManager database;
        if (!database.isOpen()) {
            return false;
        }

        if (!restaurantExists(database.connection(), restaurantID)) {
            return false;
        }

        return database.execute(sql, {value, restaurantID});
    }
}

string RestaurantStorage::getName(RestID_tp restaurantID) const
{
    return selectTextColumn(restaurantID, "SELECT name FROM restaurants WHERE id = ?;", "NotFound");
}

string RestaurantStorage::getAddress(RestID_tp restaurantID) const
{
    return selectTextColumn(restaurantID, "SELECT address FROM restaurants WHERE id = ?;", "NotFound");
}

bool RestaurantStorage::getStatus(RestID_tp restaurantID) const
{
    DatabaseManager database;
    if (!database.isOpen()) {
        return false;
    }

    sqlite3_stmt* statement = nullptr;
    if (!prepareStatement(database.connection(), "SELECT active FROM restaurants WHERE id = ?;", &statement)) {
        return false;
    }

    bool active = false;
    if (bindText(database.connection(), statement, 1, restaurantID)) {
        const int rc = sqlite3_step(statement);
        if (rc == SQLITE_ROW) {
            active = sqlite3_column_int(statement, 0) != 0;
        } else if (rc != SQLITE_DONE) {
            printSQLiteError(database.connection(), "step");
        }
    }

    sqlite3_finalize(statement);
    return active;
}

size_t RestaurantStorage::getPreparationMinutes(RestID_tp restaurantID) const
{
    DatabaseManager database;
    if (!database.isOpen()) {
        return 0;
    }

    sqlite3_stmt* statement = nullptr;
    if (!prepareStatement(database.connection(), "SELECT preparation_minutes FROM restaurants WHERE id = ?;", &statement)) {
        return 0;
    }

    size_t minutes = 0;
    if (bindText(database.connection(), statement, 1, restaurantID)) {
        const int rc = sqlite3_step(statement);
        if (rc == SQLITE_ROW) {
            minutes = static_cast<size_t>(sqlite3_column_int(statement, 0));
        } else if (rc != SQLITE_DONE) {
            printSQLiteError(database.connection(), "step");
        }
    }

    sqlite3_finalize(statement);
    return minutes;
}

string RestaurantStorage::getPhoneNumber(RestID_tp restaurantID) const
{
    return selectTextColumn(restaurantID, "SELECT phone FROM restaurants WHERE id = ?;", "NotFound");
}

string RestaurantStorage::getBio(RestID_tp restaurantID) const
{
    return selectTextColumn(restaurantID, "SELECT bio FROM restaurants WHERE id = ?;", "NotFound");
}

MenuID_tp RestaurantStorage::getMenuID(RestID_tp restaurantID) const
{
    return selectTextColumn(restaurantID, "SELECT menu_id FROM restaurants WHERE id = ?;", "NotFound");
}

bool RestaurantStorage::isValidRestaurant(RestID_tp restaurantID)
{
    DatabaseManager database;
    if (!database.isOpen()) {
        return false;
    }

    return restaurantExists(database.connection(), restaurantID);
}

bool RestaurantStorage::addOrderToRestaurant(RestID_tp restaurantID, OrderID_tp orderID)
{
    DatabaseManager database;
    if (!database.isOpen()) {
        return false;
    }

    OrderStorage orderStorage;
    if (!restaurantExists(database.connection(), restaurantID)
        || !orderStorage.isValidOrder(orderID)
        || restaurantOrderExists(database.connection(), restaurantID, orderID)) {
        return false;
    }

    return database.execute(insertRestaurantOrderSql, {restaurantID, orderID});
}

bool RestaurantStorage::orderIsInQueue(RestID_tp restaurantID, OrderID_tp orderID)
{
    DatabaseManager database;
    if (!database.isOpen()) {
        return false;
    }

    return restaurantOrderExists(database.connection(), restaurantID, orderID);
}

Restaurant RestaurantStorage::giveRestaurant(RestID_tp restaurantID)
{
    DatabaseManager database;
    if (!database.isOpen()) {
        return Restaurant();
    }

    Restaurant restaurant;
    if (!loadRestaurant(database.connection(), restaurantID, restaurant)) {
        return Restaurant();
    }

    return restaurant;
}

bool RestaurantStorage::deleteOrderFromRestaurant(RestID_tp restaurantID, OrderID_tp orderID)
{
    DatabaseManager database;
    if (!database.isOpen()) {
        return false;
    }

    if (!restaurantOrderExists(database.connection(), restaurantID, orderID)) {
        return false;
    }

    return database.execute(deleteRestaurantOrderSql, {restaurantID, orderID});
}

RestID_tp RestaurantStorage::saveRestaurant(const Restaurant& newRestaurant)
{
    DatabaseManager database;
    if (!database.isOpen()) {
        return "";
    }

    const RestID_tp newID = newRestaurant.getID();
    if (newID.empty() || restaurantExists(database.connection(), newID)) {
        return "";
    }

    DatabaseManager::Transaction transaction(database);
    if (!transaction.isActive()) {
        return "";
    }

    if (!database.execute(
        insertRestaurantSql,
        {
            newID,
            newRestaurant.getMenuID(),
            newRestaurant.getName(),
            newRestaurant.getAddress(),
            newRestaurant.isActive() ? "1" : "0",
            std::to_string(newRestaurant.getMinutesFoodPrepare()),
            newRestaurant.getPhone(),
            newRestaurant.getBio()
        }
    )) {
        return "";
    }

    if (!replaceRestaurantOrders(database, newID, newRestaurant)) {
        return "";
    }

    return transaction.commit() ? newID : "";
}

bool RestaurantStorage::deleteRestaurant(RestID_tp restaurantID)
{
    DatabaseManager database;
    if (!database.isOpen()) {
        return false;
    }

    if (!restaurantExists(database.connection(), restaurantID)) {
        return false;
    }

    DatabaseManager::Transaction transaction(database);
    if (!transaction.isActive()) {
        return false;
    }

    if (!database.execute(deleteRestaurantOrdersSql, {restaurantID})) {
        return false;
    }

    if (!database.execute(deleteRestaurantSql, {restaurantID})) {
        return false;
    }

    return transaction.commit();
}

bool RestaurantStorage::replaceRestaurant(
    RestID_tp prerestaurantID,
    const Restaurant& updatingRestaurant
)
{
    DatabaseManager database;
    if (!database.isOpen()) {
        return false;
    }

    if (!restaurantExists(database.connection(), prerestaurantID)) {
        return false;
    }

    DatabaseManager::Transaction transaction(database);
    if (!transaction.isActive()) {
        return false;
    }

    if (!database.execute(
        updateRestaurantSql,
        {
            updatingRestaurant.getMenuID(),
            updatingRestaurant.getName(),
            updatingRestaurant.getAddress(),
            updatingRestaurant.isActive() ? "1" : "0",
            std::to_string(updatingRestaurant.getMinutesFoodPrepare()),
            updatingRestaurant.getPhone(),
            updatingRestaurant.getBio(),
            prerestaurantID
        }
    )) {
        return false;
    }

    if (!replaceRestaurantOrders(database, prerestaurantID, updatingRestaurant)) {
        return false;
    }

    return transaction.commit();
}

bool RestaurantStorage::setName(RestID_tp restaurantID, string newName)
{
    return updateRestaurantColumn(
        restaurantID,
        "UPDATE restaurants SET name = ? WHERE id = ?;",
        newName
    );
}

bool RestaurantStorage::setAddress(RestID_tp restaurantID, string newAddress)
{
    return updateRestaurantColumn(
        restaurantID,
        "UPDATE restaurants SET address = ? WHERE id = ?;",
        newAddress
    );
}

bool RestaurantStorage::activateRestaurant(RestID_tp restaurantID)
{
    return updateRestaurantColumn(
        restaurantID,
        "UPDATE restaurants SET active = ? WHERE id = ?;",
        "1"
    );
}

bool RestaurantStorage::deactivateRestaurant(RestID_tp restaurantID)
{
    return updateRestaurantColumn(
        restaurantID,
        "UPDATE restaurants SET active = ? WHERE id = ?;",
        "0"
    );
}

bool RestaurantStorage::setPreparationTime(RestID_tp restaurantID, const size_t minutes)
{
    return updateRestaurantColumn(
        restaurantID,
        "UPDATE restaurants SET preparation_minutes = ? WHERE id = ?;",
        std::to_string(minutes)
    );
}

bool RestaurantStorage::setPhoneNumber(RestID_tp restaurantID, const string newPhoneNumber)
{
    return updateRestaurantColumn(
        restaurantID,
        "UPDATE restaurants SET phone = ? WHERE id = ?;",
        newPhoneNumber
    );
}

bool RestaurantStorage::setBio(RestID_tp restaurantID, const string newBio)
{
    return updateRestaurantColumn(
        restaurantID,
        "UPDATE restaurants SET bio = ? WHERE id = ?;",
        newBio
    );
}

vector<OrderID_tp> RestaurantStorage::getOrderHistoryIDs(RestID_tp restaurantID) const
{
    DatabaseManager database;
    vector<OrderID_tp> orderIDs;
    if (!database.isOpen()) {
        return orderIDs;
    }

    if (!restaurantExists(database.connection(), restaurantID)) {
        return orderIDs;
    }

    if (!loadRestaurantOrders(database.connection(), restaurantID, orderIDs)) {
        orderIDs.clear();
    }

    return orderIDs;
}

vector<OrderID_tp> RestaurantStorage::getOrderIDs(RestID_tp restaurantID)
{
    OrderStorage orderStorage;
    vector<OrderID_tp> result;
    for (const auto& orderID : getOrderHistoryIDs(restaurantID)) {
        const OrderStatus status = orderStorage.getOrderStatus(orderID);
        if (status == OrderStatus::InPreparation || status == OrderStatus::ReadyToSend) {
            result.push_back(orderID);
        }
    }
    return result;
}

map<RestID_tp, Restaurant> RestaurantStorage::giveAllRestaurants() const
{
    DatabaseManager database;
    map<RestID_tp, Restaurant> restaurants;
    if (!database.isOpen()) {
        return restaurants;
    }

    sqlite3_stmt* statement = nullptr;
    if (!prepareStatement(database.connection(), allRestaurantsSql, &statement)) {
        return restaurants;
    }

    int rc = SQLITE_OK;
    while ((rc = sqlite3_step(statement)) == SQLITE_ROW) {
        const unsigned char* idText = sqlite3_column_text(statement, 0);
        const RestID_tp restaurantID = sqliteText(idText);

        Restaurant restaurant;
        if (!loadRestaurant(database.connection(), restaurantID, restaurant)) {
            restaurants.clear();
            rc = SQLITE_ABORT;
            break;
        }

        restaurants.emplace(restaurantID, restaurant);
    }

    if (rc != SQLITE_DONE && rc != SQLITE_ABORT) {
        printSQLiteError(database.connection(), "step");
        restaurants.clear();
    }

    sqlite3_finalize(statement);
    return restaurants;
}

vector<RestID_tp> RestaurantStorage::activeRestaurantList()
{
    DatabaseManager database;
    vector<RestID_tp> restaurantIDs;
    if (!database.isOpen()) {
        return restaurantIDs;
    }

    sqlite3_stmt* statement = nullptr;
    if (!prepareStatement(database.connection(), activeRestaurantsSql, &statement)) {
        return restaurantIDs;
    }

    int rc = SQLITE_OK;
    while ((rc = sqlite3_step(statement)) == SQLITE_ROW) {
        const unsigned char* idText = sqlite3_column_text(statement, 0);
        restaurantIDs.emplace_back(sqliteText(idText));
    }

    if (rc != SQLITE_DONE) {
        printSQLiteError(database.connection(), "step");
        restaurantIDs.clear();
    }

    sqlite3_finalize(statement);
    return restaurantIDs;
}
