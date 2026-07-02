# pragma once 

# include "Common/Types.h"
# include "Domain/Order.h"

class OrderStorage{
public:
    bool isValidOrder(OrderID_tp orderID);
    bool saveOrder(const Order& newOrder);
    // returns true if the order has been saved successfully, otherwise false
    bool deleteOrder(OrderID_tp orderID);  // returns true if the order with the given ID exists and has been deleted, otherwise returns false
    bool updateOrder(const Order& updatingOrder);  // returns true if the order with the given ID exists and has been updated, otherwise returns false
    bool addItem(OrderID_tp orderID, MenuID_tp menuID, ItemID_tp itemID, double quantity=1);
    bool removeItem(OrderID_tp orderID, ItemID_tp itemID);
    OrderStatus getOrderStatus(OrderID_tp orderID);
    map<OrderID_tp, Order> giveAllOrders() const;
    bool updateStatus(OrderID_tp orderID, OrderStatus stat);
};
