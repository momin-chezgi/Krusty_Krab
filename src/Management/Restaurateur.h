#pragma once

#include "Restaurant.h"

class Restaurateur{
    string id = IDGenerator::uuid();
    
    string name;
    string restaurantID;

    map<string, cost> saleStatisics{}; // Some of costs of a 'MenuItem'(distinguished by id)
    map<string, vector<Order>> customerStatistics{}; // Orders of a 'Customer'(distinguished by id)

public:
    Restaurateur() = default;
    Restaurateur(string initRestaurantID, string initName="");
    ~Restaurateur();

    Restaurateur& operator=(const Restaurateur& newRestaurateur);

    // Getters and Setters:---------------------------------
    
    // Itself:
    
    string getID() const;
    string getName() const;

    void setName(string newName);

    // Restaurant:

    string getRestaurantID() const;
    string getRestaurantName() const;
    string getRestaurantAddress() const;
    bool getRestaurantStatus() const;
    size_t getRestaurantPreparationMinutes() const;
    string getRestaurantPhone() const;
    string getRestaurantBio() const;

    bool setRestaurantID(string newRestaurantID); // And updates statistics, so on.
    bool editRestaurantName(string newName);
    bool editRestaurantAddress(string newAddress);
    bool activateRestaurant();
    bool deactivateRestaurant();
    bool setPreparationTime(const size_t minutes);
    bool setPhoneNumber(const string newPhoneNumber);
    bool setBio(const string newBio);

    // Menu:
    
    string getMenuID() const;
    
    bool addItemToMenu(const MenuItem& item);
    // 'addItemToMenu' returns true if the item wasn't in the menu and has been added
    bool removeItemFromMenu(string itemID);
    // 'removeItemFromMenu' returns true if the item was in the menu and has been deleted
    bool replaceItemInMenu(string previousItemID, const MenuItem& replacedItem);

    // Order
    
    
    bool addItemToOrder(string orderID, const MenuItem& item, double quantity = 1);
    bool removeItemFromOrder(string orderID, string itemID);

    // Queue:

    vector<string> getOrderIDs() const;

    bool addOrderToQueue(string orderID);
    // addOrderToQueue returns true if the order hasn't been in the orders(distinguished by id)
    // and carefully added
    bool removeOrderFromQueue( string orderID);
    // removeOrderFromQueue returns true if the order has been in the orders(distinguished by id)
    // and carefully removed
    bool replaceOrderInQueue(string previousOrderID, string newOrderID);
    // replaceOrderInQueue returns true if the previous order has been in the orders(distinguished by id)
    // and the new order hasn't been in the orders(distinguished by id) and

    // statistics:

    bool updateAndPrintSaleStatistics();
    bool updateAndPrintCustomerStatistics();

};