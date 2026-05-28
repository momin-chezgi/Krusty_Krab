# include "Restaurateur.h"

Restaurateur::Restaurateur(size_t newID) : id(newID){}

Restaurateur::Restaurateur(size_t newID, const Restaurant const *initRestaurant, string initName="") :
    id(newID), restaurant(initRestaurant) , name(initName){}


Restaurant *Restaurateur::myRestaurant() const
{
    return restaurant;
}
size_t Restaurateur::getRestaurantID() const
{
    return restaurant->getID();
}
string Restaurateur::getRestaurntName() const
{
    return restaurant->getName();
}
vector<string> Restaurateur::getRestaurantAddress() const
{
    return restaurant->getAddress();
}
bool Restaurateur::getRestaurantStatus() const
{
    return restaurant->isActive();
}
size_t Restaurateur::getRestaurantPreparationMinutes() const
{
    return restaurant->getMinutesFoodPrepare();
}
Menu Restaurateur::getMenu() const
{
    return restaurant->getMenu();
}
vector<Order> Restaurateur::getOrders() const
{
    return restaurant->getOrders();
}
size_t Restaurateur::getID() const
{
    return id;
}
string Restaurateur::getName() const
{
    return name;
}

void Restaurateur::setID(size_t newID)
{
    if(IDInitializer::Restaurateur(1, newID)==1){
        id = newID;
    }
}
void Restaurateur::setName(string newName)
{
    if(newName != "" || newName != " "){
        name = newName;
    }
}

// void Restaurateur::editRestaurantID(size_t newID)
// {
//     restaurant->
// }
void Restaurateur::editRestaurantName(string newName)
{
    if(newName != "" || newName != " "){
        restaurant->setName(newName);
    }
}
void Restaurateur::editRestaurantAddress(vector<string> newAddress)
{
    restaurant->setAddress(newAddress);
}
void Restaurateur::activateRestaurant()
{
    restaurant->activate();
}
void Restaurateur::deactivateRestaurant()
{
    restaurant->deactivate();
}
// bool Restaurateur::addItemToMenu(const MenuItem const* mi)
// {

// }
// bool Restaurateur::removeItemFromMenu(MenuItem previousOrder, MenuItem replacedOrder){}
// bool Restaurateur::replaceItemFromMenu(MenuItem previousItem, MenuItem replacedItem){}

// bool Restaurateur::editThisOrder(Order *previousOrderptr, Order replacedOrder);

void Restaurateur::updateAndPrintSaleStatistics(){}
void Restaurateur::updateAndPrintCustomerStatistics(){}

bool Restaurateur::AddItemToOrder(size_t orderID, const MenuItem const* item)
{
    return restaurant->AddItemToOrder(orderID, item);
}
bool Restaurateur::RemoveItemFromOrder(size_t orderID, const MenuItem const* item)
{
    return restaurant->RemoveItemFromOrder(orderID, item);
}

bool Restaurateur::addItemToMenu( const MenuItem const *item)
{
    return restaurant->addItemToMenu(item);
}
bool Restaurateur::removeItemFromMenu( const MenuItem const * item)
{
    return restaurant->removeItemFromMenu(item);
}

bool Restaurateur::AddOrderToQueue(const Order newOrder)
{
    return restaurant->AddOrderToQueue(newOrder);
}
bool Restaurateur::removeOrderFromQueue(const Order removingOrder)
{
    return restaurant->removeOrderFromQueue(removingOrder);
}

Restaurateur::~Restaurateur(){}
