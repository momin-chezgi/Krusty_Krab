#pragma once

#include "Domain/Order.h"

class Customer{
    CustID_tp id = IDGenerator::uuid();
    string name;
    vector<OrderID_tp> myOrders;
    // It just saves the current orders, not all history of orders
    // The history of all orders are stored in MySQL file
public:
    Customer();
    Customer(string initName, const vector<OrderID_tp> &initOrder = {});
    Customer(CustID_tp initID, string initName, const vector<OrderID_tp> &initOrder = {});
    Customer(const Customer& other) = default;

    void orderOut(OrderID_tp orderID);
    MenuID_tp getMenuID(RestID_tp restaurantID) const;
    CustID_tp getID() const;
    string getName() const;
    const vector<OrderID_tp>& getMyOrders() const;
    bool deleteFromMyOrder (OrderID_tp orderID, ItemID_tp menuItemID); // true if `mi` has been in the menu and flase vice-versa
    bool addToMyOrder (OrderID_tp orderID, MenuID_tp menuID, ItemID_tp itemID, double quantity = 1); // true if `mi` has been in the menu and flase vice-versa
    vector<Order> historyOfOrders() const;
    bool isReady (OrderID_tp orderID) const;

    Customer& operator=(const Customer& other);
    ~Customer();
};
