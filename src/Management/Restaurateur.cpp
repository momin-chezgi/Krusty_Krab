# include "Restaurateur.h"

Restaurateur::Restaurateur(string initName="") : name(initName){}

Restaurateur::Restaurateur(const Restaurant& initRestaurant, string initName="") :
    name(initName), restaurantID(initRestaurant.getID()){}

Restaurateur& Restaurateur::operator=(const Restaurateur& newRestaurateur)
{
    if(this != &newRestaurateur){
        restaurant = newRestaurateur.restaurant;
        name = newRestaurateur.name;
        saleStatisics = newRestaurateur.saleStatisics;
        customerStatistics = newRestaurateur.customerStatistics;
    }
    return *this;
}

string Restaurateur::myRestaurant() const
{
    return restaurantID;
}

string Restaurateur::getID() const
{
    return id;
}
string Restaurateur::getName() const
{
    return name;
}

void Restaurateur::setName(string newName)
{
    if(newName != "" || newName != " "){
        name = newName;
    }
}

void Restaurateur::editRestaurantName(string newName)
{
    RestaurantStorage storage;
    storage.setName(restaurantID, newName);
}
void Restaurateur::editRestaurantAddress(vector<string> newAddress)
{
    RestaurantStorage storage;
    storage.setAddress(restaurantID, newAddress);
}
void Restaurateur::activateRestaurant()
{
    RestaurantStorage storage;
    storage.activateRestaurant(restaurantID);
}
void Restaurateur::deactivateRestaurant()
{
    RestaurantStorage storage;
    storage.deactivateRestaurant(restaurantID);
}


bool Restaurateur::addItemToMenu(const MenuItem const* mi)
{

}
bool Restaurateur::removeItemFromMenu(MenuItem previousOrder, MenuItem replacedOrder){}
bool Restaurateur::replaceItemFromMenu(MenuItem previousItem, MenuItem replacedItem){}

bool Restaurateur::editThisOrder(Order *previousOrderptr, Order replacedOrder);

void Restaurateur::updateAndPrintSaleStatistics(){}
void Restaurateur::updateAndPrintCustomerStatistics(){}

bool Restaurateur::AddItemToOrder(string orderID, const MenuItem const* item)
{
    return restaurant->AddItemToOrder(orderID, item);
}
bool Restaurateur::RemoveItemFromOrder(string orderID, string itemID)
{
    return restaurant->RemoveItemFromOrder(orderID, itemID);
}

bool Restaurateur::addItemToMenu( const MenuItem const *item)
{
    return restaurant->addItemToMenu(item);
}
bool Restaurateur::removeItemFromMenu( const MenuItem const * item)
{
    return restaurant->removeItemFromMenu(item);
}

bool Restaurateur::AddOrderToQueue(string orderID)
{
    return restaurant->AddOrderToQueue(orderID);
}
bool Restaurateur::removeOrderFromQueue(string orderID)
{
    return restaurant->removeOrderFromQueue(orderID);
}

Restaurateur::~Restaurateur(){}
