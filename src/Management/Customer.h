#pragma once

#include "Order.h"
#include "Menu.h"

class Customer{
    size_t id;
    string name;
    vector<Order *> myOrders {nullptr};
    // It just saves the current orders, not all history of orders
    // The history of all orders are stored in MySQL file
public:
    Customer(size_t initID);
    Customer(size_t initID, string initName, const vector<Order const *> &initOrder = {nullptr});

    void order (Order * o);
    Menu getMenu() const;
    const vector<Order const*>& getMyOrder() const;
    bool deleteFromMyOrder (size_t orderID, MenuItem mi); // true if `mi` has been in the menu and flase vice-versa
    bool addToMyOrder (size_t orderID, MenuItem mi); // true if `mi` has been in the menu and flase vice-versa
    vector<Order> historyOfOrders() const;
    bool isReady (size_t orderID, MenuItem mi) const;

    ~Customer();
};
