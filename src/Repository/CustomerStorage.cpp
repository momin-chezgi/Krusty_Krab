#include "Repository/CustomerStorage.h"

#include "Database/DatabaseManager.h"

namespace {
    const string customerExistsSql = "SELECT 1 FROM customers WHERE id = ? LIMIT 1;";
    const string customerSelectSql = "SELECT name FROM customers WHERE id = ?;";
    const string customerSelectOrdersSql =
        "SELECT order_id FROM customer_orders WHERE customer_id = ? ORDER BY rowid;";
    const string allCustomersSql = "SELECT id, name FROM customers ORDER BY rowid;";
    const string insertCustomerSql = "INSERT INTO customers (id, name) VALUES (?, ?);";
    const string updateCustomerSql = "UPDATE customers SET name = ? WHERE id = ?;";
    const string deleteCustomerSql = "DELETE FROM customers WHERE id = ?;";
    const string deleteCustomerOrdersSql =
        "DELETE FROM customer_orders WHERE customer_id = ?;";
    const string insertCustomerOrderSql =
        "INSERT INTO customer_orders (customer_id, order_id) VALUES (?, ?);";

    void printSQLiteError(sqlite3* connection, const string& action)
    {
        cerr << "CustomerStorage::" << action << " failed: "
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

    bool customerExists(sqlite3* connection, const CustID_tp& customerID)
    {
        sqlite3_stmt* statement = nullptr;
        if (!prepareStatement(connection, customerExistsSql, &statement)) {
            return false;
        }

        bool exists = false;
        if (bindText(connection, statement, 1, customerID)) {
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

    bool loadCustomerName(sqlite3* connection, const CustID_tp& customerID, string& name)
    {
        sqlite3_stmt* statement = nullptr;
        if (!prepareStatement(connection, customerSelectSql, &statement)) {
            return false;
        }

        bool found = false;
        if (bindText(connection, statement, 1, customerID)) {
            const int rc = sqlite3_step(statement);
            if (rc == SQLITE_ROW) {
                const unsigned char* text = sqlite3_column_text(statement, 0);
                name = text != nullptr ? reinterpret_cast<const char*>(text) : "";
                found = true;
            } else if (rc != SQLITE_DONE) {
                printSQLiteError(connection, "step");
            }
        }

        sqlite3_finalize(statement);
        return found;
    }

    bool loadCustomerOrders(
        sqlite3* connection,
        const CustID_tp& customerID,
        vector<OrderID_tp>& orderIDs
    )
    {
        sqlite3_stmt* statement = nullptr;
        if (!prepareStatement(connection, customerSelectOrdersSql, &statement)) {
            return false;
        }

        bool success = false;
        if (bindText(connection, statement, 1, customerID)) {
            int rc = SQLITE_OK;
            while ((rc = sqlite3_step(statement)) == SQLITE_ROW) {
                const unsigned char* text = sqlite3_column_text(statement, 0);
                orderIDs.emplace_back(text != nullptr ? reinterpret_cast<const char*>(text) : "");
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

    bool loadCustomer(sqlite3* connection, const CustID_tp& customerID, Customer& customer)
    {
        string name;
        if (!loadCustomerName(connection, customerID, name)) {
            return false;
        }

        vector<OrderID_tp> orderIDs;
        if (!loadCustomerOrders(connection, customerID, orderIDs)) {
            return false;
        }

        customer = Customer(name, orderIDs);
        return true;
    }

    bool replaceCustomerOrders(
        DatabaseManager& database,
        const CustID_tp& customerID,
        const Customer& customer
    )
    {
        if (!database.execute(deleteCustomerOrdersSql, {customerID})) {
            return false;
        }

        for (const auto& orderID : customer.getMyOrders()) {
            if (!database.execute(insertCustomerOrderSql, {customerID, orderID})) {
                return false;
            }
        }

        return true;
    }
}

Customer CustomerStorage::giveCustomer(CustID_tp customerID)
{
    DatabaseManager database;
    if (!database.isOpen()) {
        return Customer("NotFound");
    }

    Customer customer;
    if (!loadCustomer(database.connection(), customerID, customer)) {
        return Customer("NotFound");
    }

    return customer;
}

bool CustomerStorage::isValidCustomer(CustID_tp customerID)
{
    DatabaseManager database;
    if (!database.isOpen()) {
        return false;
    }

    return customerExists(database.connection(), customerID);
}

bool CustomerStorage::saveCustomer(const Customer& newCustomer)
{
    DatabaseManager database;
    if (!database.isOpen()) {
        return false;
    }

    DatabaseManager::Transaction transaction(database);
    if (!transaction.isActive()) {
        return false;
    }

    const CustID_tp customerID = newCustomer.getID();
    if (!database.execute(insertCustomerSql, {customerID, newCustomer.getName()})) {
        return false;
    }

    if (!replaceCustomerOrders(database, customerID, newCustomer)) {
        return false;
    }

    return transaction.commit();
}

bool CustomerStorage::updateCustomer(const Customer& updatingCustomer)
{
    DatabaseManager database;
    if (!database.isOpen()) {
        return false;
    }

    const CustID_tp customerID = updatingCustomer.getID();
    if (!customerExists(database.connection(), customerID)) {
        return false;
    }

    DatabaseManager::Transaction transaction(database);
    if (!transaction.isActive()) {
        return false;
    }

    if (!database.execute(updateCustomerSql, {updatingCustomer.getName(), customerID})) {
        return false;
    }

    if (!replaceCustomerOrders(database, customerID, updatingCustomer)) {
        return false;
    }

    return transaction.commit();
}

bool CustomerStorage::deleteCustomer(CustID_tp customerID)
{
    DatabaseManager database;
    if (!database.isOpen()) {
        return false;
    }

    if (!customerExists(database.connection(), customerID)) {
        return false;
    }

    DatabaseManager::Transaction transaction(database);
    if (!transaction.isActive()) {
        return false;
    }

    if (!database.execute(deleteCustomerOrdersSql, {customerID})) {
        return false;
    }

    if (!database.execute(deleteCustomerSql, {customerID})) {
        return false;
    }

    return transaction.commit();
}

map<CustID_tp, Customer> CustomerStorage::giveAllCustomers() const
{
    DatabaseManager database;
    map<CustID_tp, Customer> customers;
    if (!database.isOpen()) {
        return customers;
    }

    sqlite3_stmt* statement = nullptr;
    if (!prepareStatement(database.connection(), allCustomersSql, &statement)) {
        return customers;
    }

    int rc = SQLITE_OK;
    while ((rc = sqlite3_step(statement)) == SQLITE_ROW) {
        const unsigned char* idText = sqlite3_column_text(statement, 0);
        const unsigned char* nameText = sqlite3_column_text(statement, 1);

        const CustID_tp customerID =
            idText != nullptr ? reinterpret_cast<const char*>(idText) : "";
        const string customerName =
            nameText != nullptr ? reinterpret_cast<const char*>(nameText) : "";

        vector<OrderID_tp> orderIDs;
        if (!loadCustomerOrders(database.connection(), customerID, orderIDs)) {
            customers.clear();
            rc = SQLITE_DONE;
            break;
        }

        customers.emplace(
            customerID,
            Customer(customerName, orderIDs)
        );
    }

    if (rc != SQLITE_DONE) {
        printSQLiteError(database.connection(), "step");
        customers.clear();
    }

    sqlite3_finalize(statement);
    return customers;
}
