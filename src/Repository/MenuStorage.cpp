#include "Repository/MenuStorage.h"

#include "Database/DatabaseManager.h"
#include "Domain/Drink.h"
#include "Domain/Food.h"

namespace {
    const string menuExistsSql = "SELECT 1 FROM menus WHERE id = ? LIMIT 1;";
    const string menuSelectSql = "SELECT id FROM menus WHERE id = ?;";
    const string allMenusSql = "SELECT id FROM menus ORDER BY rowid;";
    const string itemExistsSql =
        "SELECT 1 FROM menu_items WHERE menu_id = ? AND id = ? LIMIT 1;";
    const string itemTypeSql =
        "SELECT item_type FROM menu_items WHERE menu_id = ? AND id = ? LIMIT 1;";
    const string itemSelectSql = R"SQL(
        SELECT
            id,
            item_type,
            name,
            bio,
            price,
            stock_quantity,
            preparation_minutes,
            food_type
        FROM menu_items
        WHERE menu_id = ? AND id = ?
        LIMIT 1;
    )SQL";
    const string menuItemsSql = R"SQL(
        SELECT
            id,
            item_type,
            name,
            bio,
            price,
            stock_quantity,
            preparation_minutes,
            food_type
        FROM menu_items
        WHERE menu_id = ?
        ORDER BY rowid;
    )SQL";
    const string insertMenuSql = "INSERT INTO menus (id) VALUES (?);";
    const string deleteMenuSql = "DELETE FROM menus WHERE id = ?;";
    const string deleteMenuItemsSql = "DELETE FROM menu_items WHERE menu_id = ?;";
    const string deleteItemSql = "DELETE FROM menu_items WHERE menu_id = ? AND id = ?;";
    const string updateItemQuantitySql =
        "UPDATE menu_items SET stock_quantity = ? WHERE menu_id = ? AND id = ?;";
    const string insertItemSql = R"SQL(
        INSERT INTO menu_items (
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
        VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?);
    )SQL";

    void printSQLiteError(sqlite3* connection, const string& action)
    {
        cerr << "MenuStorage::" << action << " failed: "
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

    bool menuExists(sqlite3* connection, const MenuID_tp& menuID)
    {
        sqlite3_stmt* statement = nullptr;
        if (!prepareStatement(connection, menuExistsSql, &statement)) {
            return false;
        }

        bool exists = false;
        if (bindText(connection, statement, 1, menuID)) {
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

    bool itemExists(sqlite3* connection, const MenuID_tp& menuID, const ItemID_tp& itemID)
    {
        sqlite3_stmt* statement = nullptr;
        if (!prepareStatement(connection, itemExistsSql, &statement)) {
            return false;
        }

        bool exists = false;
        if (bindText(connection, statement, 1, menuID)
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

    double itemQuantity(const MenuItem* item)
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

    bool insertItem(DatabaseManager& database, const MenuID_tp& menuID, const MenuItem* item)
    {
        if (item == nullptr) {
            return false;
        }

        return database.execute(
            insertItemSql,
            {
                item->getID(),
                menuID,
                item->getItemType(),
                item->getName(),
                item->getBio(),
                std::to_string(item->getPricePerUnit()),
                std::to_string(itemQuantity(item)),
                std::to_string(item->getPreparationMinutes()),
                item->getFoodType()
            }
        );
    }

    MenuItem* hydrateItem(sqlite3_stmt* statement)
    {
        const unsigned char* idText = sqlite3_column_text(statement, 0);
        const unsigned char* typeText = sqlite3_column_text(statement, 1);
        const unsigned char* nameText = sqlite3_column_text(statement, 2);
        const unsigned char* bioText = sqlite3_column_text(statement, 3);
        const double price = sqlite3_column_double(statement, 4);
        const double stockQuantity = sqlite3_column_double(statement, 5);
        const int preparationMinutes = sqlite3_column_int(statement, 6);
        const unsigned char* foodTypeText = sqlite3_column_text(statement, 7);

        const string itemID = idText != nullptr ? reinterpret_cast<const char*>(idText) : "";
        const string itemType = typeText != nullptr ? reinterpret_cast<const char*>(typeText) : "";
        const string itemName = nameText != nullptr ? reinterpret_cast<const char*>(nameText) : "";
        const string itemBio = bioText != nullptr ? reinterpret_cast<const char*>(bioText) : "";
        const string foodType =
            foodTypeText != nullptr ? reinterpret_cast<const char*>(foodTypeText) : "";

        if (itemType == "Food") {
            return new Food(
                itemName,
                price,
                stockQuantity,
                itemBio,
                static_cast<size_t>(preparationMinutes),
                foodType
            );
        }

        if (itemType == "Drink") {
            return new Drink(
                itemName,
                price,
                stockQuantity,
                itemBio,
                static_cast<size_t>(preparationMinutes)
            );
        }

        return nullptr;
    }

    void clearItems(vector<MenuItem*>& items)
    {
        for (MenuItem* item : items) {
            delete item;
        }
        items.clear();
    }

    bool loadMenuItems(
        sqlite3* connection,
        const MenuID_tp& menuID,
        vector<MenuItem*>& items
    )
    {
        sqlite3_stmt* statement = nullptr;
        if (!prepareStatement(connection, menuItemsSql, &statement)) {
            return false;
        }

        bool success = false;
        if (bindText(connection, statement, 1, menuID)) {
            int rc = SQLITE_OK;
            while ((rc = sqlite3_step(statement)) == SQLITE_ROW) {
                MenuItem* item = hydrateItem(statement);
                if (item == nullptr) {
                    clearItems(items);
                    rc = SQLITE_ABORT;
                    break;
                }
                items.push_back(item);
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

    bool loadMenu(sqlite3* connection, const MenuID_tp& menuID, Menu& menu)
    {
        sqlite3_stmt* statement = nullptr;
        if (!prepareStatement(connection, menuSelectSql, &statement)) {
            return false;
        }

        bool found = false;
        if (bindText(connection, statement, 1, menuID)) {
            const int rc = sqlite3_step(statement);
            if (rc == SQLITE_ROW) {
                vector<MenuItem*> items;
                if (loadMenuItems(connection, menuID, items)) {
                    menu = Menu(items);
                    clearItems(items);
                    found = true;
                }
            } else if (rc != SQLITE_DONE) {
                printSQLiteError(connection, "step");
            }
        }

        sqlite3_finalize(statement);
        return found;
    }

    ItemType itemTypeFromRow(sqlite3* connection, const MenuID_tp& menuID, const ItemID_tp& itemID)
    {
        sqlite3_stmt* statement = nullptr;
        if (!prepareStatement(connection, itemTypeSql, &statement)) {
            return ItemType::None;
        }

        ItemType result = ItemType::None;
        if (bindText(connection, statement, 1, menuID)
            && bindText(connection, statement, 2, itemID)) {
            const int rc = sqlite3_step(statement);
            if (rc == SQLITE_ROW) {
                const unsigned char* typeText = sqlite3_column_text(statement, 0);
                const string itemType =
                    typeText != nullptr ? reinterpret_cast<const char*>(typeText) : "";
                if (itemType == "Food") {
                    result = ItemType::Food;
                } else if (itemType == "Drink") {
                    result = ItemType::Drink;
                }
            } else if (rc != SQLITE_DONE) {
                printSQLiteError(connection, "step");
            }
        }

        sqlite3_finalize(statement);
        return result;
    }

    MenuItem* loadSingleItem(sqlite3* connection, const MenuID_tp& menuID, const ItemID_tp& itemID)
    {
        sqlite3_stmt* statement = nullptr;
        if (!prepareStatement(connection, itemSelectSql, &statement)) {
            return nullptr;
        }

        MenuItem* item = nullptr;
        if (bindText(connection, statement, 1, menuID)
            && bindText(connection, statement, 2, itemID)) {
            const int rc = sqlite3_step(statement);
            if (rc == SQLITE_ROW) {
                item = hydrateItem(statement);
            } else if (rc != SQLITE_DONE) {
                printSQLiteError(connection, "step");
            }
        }

        sqlite3_finalize(statement);
        return item;
    }

    bool replaceMenuItems(DatabaseManager& database, const Menu& menu)
    {
        const MenuID_tp menuID = menu.getID();
        if (!database.execute(deleteMenuItemsSql, {menuID})) {
            return false;
        }

        for (const auto* item : menu.getMenu()) {
            if (!insertItem(database, menuID, item)) {
                return false;
            }
        }

        return true;
    }
}

bool MenuStorage::has(MenuID_tp menuID, ItemID_tp itemID)
{
    DatabaseManager database;
    if (!database.isOpen()) {
        return false;
    }

    return itemExists(database.connection(), menuID, itemID);
}

ItemType MenuStorage::type(MenuID_tp menuID, ItemID_tp itemID)
{
    DatabaseManager database;
    if (!database.isOpen()) {
        return ItemType::None;
    }

    return itemTypeFromRow(database.connection(), menuID, itemID);
}

bool MenuStorage::isValidMenu(MenuID_tp menuID)
{
    DatabaseManager database;
    if (!database.isOpen()) {
        return false;
    }

    return menuExists(database.connection(), menuID);
}

bool MenuStorage::saveMenu(const Menu& newMenu)
{
    DatabaseManager database;
    if (!database.isOpen()) {
        return false;
    }

    const MenuID_tp menuID = newMenu.getID();
    if (menuExists(database.connection(), menuID)) {
        return false;
    }

    DatabaseManager::Transaction transaction(database);
    if (!transaction.isActive()) {
        return false;
    }

    if (!database.execute(insertMenuSql, {menuID})) {
        return false;
    }

    if (!replaceMenuItems(database, newMenu)) {
        return false;
    }

    return transaction.commit();
}

Menu MenuStorage::giveMenu(MenuID_tp menuID)
{
    DatabaseManager database;
    if (!database.isOpen()) {
        return Menu();
    }

    Menu menu;
    if (!loadMenu(database.connection(), menuID, menu)) {
        return Menu();
    }

    return menu;
}

bool MenuStorage::deleteMenu(MenuID_tp menuID)
{
    DatabaseManager database;
    if (!database.isOpen()) {
        return false;
    }

    if (!menuExists(database.connection(), menuID)) {
        return false;
    }

    DatabaseManager::Transaction transaction(database);
    if (!transaction.isActive()) {
        return false;
    }

    if (!database.execute(deleteMenuItemsSql, {menuID})) {
        return false;
    }

    if (!database.execute(deleteMenuSql, {menuID})) {
        return false;
    }

    return transaction.commit();
}

bool MenuStorage::updateMenu(const Menu& updatingMenu)
{
    DatabaseManager database;
    if (!database.isOpen()) {
        return false;
    }

    const MenuID_tp menuID = updatingMenu.getID();
    if (!menuExists(database.connection(), menuID)) {
        return false;
    }

    DatabaseManager::Transaction transaction(database);
    if (!transaction.isActive()) {
        return false;
    }

    if (!replaceMenuItems(database, updatingMenu)) {
        return false;
    }

    return transaction.commit();
}

bool MenuStorage::addItem(MenuID_tp menuID, const MenuItem* newItem)
{
    DatabaseManager database;
    if (!database.isOpen()) {
        return false;
    }

    if (!menuExists(database.connection(), menuID) || newItem == nullptr) {
        return false;
    }

    if (itemExists(database.connection(), menuID, newItem->getID())) {
        return false;
    }

    return insertItem(database, menuID, newItem);
}

bool MenuStorage::deleteItem(MenuID_tp menuID, ItemID_tp itemID)
{
    DatabaseManager database;
    if (!database.isOpen()) {
        return false;
    }

    if (!itemExists(database.connection(), menuID, itemID)) {
        return false;
    }

    return database.execute(deleteItemSql, {menuID, itemID});
}

bool MenuStorage::reduceItemQuantity(MenuID_tp menuID, ItemID_tp itemID, double quantity)
{
    DatabaseManager database;
    if (!database.isOpen()) {
        return false;
    }

    MenuItem* item = loadSingleItem(database.connection(), menuID, itemID);
    if (item == nullptr) {
        return false;
    }

    const bool reduced = item->delItemQuantity(quantity);
    if (!reduced) {
        delete item;
        return false;
    }

    const bool updated = database.execute(
        updateItemQuantitySql,
        {
            std::to_string(itemQuantity(item)),
            menuID,
            itemID
        }
    );

    delete item;
    return updated;
}

MenuItem* MenuStorage::cloneItem(MenuID_tp menuID, ItemID_tp itemID)
{
    DatabaseManager database;
    if (!database.isOpen()) {
        return nullptr;
    }

    MenuItem* item = loadSingleItem(database.connection(), menuID, itemID);
    if (item == nullptr) {
        return nullptr;
    }

    MenuItem* clone = item->clone();
    delete item;
    return clone;
}

map<MenuID_tp, Menu> MenuStorage::giveAllMenus() const
{
    DatabaseManager database;
    map<MenuID_tp, Menu> menus;
    if (!database.isOpen()) {
        return menus;
    }

    sqlite3_stmt* statement = nullptr;
    if (!prepareStatement(database.connection(), allMenusSql, &statement)) {
        return menus;
    }

    int rc = SQLITE_OK;
    while ((rc = sqlite3_step(statement)) == SQLITE_ROW) {
        const unsigned char* idText = sqlite3_column_text(statement, 0);
        const MenuID_tp menuID = idText != nullptr ? reinterpret_cast<const char*>(idText) : "";

        Menu menu;
        if (!loadMenu(database.connection(), menuID, menu)) {
            menus.clear();
            rc = SQLITE_ABORT;
            break;
        }

        menus.emplace(menuID, menu);
    }

    if (rc != SQLITE_DONE && rc != SQLITE_ABORT) {
        printSQLiteError(database.connection(), "step");
        menus.clear();
    }

    sqlite3_finalize(statement);
    return menus;
}
