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
    orderIDs = r->getOrderIDs();
}

Restaurant::Restaurant(string initName,
     string initAddress,
     string initPhoneNumber,
     string initBio,
     size_t minutesPrepared) :
    name(initName), 
    address(initAddress),
    phoneNumber(initPhoneNumber),
    bio(initBio),
    standardTimeOfPreparation(minutesPrepared)
{}

Restaurant::~Restaurant(){}



// Getters:-----------------------------------------------------------------

string Restaurant::getName() const {return name;}

string Restaurant::getAddress() const {return address;}

bool Restaurant::isActive() const {return activationStatus;}

size_t Restaurant::getMinutesFoodPrepare() const {return standardTimeOfPreparation;}

string Restaurant::getPhone() const {return phoneNumber;}

string Restaurant::getBio() const   {return bio;}

const vector<OrderID_tp> &Restaurant::getOrderIDs() const   {return orderIDs;}

Menu Restaurant::getMenu() const    {return menu;}



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
    MenuStorage mstorage;
    if(!ostorage.isValidOrder(orderID)) return false;
    if(!mstorage.has(menuID,itemID))   return false;
    ostorage.addItem(orderID, mstorage., quantity)
}

bool Restaurant::RemoveItemFromOrder(OrderID_tp orderID, ItemID_tp itemID)
{

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

bool Restaurant::addItemToMenu( const MenuItem const *item)
{
    return menu.addItem(item);
}

bool Restaurant::removeItemFromMenu(const MenuItem const *item)
{
    return menu.removeItem(item);
}


