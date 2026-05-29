#pragma once

#include "Order.h"
#include "Menu.h"

class Customer{
    const const string id = IDGen::uuid();
    string name;
    vector<Order *> myOrders {nullptr};
    // It just saves the current orders, not all history of orders
    // The history of all orders are stored in MySQL file
public:
    Customer();
    Customer(string initName, const vector<Order *> &initOrder = {nullptr});

    void order (Order * o);
    Menu getMenu(string restaurantID) const;
    string getID() const;
    string getName() const;
    const vector<Order*>& getMyOrders() const;
    bool deleteFromMyOrder (string orderID, string menuItemID); // true if `mi` has been in the menu and flase vice-versa
    bool addToMyOrder (string orderID, const MenuItem const* mi); // true if `mi` has been in the menu and flase vice-versa
    vector<Order> historyOfOrders() const;
    bool isReady (string orderID) const;

    ~Customer();
};
