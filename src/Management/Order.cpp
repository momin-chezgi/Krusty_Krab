# include "Order.h"

Order::Order(size_t initID) : id(initID){}
Order::Order(size_t initID, const Order const *o) : id(initID)
{
    orderer = o->getOrderer();
    order = o->getOrder();
    orderStatus = o->getOrderStatus();
}
Order::Order(size_t initID, const Customer const *initOrderer, const vector<const MenuItem> &initOrder = {}) :
    id(initID), orderer(initOrderer), order(initOrder)
{}

size_t Order::getID() const
{
    return id;
}

// (Arabic)'Fa taamal jaiiedan' that it is a good way to return a pointer
// to the orderer ? 
// (which ables the accesser to modify or see inside the customer)
Customer *Order::getOrderer() const
{
    return orderer;
}

vector<MenuItem> Order::getOrder() const
{
    return order;
}
string Order::getOrderStatus() const
{
    return orderStatus;
}

void Order::setOrderer(const Customer const *newOrderer)
{
    orderer = *newOrderer;
}
void Order::setOrder(const vector<MenuItem> &newOrder)
{
    order = newOrder;
}
void Order::setOrderStatus(string newStatus)
{
    orderStatus = newStatus;
}
void Order::copyFromOrder(const Order newOrder)
{
    orderer = newOrder.getOrderer();
    order = newOrder.getOrder();
    orderStatus = newOrder.getOrderStatus();
}

bool Order::addItem(const MenuItem const *newItem)
{
    auto iter = find_if(order.begin(), order.end(), [newItem](const MenuItem &mi)
                        { return newItem->getID() == mi.getID(); });
    if (iter != order.end())
    {
        return false;
    }
    order.push_back(*newItem);
    return true;
}
bool Order::removeItem(const MenuItem const *removingItem)
{
    auto iter = find_if(order.begin(), order.end(), [removingItem](const MenuItem &mi)
                      { return removingItem->getID() == mi.getID(); 
                    });
    if(iter == order.end())
    {
        return false;
    }
    order.erase(iter);
    return true;
}

Order::~Order(){}