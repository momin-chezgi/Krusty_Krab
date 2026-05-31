# include "Order.h"


Order::Order(const Order& o)
{
    ordererID = o.getOrderer();
    order = o.getOrder();
    orderStatus = o.getOrderStatus();
}
Order::Order(string oID, const vector<OrderLine> &initOrder = {}) :
    ordererID(oID), order(initOrder)
{}

string Order::getID() const
{
    return id;
}
Order::~Order()
{
    for (auto &line : order)
    {
        delete line.first;
    }
}
// (Arabic)'Fa taamal jaiiedan' that it is a good way to return a pointer
// to the orderer ? 
// (which ables the accesser to modify or see inside the customer)
string Order::getOrderer() const
{
    return ordererID;
}

vector<OrderLine> Order::getOrder() const
{
    return order;
}
OrderStatus Order::getOrderStatus() const
{
    return orderStatus;
}

cost Order::getTotalPrice() const {
    cost totalPrice = 0;
    for(const auto& item : order){
        totalPrice += item.first->getPricePerUnit() * item.second;
    }
    return totalPrice;
}

void Order::setOrderer(string newOrdererID)
{
    ordererID = newOrdererID;
}

void Order::setOrderStatus(OrderStatus newStatus)
{
    orderStatus = newStatus;
}
void Order::copyFromOrder(const Order newOrder)
{
    ordererID = newOrder.getOrderer();
    order = newOrder.getOrder();
    orderStatus = newOrder.getOrderStatus();
}

Order& Order::operator=(const Order& other)
{
    if(this != &other){
        ordererID = other.getOrderer();
        order = other.getOrder();
        orderStatus = other.getOrderStatus();
    }
    return *this;
}

bool Order::addItem(const MenuItem& newItem, double quantity)
{
    string itemID = newItem.getID();
    auto iter = find_if(order.begin(), order.end(), [itemID](const OrderLine &line)
                        { return itemID == line.first->getID(); });
    if (iter != order.end())
    {
        iter->second += quantity;
        return false;
    }
    OrderLine line;
    line.first = newItem.clone();
    line.second = quantity;
    order.push_back(line);
    return true;
}
bool Order::removeItem(string menuItemID)
{
    auto iter = find_if(order.begin(), order.end(), [menuItemID](const OrderLine &line)
                      { return menuItemID == line.first->getID(); });
    if(iter == order.end())
    {
        return false;
    }
    delete iter->first;
    order.erase(iter);
    return true;
}

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
