#include "Domain/Customer.h"
#include "Repository/OrderStorage.h"
#include "Repository/RestaurantStorage.h"

Customer::Customer() {}
Customer::Customer(string initName, const vector<OrderID_tp> &initOrder) :
 name(initName), myOrders(initOrder) {}
Customer::Customer(CustID_tp initID, string initName, const vector<OrderID_tp> &initOrder) :
 id(initID), name(initName), myOrders(initOrder) {}

void Customer::orderOut(OrderID_tp orderID)
{
    myOrders.push_back(orderID);
}
MenuID_tp Customer::getMenuID(RestID_tp restaurantID) const
{
    RestaurantStorage rstorage;
    return rstorage.getMenuID(restaurantID);
}

string Customer::getName() const
{
    return name;
}
const vector<OrderID_tp>& Customer::getMyOrders() const
{
    return myOrders;
}

bool Customer::deleteFromMyOrder(OrderID_tp orderID, ItemID_tp menuItemID)
{
    if (find(myOrders.begin(), myOrders.end(), orderID) == myOrders.end()) {
        return false;
    }

    OrderStorage storage;
    return storage.removeItem(orderID, menuItemID);
}

bool Customer::addToMyOrder (OrderID_tp orderID, MenuID_tp menuID, ItemID_tp itemID, double quantity)
{
    if (find(myOrders.begin(), myOrders.end(), orderID) == myOrders.end()) {
        return false;
    }

    OrderStorage storage;
    return storage.addItem(orderID, menuID, itemID, quantity);
}

bool Customer::isReady (OrderID_tp orderID) const
{
    if (find(myOrders.begin(), myOrders.end(), orderID) == myOrders.end()) {
        return false;
    }

    OrderStorage storage;
    if (!storage.isValidOrder(orderID)) {
        return false;
    }

    return orderStatus2IsPrepared(storage.getOrderStatus(orderID));
}

vector<Order> Customer::historyOfOrders() const
{
    vector<Order> orders;
    OrderStorage storage;

    for (const auto& orderID : myOrders) {
        Order order;
        if (storage.giveOrder(orderID, order)) {
            orders.push_back(order);
        }
    }

    return orders;
}

CustID_tp Customer::getID() const
{
    return id;
}
Customer::~Customer() {}

Customer& Customer::operator=(const Customer& other)
{
    if (this != &other)
    {
        id = other.id;
        name = other.name;
        myOrders = other.myOrders;
    }
    return *this;
}
