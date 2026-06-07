
#include "Repository/OrderStorage.h"

map<OrderID_tp, Order> OrderStorage::orders;

bool OrderStorage::saveOrder(const Order& newOrder)
{
    auto result = orders.insert({newOrder.getID(), newOrder});
    return result.second; 
    // returns true if the order was inserted,
    //  false if it already exists
}

bool OrderStorage::deleteOrder(OrderID_tp orderID)
{
    return orders.erase(orderID) > 0;
}

bool OrderStorage::updateOrder(const Order& updatingOrder)
{
    auto it = orders.find(updatingOrder.getID());
    if (it == orders.end()) {
        return false;
    }
    it->second = updatingOrder;
    return true;
}

bool OrderStorage::isValidOrder(OrderID_tp orderID)
{
    auto it = orders.find(orderID);
    return it != orders.end();
}

bool OrderStorage::addItem(OrderID_tp orderID, MenuID_tp menuID, ItemID_tp itemID, double quantity)
{
    auto it = orders.find(orderID);
    if (it == orders.end()) {
        return false;
    }
    return it->second.addItem(menuID, itemID, quantity);
}

bool OrderStorage::removeItem(OrderID_tp orderID, ItemID_tp itemID)
{
    auto it = orders.find(orderID);
    if (it == orders.end()) {
        return false;
    }
    return it->second.removeItem(itemID);
}

OrderStatus OrderStorage::getOrderStatus(OrderID_tp orderID)
{
    auto it = orders.find(orderID);
    if (it == orders.end()) {
        return OrderStatus::Cancelled;
    }
    return it->second.getOrderStatus();
}

map<OrderID_tp, Order> OrderStorage::giveAllOrders() const
{
    return orders;
}
