#include "Customer.h"

Customer::Customer() {}
Customer::Customer(string initName, const vector<Order *> &initOrder = {}) : name(initName), myOrders(initOrder) {}

void Customer::order(Order *o)
{
    myOrders.push_back(o);
}
Menu Customer::getMenu(string restaurantID) const
{
    return giveMenu(restaurantID);
}

string Customer::getName() const
{
    return name;
}
const vector<Order*>& Customer::getMyOrders() const
{
    return myOrders;
}

bool Customer::deleteFromMyOrder(string orderID, string menuItemID)
{
    for (auto &order : myOrders)
    {
        if (order->getID() == orderID)
        {
            return order->removeItem(menuItemID);
        }
    }
    return false;
}

bool Customer::addToMyOrder (string orderID, const MenuItem const* mi)
{
    for (auto &order : myOrders){
        if(order->getID() == orderID){
            return order->addItem(mi);
        }
    }
    return false;
}

bool Customer::isReady (string orderID) const
{
    for (auto &order : myOrders){
        if(order->getID() == orderID){
            return orderStatus2IsPrepared(order->getOrderStatus());
        }
    }
    return false;
}

string Customer::getID() const
{
    return id;
}
Customer::~Customer() {}

Customer& Customer::operator=(const Customer& other)
{
    if (this != &other)
    {
        // We don't copy the ID because it is unique for each customer
        name = other.name;
        myOrders = other.myOrders;
    }
    return *this;
}
