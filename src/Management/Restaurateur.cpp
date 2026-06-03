# include "Restaurateur.h"

Restaurateur::Restaurateur(string initRestaurantID, string initName) :
    name(initName) 
{
    RestaurantStorage storage;
    if(storage.isValidRestaurant(initRestaurantID)){
        restaurantID = initRestaurantID;
    }
    else{
        restaurantID = "NotFound";
    }
}
Restaurateur::~Restaurateur(){}


Restaurateur& Restaurateur::operator=(const Restaurateur& newRestaurateur)
{
    if(this != &newRestaurateur){
        restaurantID = newRestaurateur.restaurantID;
        name = newRestaurateur.name;
        saleStatisics = newRestaurateur.saleStatisics;
        customerStatistics = newRestaurateur.customerStatistics;
    }
    return *this;
}


// Getters and Setters:---------------------------------
    
// Itself:

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
    if(newName != "" && newName != " "){
        name = newName;
    }
}

// Restaurant:


string Restaurateur::getRestaurantID() const
{
    return restaurantID;
}

string Restaurateur::getRestaurantName() const
{
    RestaurantStorage storage;
    return storage.getName(restaurantID);
}

string Restaurateur::getRestaurantAddress() const
{
    RestaurantStorage storage;
    return storage.getAddress(restaurantID);
}

bool Restaurateur::getRestaurantStatus() const
{
    RestaurantStorage storage;
    return storage.getStatus(restaurantID);
}

size_t Restaurateur::getRestaurantPreparationMinutes() const
{
    RestaurantStorage storage;
    return storage.getPreparationMinutes(restaurantID);
}

string Restaurateur::getRestaurantPhone() const
{
    RestaurantStorage storage;
    return storage.getPhoneNumber(restaurantID);
}

string Restaurateur::getRestaurantBio() const
{
    RestaurantStorage storage;
    return storage.getBio(restaurantID);
}


bool Restaurateur::setRestaurantID(string newRestaurantID)
{
    RestaurantStorage storage;
    if(!storage.isValidRestaurant(newRestaurantID)){
        return false;
    }
    restaurantID = newRestaurantID;
    return true;
}

bool Restaurateur::editRestaurantName(string newName)
{
    RestaurantStorage storage;
    return storage.setName(restaurantID, newName);
}

bool Restaurateur::editRestaurantAddress(string newAddress)
{
    RestaurantStorage storage;
    return storage.setAddress(restaurantID, newAddress);
}

bool Restaurateur::activateRestaurant()
{
    RestaurantStorage storage;
    return storage.activateRestaurant(restaurantID);
}

bool Restaurateur::deactivateRestaurant()
{
    RestaurantStorage storage;
    return storage.deactivateRestaurant(restaurantID);
}

bool Restaurateur::setPreparationTime(const size_t minutes)
{
    RestaurantStorage storage;
    return storage.setPreparationTime(restaurantID, minutes);
}

bool Restaurateur::setPhoneNumber(const string newPhoneNumber)
{
    RestaurantStorage storage;
    return storage.setPhoneNumber(restaurantID, newPhoneNumber);
}

bool Restaurateur::setBio(const string newBio)
{
    RestaurantStorage storage;
    return storage.setBio(restaurantID, newBio);
}



// Menu:

string Restaurateur::getMenuID() const
{
    RestaurantStorage storage;
    return storage.getMenuID(restaurantID);
}

bool Restaurateur::addItemToMenu(const MenuItem& item)
{
    MenuStorage storage;
    RestaurantStorage rStorage;

    string menuID = rStorage.getMenuID(restaurantID);
    return storage.addItem(menuID, item);

}
bool Restaurateur::removeItemFromMenu(string itemID)
{
    MenuStorage storage;
    RestaurantStorage rStorage;

    string menuID = rStorage.getMenuID(restaurantID);
    return storage.deleteItem(menuID, itemID);
}
bool Restaurateur::replaceItemInMenu(string previousItemID, const MenuItem& replacedItem)
{
    MenuStorage storage;
    RestaurantStorage rStorage;
    
    string menuID = rStorage.getMenuID(restaurantID);
    
    if(replacedItem.getID() == previousItemID) return false;
    if(storage.has(menuID, replacedItem.getID())) return false;
    if(!storage.has(menuID, previousItemID)) return false; 
    
    return storage.addItem(menuID,replacedItem) && storage.deleteItem(menuID, previousItemID);
}

// Order:

vector<string> Restaurateur::getOrderIDs() const
{
    RestaurantStorage storage;
    return storage.getOrderIDs(restaurantID);
}

bool Restaurateur::addItemToOrder(string orderID, const MenuItem& item, double quantity)
{
    OrderStorage storage;
    return storage.addItem(orderID, item, quantity);
}
bool Restaurateur::removeItemFromOrder(string orderID, string itemID)
{
    OrderStorage storage;
    return storage.removeItem(orderID, itemID);
}


// Queue:

bool Restaurateur::addOrderToQueue(string orderID)
{
    RestaurantStorage storage;
    return storage.addOrderToRestaurant(restaurantID, orderID);
}
bool Restaurateur::removeOrderFromQueue(string orderID)
{
    RestaurantStorage storage;
    return storage.deleteOrderFromRestaurant(restaurantID, orderID);
}
bool Restaurateur::replaceOrderInQueue(string previousOrderID, string newOrderID)
{
    OrderStorage storage;
    RestaurantStorage rStorage;
    if(!storage.isValidOrder(newOrderID)) return false;
    if(!storage.isValidOrder(previousOrderID)) return false;
    // if provious order is in queue and the new order, too:
    if(rStorage.orderIsInQueue(restaurantID, previousOrderID) && rStorage.orderIsInQueue(restaurantID, newOrderID)) return false;
    return rStorage.deleteOrderFromRestaurant(restaurantID, previousOrderID) && rStorage.addOrderToRestaurant(restaurantID, newOrderID);
}
// statistics:

bool Restaurateur::updateAndPrintSaleStatistics(){}
bool Restaurateur::updateAndPrintCustomerStatistics(){}