# include "Domain/Restaurant.h"
# include "Repository/MenuStorage.h"
# include "Repository/OrderStorage.h"

Restaurant::Restaurant(Restaurant* r) 
{
    name = r->getName();
    address = r->getAddress();
    activationStatus = r->isActive();
    standardTimeOfPreparation = r->getMinutesFoodPrepare();
    phoneNumber = r->getPhone();
    bio = r->getBio();
    orderIDs = r->getOrderHistoryIDs();
    menuID = r->getMenuID();
}

Restaurant::Restaurant(string initMenuID,
     string initName,
     string initAddress,
     string initPhoneNumber,
     string initBio,
     size_t minutesPrepared) :
    name(initName), 
    address(initAddress),
    standardTimeOfPreparation(minutesPrepared),
    phoneNumber(initPhoneNumber),
    bio(initBio),
    menuID(initMenuID)
{}

Restaurant::Restaurant(
     RestID_tp initID,
     string initMenuID,
     string initName,
     string initAddress,
     string initPhoneNumber,
     string initBio,
     size_t minutesPrepared) :
    id(initID),
    name(initName),
    address(initAddress),
    standardTimeOfPreparation(minutesPrepared),
    phoneNumber(initPhoneNumber),
    bio(initBio),
    menuID(initMenuID)
{}

Restaurant::Restaurant(
     RestID_tp initID,
     string initMenuID,
     string initName,
     string initAddress,
     string initPhoneNumber,
     string initBio,
     size_t minutesPrepared,
     const vector<OrderID_tp>& initOrderIDs) :
    id(initID),
    name(initName),
    address(initAddress),
    standardTimeOfPreparation(minutesPrepared),
    phoneNumber(initPhoneNumber),
    bio(initBio),
    orderIDs(initOrderIDs),
    menuID(initMenuID)
{}

Restaurant::~Restaurant(){}



// Getters:-----------------------------------------------------------------

RestID_tp Restaurant::getID() const {return id;}

string Restaurant::getName() const {return name;}

string Restaurant::getAddress() const {return address;}

bool Restaurant::isActive() const {return activationStatus;}

size_t Restaurant::getMinutesFoodPrepare() const {return standardTimeOfPreparation;}

string Restaurant::getPhone() const {return phoneNumber;}

string Restaurant::getBio() const   {return bio;}

const vector<OrderID_tp> &Restaurant::getOrderHistoryIDs() const   {return orderIDs;}

MenuID_tp Restaurant::getMenuID() const    {return menuID;}



// Setters:-------------------------------------------------------
void Restaurant::setName(const string newName)
{
    if(newName != "" || newName != " "){
        name = newName;
    }
}

void Restaurant::setAddress(const string& newAddress)
{
    if(newAddress.size() != 0){
        address = newAddress;
    }
}

void Restaurant::activate() {activationStatus = true;}

void Restaurant::deactivate()   {activationStatus = false;}

void Restaurant::setPreparationTime(const size_t minutes)
{
    standardTimeOfPreparation = minutes;
}

void Restaurant::setPhoneNumber(const string newPhoneNumber)
{
    if(newPhoneNumber != "" || newPhoneNumber != " "){
        phoneNumber = newPhoneNumber;
    }
}

void Restaurant::setBio(const string newBio)
{
    if(newBio != "" || newBio != " "){
        bio = newBio;
    }
}



// Order:-----------------------------------------------------------------

bool Restaurant::AddItemToOrder(OrderID_tp orderID ,
    MenuID_tp menuID ,
    ItemID_tp itemID,
    double quantity
){ 
    OrderStorage ostorage;
    if(!ostorage.isValidOrder(orderID)) return false;
    MenuStorage mstorage;
    if(!mstorage.has(menuID,itemID))   return false;
    return ostorage.addItem(orderID, menuID, itemID, quantity);
}

bool Restaurant::RemoveItemFromOrder(OrderID_tp orderID, ItemID_tp itemID)
{
    OrderStorage ostorage;
    if(!ostorage.isValidOrder(orderID)) return false;
    return ostorage.removeItem(orderID, itemID);
}

// Queue:-----------------------------------------------------------------

bool Restaurant::AddOrderToQueue(OrderID_tp newOrderID)
{
    // Check if the given ID is valid or not
    OrderStorage storage;
    if(!storage.isValidOrder(newOrderID)){
        return false;
    }
    // Check whether the order with the given ID is already in the queue or not
    auto  iter = find_if(orderIDs.begin(), orderIDs.end(), [newOrderID](const OrderID_tp &inQ)
                      { return newOrderID == inQ; });
    if( iter != orderIDs.end()){
        return false;
    }
    orderIDs.push_back(newOrderID);
    return true;
}

bool Restaurant::removeOrderFromQueue(OrderID_tp removingOrderID)
{
    auto iter = find_if(orderIDs.begin(), orderIDs.end(), [removingOrderID](const OrderID_tp &inQ)
                        { return removingOrderID == inQ; });
    if( iter == orderIDs.end()){
        return false;
    }
    orderIDs.erase(iter);
    return true;
}

bool Restaurant::orderIsInQueue(OrderID_tp orderID) const
{
    auto iter = find_if(orderIDs.begin(), orderIDs.end(), [orderID](const OrderID_tp &inQ)
                        { return orderID == inQ; });
    return iter != orderIDs.end();
}



// Menu:----------------------------------------------------------------

bool Restaurant::addItemToMenu(const MenuItem *item)
{
    MenuStorage mstorage;
    return mstorage.addItem(menuID, item);
}

bool Restaurant::removeItemFromMenu(ItemID_tp itemID)
{
    MenuStorage mstorage;
    return mstorage.deleteItem(menuID, itemID);
}
