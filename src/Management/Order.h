#pragma once

# include "Customer.h"
# include "Enums.h"

bool orderStatus2IsPrepared(OrderStatus s);
string orderStatus2String(OrderStatus s);
OrderStatus orderStatusString2Enum(string s);

class Order{
public:
    Order() = default;
    Order(const Order& o);
    Order::Order(string oID, const vector<OrderLine> &initOrder = {});
    // For that time, the correctness of oID(ordererID) is 
    // the responsibility of the caller function,
    //  but we can add some authentication process to it in the future

    string getID() const;
    string getOrderer() const;
    vector<OrderLine> getOrder() const;
    OrderStatus getOrderStatus() const;
    cost getTotalPrice() const;

    void setOrderer(string newOrdererID);
    void setOrderStatus(OrderStatus newStatus);
    void copyFromOrder(const Order newOrder);
    Order& operator=(const Order& newOrder);
    // There isn't setOrderCost, because it's calculation depends on vector<OrderLine>

    bool addItem(const MenuItem& newItem, double quantity);       
    // returns true if it hasn't been in the order and the quantity is on the kitchen
    bool removeItem(string menuItemID);
    // returns true if it has been in the order and the removing quantity isn't more than the ordered quantity

    ~Order();
private:
    string id = IDGenerator::uuid();
    string ordererID;
    vector<OrderLine> order;
    OrderStatus orderStatus {OrderStatus::InPreparation};
};
