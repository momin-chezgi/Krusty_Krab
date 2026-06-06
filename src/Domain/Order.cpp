# include "Domain/Order.h"
# include "Repository/MenuStorage.h"


Order::Order(const Order& o)
{
    copyFrom(o);
}
Order::Order(CustID_tp oID, const vector<OrderLine> &initOrder) :
    ordererID(oID)
{
    for (const auto& line : initOrder) {
        if (line.first) {
            order.push_back({line.first->clone(), line.second});
        }
    }
}

OrderID_tp Order::getID() const
{
    return id;
}
Order::~Order()
{
    clear();
}
// (Arabic)'Fa taamal jaiiedan' that it is a good way to return a pointer
// to the orderer ? 
// (which ables the accesser to modify or see inside the customer)
CustID_tp Order::getOrderer() const
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

void Order::setOrderer(CustID_tp newOrdererID)
{
    ordererID = newOrdererID;
}

void Order::setOrderStatus(OrderStatus newStatus)
{
    orderStatus = newStatus;
}

void Order::copyFromOrder(const Order& newOrder)
{
    clear();
    copyFrom(newOrder);
}

Order& Order::operator=(const Order& other)
{
    if(this != &other){
        clear();
        copyFrom(other);
    }
    return *this;
}

void Order::clear()
{
    for (auto &line : order) {
        delete line.first;
    }
    order.clear();
}

void Order::copyFrom(const Order& other)
{
    id = other.id;
    ordererID = other.ordererID;
    orderStatus = other.orderStatus;
    for (const auto& line : other.order) {
        if (line.first) {
            order.push_back({line.first->clone(), line.second});
        }
    }
}

bool Order::addItem(MenuID_tp menuID,
    ItemID_tp itemID, 
    double quantity
){
    auto iter = find_if(order.begin(), order.end(), [itemID](const OrderLine &line)
                        { return line.first && itemID == line.first->getID(); });
    if (iter != order.end())
    {
        iter->second += quantity;
        return false;
    }
    OrderLine line;
    MenuStorage mstorage;
    if (!mstorage.has(menuID, itemID)) {
        return false;
    }
    auto newItem = mstorage.cloneItem(itemID);
    if (!newItem) {
        return false;
    }
    line.first = newItem;
    line.second = quantity;
    order.push_back(line);
    return true;
}

bool Order::removeItem(ItemID_tp menuItemID)
{
    auto iter = find_if(order.begin(), order.end(), [menuItemID](const OrderLine &line)
                      { return line.first && menuItemID == line.first->getID(); });
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
        case Cancelled:
            return "Cancelled";
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
    else if(s == "Cancelled"){
        return Cancelled;
    }
    else{
        throw invalid_argument("Invalid order status string: " + s);
    }
}
