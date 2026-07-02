
#include "Repository/OrderStorage.h"

#include "Database/DatabaseManager.h"
#include "Domain/Drink.h"
#include "Domain/Food.h"

namespace {
    const string orderExistsSql = "SELECT 1 FROM orders WHERE id = ? LIMIT 1;";
    const string orderSelectSql = R"SQL(
        SELECT id, orderer_id, status
        FROM orders
        WHERE id = ?
        LIMIT 1;
    )SQL";
    const string allOrdersSql = R"SQL(
        SELECT id, orderer_id, status
        FROM orders
        ORDER BY rowid;
    )SQL";
    const string orderItemsSql = R"SQL(
        SELECT
            item_id,
            item_type,
            name,
            bio,
            price,
            quantity,
            quantity_snapshot,
            preparation_minutes,
            food_type
        FROM order_items
        WHERE order_id = ?
        ORDER BY rowid;
    )SQL";
    const string orderItemExistsSql =
        "SELECT 1 FROM order_items WHERE order_id = ? AND item_id = ? LIMIT 1;";
    const string insertOrderSql = R"SQL(
        INSERT INTO orders (id, orderer_id, restaurant_id, status, total_price)
        VALUES (?, ?, '', ?, ?);
    )SQL";
    const string updateOrderSql = R"SQL(
        UPDATE orders
        SET orderer_id = ?, status = ?, total_price = ?
        WHERE id = ?;
    )SQL";
    const string deleteOrderSql = "DELETE FROM orders WHERE id = ?;";
    const string deleteOrderItemsSql = "DELETE FROM order_items WHERE order_id = ?;";
    const string deleteOrderItemSql =
        "DELETE FROM order_items WHERE order_id = ? AND item_id = ?;";
    const string updateOrderStatusSql = "UPDATE orders SET status = ? WHERE id = ?;";
    const string updateOrderTotalSql = R"SQL(
        UPDATE orders
        SET total_price = COALESCE(
            (SELECT SUM(price * quantity) FROM order_items WHERE order_id = ?),
            0
        )
        WHERE id = ?;
    )SQL";
    const string incrementOrderItemQuantitySql = R"SQL(
        UPDATE order_items
        SET quantity = quantity + ?
        WHERE order_id = ? AND item_id = ?;
    )SQL";
    const string insertOrderItemFromMenuSql = R"SQL(
        INSERT INTO order_items (
            order_id,
            item_id,
            item_type,
            name,
            bio,
            price,
            quantity,
            quantity_snapshot,
            preparation_minutes,
            food_type
        )
        SELECT
            ?,
            id,
            item_type,
            name,
            bio,
            price,
            ?,
            stock_quantity,
            preparation_minutes,
            food_type
        FROM menu_items
        WHERE menu_id = ? AND id = ?;
    )SQL";
    const string insertOrderItemSnapshotSql = R"SQL(
        INSERT INTO order_items (
            order_id,
            item_id,
            item_type,
            name,
            bio,
            price,
            quantity,
            quantity_snapshot,
            preparation_minutes,
            food_type
        )
        VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?);
    )SQL";

    void printSQLiteError(sqlite3* connection, const string& action)
    {
        cerr << "OrderStorage::" << action << " failed: "
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

    bool orderExists(sqlite3* connection, const OrderID_tp& orderID)
    {
        sqlite3_stmt* statement = nullptr;
        if (!prepareStatement(connection, orderExistsSql, &statement)) {
            return false;
        }

        bool exists = false;
        if (bindText(connection, statement, 1, orderID)) {
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

    bool orderItemExists(
        sqlite3* connection,
        const OrderID_tp& orderID,
        const ItemID_tp& itemID
    )
    {
        sqlite3_stmt* statement = nullptr;
        if (!prepareStatement(connection, orderItemExistsSql, &statement)) {
            return false;
        }

        bool exists = false;
        if (bindText(connection, statement, 1, orderID)
            && bindText(connection, statement, 2, itemID)) {
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

    OrderStatus statusFromStorage(const string& status)
    {
        if (status == "In-Preparation" || status == "InPreparation") {
            return OrderStatus::InPreparation;
        }
        if (status == "Ready-To-Send" || status == "ReadyToSend") {
            return OrderStatus::ReadyToSend;
        }
        if (status == "Delivered") {
            return OrderStatus::Delivered;
        }
        if (status == "Cancelled") {
            return OrderStatus::Cancelled;
        }
        return OrderStatus::Cancelled;
    }

    double itemQuantitySnapshot(const MenuItem* item)
    {
        if (item == nullptr) {
            return 0.0;
        }

        if (item->getItemType() == "Food") {
            const Food* food = dynamic_cast<const Food*>(item);
            return food != nullptr ? food->getWeight() : 0.0;
        }

        if (item->getItemType() == "Drink") {
            const Drink* drink = dynamic_cast<const Drink*>(item);
            return drink != nullptr ? drink->getVolume() : 0.0;
        }

        return 0.0;
    }

    MenuItem* hydrateOrderItem(sqlite3_stmt* statement)
    {
        const unsigned char* idText = sqlite3_column_text(statement, 0);
        const unsigned char* typeText = sqlite3_column_text(statement, 1);
        const unsigned char* nameText = sqlite3_column_text(statement, 2);
        const unsigned char* bioText = sqlite3_column_text(statement, 3);
        const double price = sqlite3_column_double(statement, 4);
        const double quantitySnapshot = sqlite3_column_double(statement, 6);
        const int preparationMinutes = sqlite3_column_int(statement, 7);
        const unsigned char* foodTypeText = sqlite3_column_text(statement, 8);

        const ItemID_tp itemID = idText != nullptr ? reinterpret_cast<const char*>(idText) : "";
        const string itemType = typeText != nullptr ? reinterpret_cast<const char*>(typeText) : "";
        const string name = nameText != nullptr ? reinterpret_cast<const char*>(nameText) : "";
        const string bio = bioText != nullptr ? reinterpret_cast<const char*>(bioText) : "";
        const string foodType =
            foodTypeText != nullptr ? reinterpret_cast<const char*>(foodTypeText) : "";

        if (itemType == "Food") {
            return new Food(
                name,
                price,
                quantitySnapshot,
                bio,
                static_cast<size_t>(preparationMinutes),
                foodType
            );
        }

        if (itemType == "Drink") {
            return new Drink(
                name,
                price,
                quantitySnapshot,
                bio,
                static_cast<size_t>(preparationMinutes)
            );
        }

        return nullptr;
    }

    void clearLines(vector<OrderLine>& lines)
    {
        for (auto& line : lines) {
            delete line.first;
            line.first = nullptr;
        }
        lines.clear();
    }

    bool loadOrderItems(
        sqlite3* connection,
        const OrderID_tp& orderID,
        vector<OrderLine>& lines
    )
    {
        sqlite3_stmt* statement = nullptr;
        if (!prepareStatement(connection, orderItemsSql, &statement)) {
            return false;
        }

        bool success = false;
        if (bindText(connection, statement, 1, orderID)) {
            int rc = SQLITE_OK;
            while ((rc = sqlite3_step(statement)) == SQLITE_ROW) {
                MenuItem* item = hydrateOrderItem(statement);
                if (item == nullptr) {
                    clearLines(lines);
                    rc = SQLITE_ABORT;
                    break;
                }

                const double orderedQuantity = sqlite3_column_double(statement, 5);
                lines.push_back({item, orderedQuantity});
            }

            if (rc == SQLITE_DONE) {
                success = true;
            } else if (rc != SQLITE_ABORT) {
                printSQLiteError(connection, "step");
            }
        }

        sqlite3_finalize(statement);
        return success;
    }

    bool loadOrder(sqlite3* connection, const OrderID_tp& orderID, Order& order)
    {
        sqlite3_stmt* statement = nullptr;
        if (!prepareStatement(connection, orderSelectSql, &statement)) {
            return false;
        }

        bool found = false;
        if (bindText(connection, statement, 1, orderID)) {
            const int rc = sqlite3_step(statement);
            if (rc == SQLITE_ROW) {
                const unsigned char* idText = sqlite3_column_text(statement, 0);
                const unsigned char* ordererText = sqlite3_column_text(statement, 1);
                const unsigned char* statusText = sqlite3_column_text(statement, 2);

                const OrderID_tp persistedID =
                    idText != nullptr ? reinterpret_cast<const char*>(idText) : orderID;
                const CustID_tp ordererID =
                    ordererText != nullptr ? reinterpret_cast<const char*>(ordererText) : "";
                const string status =
                    statusText != nullptr ? reinterpret_cast<const char*>(statusText) : "";

                vector<OrderLine> lines;
                if (loadOrderItems(connection, persistedID, lines)) {
                    order = Order(ordererID, lines);
                    order.setOrderStatus(statusFromStorage(status));
                    clearLines(lines);
                    found = true;
                }
            } else if (rc != SQLITE_DONE) {
                printSQLiteError(connection, "step");
            }
        }

        sqlite3_finalize(statement);
        return found;
    }

    bool insertOrderItemSnapshot(
        DatabaseManager& database,
        const OrderID_tp& orderID,
        const OrderLine& line
    )
    {
        const MenuItem* item = line.first;
        if (item == nullptr) {
            return false;
        }

        return database.execute(
            insertOrderItemSnapshotSql,
            {
                orderID,
                item->getID(),
                item->getItemType(),
                item->getName(),
                item->getBio(),
                std::to_string(item->getPricePerUnit()),
                std::to_string(line.second),
                std::to_string(itemQuantitySnapshot(item)),
                std::to_string(item->getPreparationMinutes()),
                item->getFoodType()
            }
        );
    }

    bool replaceOrderItems(DatabaseManager& database, const Order& order)
    {
        const OrderID_tp orderID = order.getID();
        if (!database.execute(deleteOrderItemsSql, {orderID})) {
            return false;
        }

        for (const auto& line : order.getOrder()) {
            if (!insertOrderItemSnapshot(database, orderID, line)) {
                return false;
            }
        }

        return true;
    }

    bool updateOrderTotal(DatabaseManager& database, const OrderID_tp& orderID)
    {
        return database.execute(updateOrderTotalSql, {orderID, orderID});
    }
}

bool OrderStorage::saveOrder(const Order& newOrder)
{
    DatabaseManager database;
    if (!database.isOpen()) {
        return false;
    }

    const OrderID_tp orderID = newOrder.getID();
    if (orderExists(database.connection(), orderID)) {
        return false;
    }

    DatabaseManager::Transaction transaction(database);
    if (!transaction.isActive()) {
        return false;
    }

    if (!database.execute(
        insertOrderSql,
        {
            orderID,
            newOrder.getOrderer(),
            orderStatus2String(newOrder.getOrderStatus()),
            std::to_string(newOrder.getTotalPrice())
        }
    )) {
        return false;
    }

    if (!replaceOrderItems(database, newOrder)) {
        return false;
    }

    return transaction.commit();
}

bool OrderStorage::deleteOrder(OrderID_tp orderID)
{
    DatabaseManager database;
    if (!database.isOpen()) {
        return false;
    }

    if (!orderExists(database.connection(), orderID)) {
        return false;
    }

    DatabaseManager::Transaction transaction(database);
    if (!transaction.isActive()) {
        return false;
    }

    if (!database.execute(deleteOrderItemsSql, {orderID})) {
        return false;
    }

    if (!database.execute(deleteOrderSql, {orderID})) {
        return false;
    }

    return transaction.commit();
}

bool OrderStorage::updateOrder(const Order& updatingOrder)
{
    DatabaseManager database;
    if (!database.isOpen()) {
        return false;
    }

    const OrderID_tp orderID = updatingOrder.getID();
    if (!orderExists(database.connection(), orderID)) {
        return false;
    }

    DatabaseManager::Transaction transaction(database);
    if (!transaction.isActive()) {
        return false;
    }

    if (!database.execute(
        updateOrderSql,
        {
            updatingOrder.getOrderer(),
            orderStatus2String(updatingOrder.getOrderStatus()),
            std::to_string(updatingOrder.getTotalPrice()),
            orderID
        }
    )) {
        return false;
    }

    if (!replaceOrderItems(database, updatingOrder)) {
        return false;
    }

    return transaction.commit();
}

bool OrderStorage::isValidOrder(OrderID_tp orderID)
{
    DatabaseManager database;
    if (!database.isOpen()) {
        return false;
    }

    return orderExists(database.connection(), orderID);
}

bool OrderStorage::addItem(OrderID_tp orderID, MenuID_tp menuID, ItemID_tp itemID, double quantity)
{
    DatabaseManager database;
    if (!database.isOpen()) {
        return false;
    }

    if (!orderExists(database.connection(), orderID)) {
        return false;
    }

    DatabaseManager::Transaction transaction(database);
    if (!transaction.isActive()) {
        return false;
    }

    if (orderItemExists(database.connection(), orderID, itemID)) {
        if (!database.execute(
            incrementOrderItemQuantitySql,
            {std::to_string(quantity), orderID, itemID}
        )) {
            return false;
        }
    } else {
        if (!database.execute(
            insertOrderItemFromMenuSql,
            {orderID, std::to_string(quantity), menuID, itemID}
        )) {
            return false;
        }

        if (sqlite3_changes(database.connection()) == 0) {
            return false;
        }
    }

    if (!updateOrderTotal(database, orderID)) {
        return false;
    }

    return transaction.commit();
}

bool OrderStorage::removeItem(OrderID_tp orderID, ItemID_tp itemID)
{
    DatabaseManager database;
    if (!database.isOpen()) {
        return false;
    }

    if (!orderExists(database.connection(), orderID)
        || !orderItemExists(database.connection(), orderID, itemID)) {
        return false;
    }

    DatabaseManager::Transaction transaction(database);
    if (!transaction.isActive()) {
        return false;
    }

    if (!database.execute(deleteOrderItemSql, {orderID, itemID})) {
        return false;
    }

    if (!updateOrderTotal(database, orderID)) {
        return false;
    }

    return transaction.commit();
}

OrderStatus OrderStorage::getOrderStatus(OrderID_tp orderID)
{
    DatabaseManager database;
    if (!database.isOpen()) {
        return OrderStatus::Cancelled;
    }

    sqlite3_stmt* statement = nullptr;
    if (!prepareStatement(database.connection(), "SELECT status FROM orders WHERE id = ?;", &statement)) {
        return OrderStatus::Cancelled;
    }

    OrderStatus status = OrderStatus::Cancelled;
    if (bindText(database.connection(), statement, 1, orderID)) {
        const int rc = sqlite3_step(statement);
        if (rc == SQLITE_ROW) {
            const unsigned char* text = sqlite3_column_text(statement, 0);
            status = statusFromStorage(text != nullptr ? reinterpret_cast<const char*>(text) : "");
        } else if (rc != SQLITE_DONE) {
            printSQLiteError(database.connection(), "step");
        }
    }

    sqlite3_finalize(statement);
    return status;
}

map<OrderID_tp, Order> OrderStorage::giveAllOrders() const
{
    DatabaseManager database;
    map<OrderID_tp, Order> orders;
    if (!database.isOpen()) {
        return orders;
    }

    sqlite3_stmt* statement = nullptr;
    if (!prepareStatement(database.connection(), allOrdersSql, &statement)) {
        return orders;
    }

    int rc = SQLITE_OK;
    while ((rc = sqlite3_step(statement)) == SQLITE_ROW) {
        const unsigned char* idText = sqlite3_column_text(statement, 0);
        const OrderID_tp orderID =
            idText != nullptr ? reinterpret_cast<const char*>(idText) : "";

        Order order;
        if (!loadOrder(database.connection(), orderID, order)) {
            orders.clear();
            rc = SQLITE_ABORT;
            break;
        }

        orders.emplace(orderID, order);
    }

    if (rc != SQLITE_DONE && rc != SQLITE_ABORT) {
        printSQLiteError(database.connection(), "step");
        orders.clear();
    }

    sqlite3_finalize(statement);
    return orders;
}

bool OrderStorage::updateStatus(OrderID_tp orderID, OrderStatus stat)
{
    DatabaseManager database;
    if (!database.isOpen()) {
        return false;
    }

    if (!orderExists(database.connection(), orderID)) {
        return false;
    }

    return database.execute(updateOrderStatusSql, {orderStatus2String(stat), orderID});
}
