# include "Order.h"

Order::Order(   ){}
Order::Order(   const Order const *o)
{
    orderer = o->getOrderer();
    order = o->getOrder();
    orderStatus = o->getOrderStatus();
}
Order::Order(   const Customer const *initOrderer, const vector<MenuItem> &initOrder = {}) :
    orderer((Customer *)initOrderer), order(initOrder)
{}

string Order::getID() const
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
OrderStatus Order::getOrderStatus() const
{
    return orderStatus;
}

void Order::setOrderer(const Customer const *newOrderer)
{
    orderer = (Customer *)newOrderer;
}
void Order::setOrder(const vector<MenuItem> &newOrder)
{
    order = newOrder;
}
void Order::setOrderStatus(OrderStatus newStatus)
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
bool Order::removeItem(string menuItemID)
{
    auto iter = find_if(order.begin(), order.end(), [menuItemID](const MenuItem &mi)
                      { return menuItemID == mi.getID(); });
    if(iter == order.end())
    {
        return false;
    }
    order.erase(iter);
    return true;
}

Order::~Order(){}

bool orderStatus2IsPrepared(OrderStatus s)
{
    return s != InPreparation;
}
string orderStatus2String(OrderStatus s)
{
    switch(s)
    {
        case InPreparation:
            return "In-Preparation";
        case ReadyToSend:
            return "Ready-To-Send";
        case Delivered:
            return "Delivered";
        default:
            return "Unknown Status";
    }
}
OrderStatus orderStatusString2Enum(string s)
{
    if(s == "In-Preparation"){
        return InPreparation;
    }
    else if(s == "Ready-To-Send"){
        return ReadyToSend;
    }
    else if(s == "Delivered"){
        return Delivered;
    }
    else{
        throw invalid_argument("Invalid order status string: " + s);
    }
}