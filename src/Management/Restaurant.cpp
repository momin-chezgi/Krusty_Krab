# include "Restaurant.h"

Restaurant::Restaurant(size_t initID) : id(initID){}
Restaurant::Restaurant(size_t initID, Restaurant* r) : id(initID)
{
    name = r->getName();
    address = r->getAddress();
    activationStatus = r->isActive();
    standardTimeOfPreparation = r->getMinutesFoodPrepare();
    phoneNumber = r->getPhone();
    bio = r->getBio();
    orders = r->getOrders();
}
Restaurant::Restaurant(size_t initID, string initName, vector<string> initAddress, string initPhoneNumber, string initBio = "", size_t minutesPrepared = 0) :
    id(initID), name(initName), address(initAddress), phoneNumber(initPhoneNumber), bio(initBio), standardTimeOfPreparation(minutesPrepared){}

size_t Restaurant::getID() const
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
// string Restaurant::getAddress() 
// {
//     string strAdress{""};
//     for (auto memb : address){
//         strAdress += memb;
//     }
//     return strAdress;
// }

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
const vector<Order> &Restaurant::getOrders() const
{
    return orders;
}
Menu Restaurant::getMenu() const
{
    return menu;
}

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


bool Restaurant::AddItemToOrder(size_t orderID, const MenuItem const* item)
{
    auto iter = find_if(orders.begin(), orders.end(), [orderID](const Order &inQ)
                        { return inQ.getID() == orderID;
                     });
    if(iter != orders.end()){
        return false;
    }
    return iter.addItem(item);
}
bool Restaurant::RemoveItemFromOrder(size_t orderID, const MenuItem const* item)
{
    auto iter = find_if(orders.begin(), orders.end(), [orderID](const Order &inQ)
                        { return orderID == inQ.getID();
                         });
    if(iter == orders.end()){
        return false;
    }
    return iter.removeItem(item);
}

bool Restaurant::AddOrderToQueue(const Order newOrder)
{
    auto  iter = find_if(orders.begin(), orders.end(), [newOrder](const Order &inQ)
                      { return newOrder.getID() == inQ.getID(); });
    if( iter != orders.end()){
        return false;
    }
    orders.push_back(newOrder);
    return true;
}
bool Restaurant::removeOrderFromQueue(const Order removingOrder)
{
    auto iter = find_if(orders.begin(), orders.end(), [removingOrder](const Order &inQ)
                        { return removingOrder.getID() == inQ.getID();
                         });
    if( iter == orders.end()){
        return false;
    }
    orders.erase(iter);
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


Restaurant::~Restaurant(){}

