# pragma once

# include "Common/Types.h"
# include "Domain/Restaurant.h"
# include "Repository/OrderStorage.h"

class RestaurantStorage{
public:
    string getName(RestID_tp restaurantID) const;
    string getAddress(RestID_tp restaurantID) const;
    bool getStatus(RestID_tp restaurantID) const;
    size_t getPreparationMinutes(RestID_tp restaurantID) const;
    string getPhoneNumber(RestID_tp restaurantID) const;
    string getBio(RestID_tp restaurantID) const;
    MenuID_tp getMenuID(RestID_tp restaurantID) const;
    vector<OrderID_tp> getOrderHistoryIDs(RestID_tp restaurantID) const;
    vector<OrderID_tp> getOrderIDs(RestID_tp restaurantID);
    // The difference between getOrderHistoryIDs() and getOrderIds() are 
    //in that latter function returns just InPrepration/ReadyToSend orders
    //but the first one returns all the orders

    bool setName(RestID_tp restaurantID, string newName);
    bool setAddress(RestID_tp restaurantID, string newAddress);
    bool activateRestaurant(RestID_tp restaurantID);
    bool deactivateRestaurant(RestID_tp restaurantID);
    bool setPreparationTime(RestID_tp restaurantID, const size_t minutes);
    bool setPhoneNumber(RestID_tp restaurantID, const string newPhoneNumber);
    bool setBio(RestID_tp restaurantID, const string newBio);
    
    bool isValidRestaurant(RestID_tp restaurantID);

    bool addOrderToRestaurant(RestID_tp restaurantID, OrderID_tp orderID);
    bool deleteOrderFromRestaurant(RestID_tp restaurantID, OrderID_tp orderID);
    bool orderIsInQueue(RestID_tp restaurantID, OrderID_tp orderID);
    
    Restaurant giveRestaurant(RestID_tp restaurantID);

    RestID_tp saveRestaurant(const Restaurant& newRestaurant);  // returns true if the restaurant has been saved successfully, otherwise false
    bool deleteRestaurant(RestID_tp restaurantID);  // returns true if the restaurant with the given ID exists and has been deleted, otherwise returns false
    bool replaceRestaurant(RestID_tp prerestaurantID, const Restaurant& updatingRestaurant);  // returns true if the restaurant with the given ID exists and has been updated, otherwise returns false
    map<RestID_tp, Restaurant> giveAllRestaurants() const;

    static vector<RestID_tp> activeRestaurantList();
private:
    static map<RestID_tp, Restaurant> restaurants;
};
