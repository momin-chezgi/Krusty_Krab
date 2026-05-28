#pragma once

#include "Customer.h"

class Order{
    size_t id;
    Customer *orderer {nullptr};
    vector<MenuItem> order;
    string orderStatus {"In-Preparation"};
    // In-Preparation, Ready-To-Send, Delivered
public:
    Order(size_t initID);
    Order(size_t initID, const Order const *o);
    Order(size_t initID, const Customer const *initOrderer, const vector<const MenuItem> &initOrder = {});

    size_t getID() const;
    Customer *getOrderer() const;
    vector<MenuItem> getOrder() const;
    string getOrderStatus() const;

    void setOrderer(const Customer const *newOrderer);
    void setOrder(const vector<MenuItem> &newOrder);
    void setOrderStatus(string newStatus);
    void copyFromOrder(const Order newOrder);

    bool addItem(const MenuItem const *newItem);       // returns true if it hasn't been in the order and the quantity is on the kitchen
    bool removeItem(const MenuItem const *removingItem); // returns true if it has been in the order and the removing quantity isn't more than the ordered quantity

    ~Order();

};
