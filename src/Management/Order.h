#pragma once

#include "Customer.h"

enum OrderStatus{
    InPreparation,
    ReadyToSend,
    Delivered
};
bool orderStatus2IsPrepared(OrderStatus s);
string orderStatus2String(OrderStatus s);
OrderStatus orderStatusString2Enum(string s);

class Order{
    const string id = IDGenerator::uuid();
    Customer *orderer {nullptr};
    vector<MenuItem> order;
    OrderStatus orderStatus {InPreparation};
    // In-Preparation, Ready-To-Send, Delivered
public:
    Order(   );
    Order(   const Order const *o);
    Order::Order(   const Customer const *initOrderer, const vector<MenuItem> &initOrder = {});

    string getID() const;
    Customer *getOrderer() const;
    vector<MenuItem> getOrder() const;
    OrderStatus getOrderStatus() const;

    void setOrderer(const Customer const *newOrderer);
    void setOrder(const vector<MenuItem> &newOrder);
    void setOrderStatus(OrderStatus newStatus);
    void copyFromOrder(const Order newOrder);

    bool addItem(const MenuItem const *newItem);       // returns true if it hasn't been in the order and the quantity is on the kitchen
    bool removeItem(string menuItemID); // returns true if it has been in the order and the removing quantity isn't more than the ordered quantity

    ~Order();

};
