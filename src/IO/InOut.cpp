# include "InOut.h"

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