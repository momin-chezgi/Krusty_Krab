# include "InOut.h"
# include "../Management/Food.h"
# include "../Management/Drink.h"

OrderLine seedOrderLine(const MenuItem& item, double quantity)
{
    OrderLine line;
    line.first = item.clone();
    line.second = quantity;
    return line;
}
map<string, Order> OrderStorage::orders = {
    {"TestOrder", Order("TestOrder", {seedOrderLine(Food("TestMenuItem", 10.0, 20.0), 1.0)})}
};
map<string, Menu> MenuStorage::menus = {
    {"TestMenu", Menu({Drink("TestMenuItem", 10.0, 20.0)})}
};
map<string, Restaurateur> RestaurateurStorage::restaurateurs = {
    {"TestRestaurateur", Restaurateur("TestRestaurateur")}
};
map<string, Restaurant> RestaurantStorage::restaurants = {
    {"TestRestaurant", Restaurant("TestRestaurant", {"TestCity", "TestStreet", "TestBuilding"}, "1234567890")}
};
map<string, AdminOfSystem> AdminStorage::admins = {
    {"TestAdmin", AdminOfSystem({"TestRestaurateur"}, "TestAdmin")}
};


Customer CustomerStorage::giveCustomer(string customerID)
{
    auto it = customers.find(customerID);
    if (it != customers.end()) {
        return it->second;
    }
    return Customer("NotFound"); 
    // We can't use [] operator to find in map 
    // because if the key doesn't exist, 
    // it will create a new element with default value, 
    // but we want to return a default customer with 'NotFound' name 
    // and no orders 
}

bool CustomerStorage::isValidCustomer(string customerID)
{
    auto it = customers.find(customerID);
    return it != customers.end();
}
bool RestaurantStorage::isValidRestaurant(string restaurantID)
{
    auto it = restaurants.find(restaurantID);
    return it != restaurants.end();
}
bool AdminStorage::isValidAdmin(string adminID)
{
    auto it = admins.find(adminID);
    return it != admins.end();
}
bool MenuStorage::isValidMenu(string menuID)
{
    auto it = menus.find(menuID);
    return it != menus.end();
}
bool OrderStorage::isValidOrder(string orderID)
{
    auto it = orders.find(orderID);
    return it != orders.end();
}

Menu MenuStorage::giveMenu(string menuID)
{
    auto it = menus.find(menuID);
    if (it != menus.end()) {
        return it->second;
    }
    return Menu({}); 
    // We can't use [] operator to find in map 
    // because if the key doesn't exist, 
    // it will create a new element with default value, 
    // but we want to return a default menu with no items 
}

bool OrderStorage::saveOrder(const Order& newOrder)
{
    auto result = orders.insert({newOrder.getID(), newOrder});
    return result.second; 
    // returns true if the order was inserted,
    //  false if it already exists
}

bool RestaurantStorage::addOrderToRestaurant(string restaurantID, string orderID)
{
    auto it = restaurants.find(restaurantID);
    if (it != restaurants.end()) { 
        return it->second.AddOrderToQueue(orderID);
    }
    return false;
}

bool RestaurantStorage::orderIsInQueue(string restaurantID, string orderID)
{
    auto it = restaurants.find(restaurantID);
    if (it != restaurants.end()) { 
        return it->second.orderIsInQueue(orderID);
    }
    return false;
}

Restaurateur RestaurateurStorage::giveRestaurateur(string restaurateurID)
{
    auto it = restaurateurs.find(restaurateurID);
    if (it != restaurateurs.end()) {
        return it->second;
    }
    return Restaurateur("NotFound"); 
}



bool RestaurantStorage::setName(string restaurantID, string newName)
{
    auto it = restaurants.find(restaurantID);
    if (it != restaurants.end()){
        it->second.setName(newName);
        return true;
    }
    return false;
}

bool RestaurantStorage::setAddress(string restaurantID, string newAddress)
{
    auto it = restaurants.find(restaurantID);
    if (it != restaurants.end()){
        it->second.setAddress(newAddress);
        return true;
    }
    return false;   
}

bool RestaurantStorage::activateRestaurant(string restaurantID)
{
    auto it = restaurants.find(restaurantID);
    if (it != restaurants.end()){
        it->second.activate();
        return true;
    }
    return false;
}

bool RestaurantStorage::deactivateRestaurant(string restaurantID)
{
    auto it = restaurants.find(restaurantID);
    if (it != restaurants.end()){
        it->second.deactivate();
        return true;
    }
    return false;
}

vector<string> RestaurantStorage::getOrderIDs(string restaurantID) const
{
    auto it = restaurants.find(restaurantID);
    if (it != restaurants.end()){
        return it->second.getOrderIDs();
    }
    return vector<string>{};
}
