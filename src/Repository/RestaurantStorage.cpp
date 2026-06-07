
#include "Repository/RestaurantStorage.h"


map<RestID_tp, Restaurant> RestaurantStorage::restaurants = {
    {"TestRestaurant", Restaurant("TestMenu", "TestRestaurant", "TestCity/TestStreet/TestBuilding", "1234567890", "Seed restaurant", 20)}
};

string RestaurantStorage::getName(RestID_tp restaurantID) const
{
    auto it = restaurants.find(restaurantID);
    return it != restaurants.end() ? it->second.getName() : "NotFound";
}

string RestaurantStorage::getAddress(RestID_tp restaurantID) const
{
    auto it = restaurants.find(restaurantID);
    return it != restaurants.end() ? it->second.getAddress() : "NotFound";
}

bool RestaurantStorage::getStatus(RestID_tp restaurantID) const
{
    auto it = restaurants.find(restaurantID);
    return it != restaurants.end() && it->second.isActive();
}

size_t RestaurantStorage::getPreparationMinutes(RestID_tp restaurantID) const
{
    auto it = restaurants.find(restaurantID);
    return it != restaurants.end() ? it->second.getMinutesFoodPrepare() : 0;
}

string RestaurantStorage::getPhoneNumber(RestID_tp restaurantID) const
{
    auto it = restaurants.find(restaurantID);
    return it != restaurants.end() ? it->second.getPhone() : "NotFound";
}

string RestaurantStorage::getBio(RestID_tp restaurantID) const
{
    auto it = restaurants.find(restaurantID);
    return it != restaurants.end() ? it->second.getBio() : "NotFound";
}

MenuID_tp RestaurantStorage::getMenuID(RestID_tp restaurantID) const
{
    auto it = restaurants.find(restaurantID);
    return it != restaurants.end() ? it->second.getMenuID() : "NotFound";
}

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

Restaurant RestaurantStorage::giveRestaurant(RestID_tp restaurantID)
{
    auto it = restaurants.find(restaurantID);
    if (it != restaurants.end()) {
        return it->second;
    }
    return Restaurant();
}

bool RestaurantStorage::deleteOrderFromRestaurant(RestID_tp restaurantID, OrderID_tp orderID)
{
    auto it = restaurants.find(restaurantID);
    if (it == restaurants.end()) {
        return false;
    }
    return it->second.removeOrderFromQueue(orderID);
}

RestID_tp RestaurantStorage::saveRestaurant(const Restaurant& newRestaurant)
{
    RestID_tp newID = newRestaurant.getID();
    if (newID.empty()) {
        return "";
    }
    auto result = restaurants.insert({newID, newRestaurant});
    return result.second ? newID : "";
}

bool RestaurantStorage::deleteRestaurant(RestID_tp restaurantID)
{
    return restaurants.erase(restaurantID) > 0;
}

bool RestaurantStorage::replaceRestaurant(RestID_tp prerestaurantID, const Restaurant& updatingRestaurant)
{
    auto it = restaurants.find(prerestaurantID);
    if (it == restaurants.end()) {
        return false;
    }
    it->second = updatingRestaurant;
    return true;
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

bool RestaurantStorage::setPreparationTime(RestID_tp restaurantID, const size_t minutes)
{
    auto it = restaurants.find(restaurantID);
    if (it != restaurants.end()){
        it->second.setPreparationTime(minutes);
        return true;
    }
    return false;
}

bool RestaurantStorage::setPhoneNumber(RestID_tp restaurantID, const string newPhoneNumber)
{
    auto it = restaurants.find(restaurantID);
    if (it != restaurants.end()){
        it->second.setPhoneNumber(newPhoneNumber);
        return true;
    }
    return false;
}

bool RestaurantStorage::setBio(RestID_tp restaurantID, const string newBio)
{
    auto it = restaurants.find(restaurantID);
    if (it != restaurants.end()){
        it->second.setBio(newBio);
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

map<RestID_tp, Restaurant> RestaurantStorage::giveAllRestaurants() const
{
    return restaurants;
}
