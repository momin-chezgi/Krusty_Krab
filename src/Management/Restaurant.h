#pragma once

#include "Order.h"

class Restaurant{
    string id = IDGenerator::uuid();
    string name;
    string address;          // city/street/building No
    bool activationStatus{true};
    size_t standardTimeOfPreparation;   // per minute
    string phoneNumber;
    string bio;
    vector<string> orderIDs{};
    Menu menu = Menu({});

public:
    Restaurant();
    Restaurant(Restaurant* r);
    Restaurant(string initName, vector<string> initAddress, string initPhoneNumber, string initBio = "", size_t minutesPrepared = 0);

    string getID() const;
    string getName() const;
    string getAddress() const;
    // string getAddress() ;  //converts the vector<string> form to a pure string
    bool isActive() const;
    size_t getMinutesFoodPrepare() const;
    string getPhone() const;
    string getBio() const;
    const vector<string> &getOrderIDs() const;
    Menu getMenu() const;

    void setName(const string newName);
    void setAddress(const string& newAddress);
    void activate();
    void deactivate();
    void setPreparationTime(const size_t minutes);
    void setPhoneNumber(const string newPhoneNumber);
    void setBio(const string newBio);
    // There isn't set menu because the restaurateur just can modify the menu

    bool AddItemToOrder(string orderID, const MenuItem const* item);
    bool RemoveItemFromOrder(string orderID, string itemID);

    bool AddOrderToQueue(string newOrderID);
    // The input is the ID of the order, 
    // Because we believe that the order has been created before
    // In the 'RestaurantStorage'
    bool removeOrderFromQueue(string removingOrderID);

    bool addItemToMenu( const MenuItem const *item);
    // 'addItemToMenu' returns true if the item wasn't in the menu and has been added
    bool removeItemFromMenu(const MenuItem const *item);
    // 'removeItemFromMenu' returns true if the item was in the menu and has been deleted

    ~Restaurant();

};

