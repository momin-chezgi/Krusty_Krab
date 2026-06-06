
#include "Repository/RestaurantStorage.h"


map<RestID_tp, Restaurant> RestaurantStorage::restaurants = {
    {"TestRestaurant", Restaurant("TestRestaurant", {"TestCity", "TestStreet", "TestBuilding"}, "1234567890")}
};


bool RestaurantStorage::isValidRestaurant(RestID_tp restaurantID)
{
    auto it = restaurants.find(restaurantID);
    return it != restaurants.end();
}
bool RestaurantStorage::addOrderToRestaurant(RestID_tp restaurantID, OrderID_tp orderID)
{
    auto it = restaurants.find(restaurantID);
    if (it != restaurants.end()) { 
        return it->second.AddOrderToQueue(orderID);
    }
    return false;
}

bool RestaurantStorage::orderIsInQueue(RestID_tp restaurantID, OrderID_tp orderID)
{
    auto it = restaurants.find(restaurantID);
    if (it != restaurants.end()) { 
        return it->second.orderIsInQueue(orderID);
    }
    return false;
}




bool RestaurantStorage::setName(RestID_tp restaurantID, string newName)
{
    auto it = restaurants.find(restaurantID);
    if (it != restaurants.end()){
        it->second.setName(newName);
        return true;
    }
    return false;
}

bool RestaurantStorage::setAddress(RestID_tp restaurantID, string newAddress)
{
    auto it = restaurants.find(restaurantID);
    if (it != restaurants.end()){
        it->second.setAddress(newAddress);
        return true;
    }
    return false;   
}

bool RestaurantStorage::activateRestaurant(RestID_tp restaurantID)
{
    auto it = restaurants.find(restaurantID);
    if (it != restaurants.end()){
        it->second.activate();
        return true;
    }
    return false;
}

bool RestaurantStorage::deactivateRestaurant(RestID_tp restaurantID)
{
    auto it = restaurants.find(restaurantID);
    if (it != restaurants.end()){
        it->second.deactivate();
        return true;
    }
    return false;
}

vector<OrderID_tp> RestaurantStorage::getOrderIDs(RestID_tp restaurantID) const
{
    auto it = restaurants.find(restaurantID);
    if (it != restaurants.end()){
        return it->second.getOrderIDs();
    }
    return vector<OrderID_tp>{};
}
