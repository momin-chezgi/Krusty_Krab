# include "Restaurant.h"
# include "../IO/InOut.h"

Restaurant::Restaurant(   ){}
Restaurant::Restaurant(   Restaurant* r) 
{
    name = r->getName();
    address = r->getAddress();
    activationStatus = r->isActive();
    standardTimeOfPreparation = r->getMinutesFoodPrepare();
    phoneNumber = r->getPhone();
    bio = r->getBio();
    orderIDs = r->getOrderIDs();
}
Restaurant::Restaurant(   string initName, vector<string> initAddress, string initPhoneNumber, string initBio = "", size_t minutesPrepared = 0) :
    name(initName), address(initAddress), phoneNumber(initPhoneNumber), bio(initBio), standardTimeOfPreparation(minutesPrepared){}

Restaurant::~Restaurant(){}

// Getters:-----------------------------------------------------------------
string Restaurant::getID() const
{
    return id;
}
string Restaurant::getName() const
{
    return name;
}
vector<string> Restaurant::getAddress() const
{
    return address;
}

bool Restaurant::isActive() const
{
    return activationStatus;
}
string Restaurant::getPhone() const
{
    return phoneNumber;
}
string Restaurant::getBio() const
{
    return bio;
}
const vector<string> &Restaurant::getOrderIDs() const
{
    return orderIDs;
}
Menu Restaurant::getMenu() const
{
    return menu;
}

// Setters:-------------------------------------------------------
void Restaurant::setName(const string newName)
{
    if(newName != "" || newName != " "){
        name = newName;
    }
}
void Restaurant::setAddress(const vector<string>& newAddress)
{
    if(size(newAddress)!=0){
        address = newAddress;
    }
}
void Restaurant::activate()
{
    activationStatus = true;
}
void Restaurant::deactivate()
{
    activationStatus = false;
}
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

// Orders and Menu:-------------------------------------------------------
// bool Restaurant::AddItemToOrder(string orderID, const MenuItem const* item)
// {
//     auto iter = find_if(orders.begin(), orders.end(), [orderID](const Order &inQ)
//                         { return inQ.getID() == orderID;
//                      });
//     if(iter != orders.end()){
//         return false;
//     }
//     return iter->addItem(item);
// }
// bool Restaurant::RemoveItemFromOrder(string orderID, string itemID)
// {
//     auto iter = find_if(orders.begin(), orders.end(), [orderID](const Order &inQ)
//                         { return orderID == inQ.getID();});
//     if(iter == orders.end()){
//         return false;
//     }
//     return iter->removeItem(itemID);
// }

bool Restaurant::AddOrderToQueue(string newOrderID)
{
    // Check if the given ID is valid or not
    OrderStorage storage;
    if(!storage.isValidOrder(newOrderID)){
        return false;
    }
    // Check whether the order with the given ID is already in the queue or not
    auto  iter = find_if(orderIDs.begin(), orderIDs.end(), [newOrderID](const string &inQ)
                      { return newOrderID == inQ; });
    if( iter != orderIDs.end()){
        return false;
    }
    orderIDs.push_back(newOrderID);
    return true;
}
bool Restaurant::removeOrderFromQueue(string removingOrderID)
{
    auto iter = find_if(orderIDs.begin(), orderIDs.end(), [removingOrderID](const string &inQ)
                        { return removingOrderID == inQ; });
    if( iter == orderIDs.end()){
        return false;
    }
    orderIDs.erase(iter);
    return true;
}

bool Restaurant::addItemToMenu( const MenuItem const *item)
{
    return menu.addItem(item);
}
bool Restaurant::removeItemFromMenu(const MenuItem const *item)
{
    return menu.removeItem(item);
}



