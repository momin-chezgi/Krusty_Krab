#pragma once

# include "Domain/MenuItem.h"
# include "Domain/Enums.h"

bool orderStatus2IsPrepared(OrderStatus s);
string orderStatus2String(OrderStatus s);
OrderStatus orderStatusString2Enum(string s);

class Order{
public:
    Order() = default;
    Order(const Order& o);
    Order(CustID_tp oID, const vector<OrderLine> &initOrder = {});
    // For that time, the correctness of oID(ordererID) is 
    // the responsibility of the caller function,
    //  but we can add some authentication process to it in the future

    OrderID_tp getID() const;
    CustID_tp getOrderer() const;
    vector<OrderLine> getOrder() const;
    OrderStatus getOrderStatus() const;
    cost getTotalPrice() const;

    void setOrderer(CustID_tp newOrdererID);
    void setOrderStatus(OrderStatus newStatus);
    void copyFromOrder(const Order newOrder);
    Order& operator=(const Order& newOrder);
    // There isn't setOrderCost, because it's calculation depends on vector<OrderLine>

    bool addItem(MenuID_tp menuID, ItemID_tp itemID, double quantity=1);       
    // returns true if it hasn't been in the order and the quantity is on the kitchen
    bool removeItem(ItemID_tp menuItemID);
    // returns true if it has been in the order and the removing quantity isn't more than the ordered quantity

    ~Order();
private:
    OrderID_tp id = IDGenerator::uuid();
    CustID_tp ordererID;
    vector<OrderLine> order;
    OrderStatus orderStatus {OrderStatus::InPreparation};
};
