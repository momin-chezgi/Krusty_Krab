#pragma once

#include "Domain/Order.h"
#include "Domain/Menu.h"

class Restaurant{
    RestID_tp id = IDGenerator::uuid();
    string name;
    string address;          // city/street/building No
    bool activationStatus{true};
    size_t standardTimeOfPreparation;   // per minute
    string phoneNumber;
    string bio;
    vector<OrderID_tp> orderIDs{};
    Menu menu{};

public:
    Restaurant() = default;
    Restaurant(Restaurant* r);
    Restaurant(string initName, string initAddress, string initPhoneNumber, string initBio = "", size_t minutesPrepared = 0);

    string getName() const;
    string getAddress() const;
    // string getAddress() ;  //converts the vector<string> form to a pure string
    bool isActive() const;
    size_t getMinutesFoodPrepare() const;
    string getPhone() const;
    string getBio() const;
    const vector<OrderID_tp> &getOrderIDs() const;
    Menu getMenu() const;

    void setName(const string newName);
    void setAddress(const string& newAddress);
    void activate();
    void deactivate();
    void setPreparationTime(const size_t minutes);
    void setPhoneNumber(const string newPhoneNumber);
    void setBio(const string newBio);
    // There isn't set menu because the restaurateur just can modify the menu

    bool AddItemToOrder(OrderID_tp orderID,MenuID_tp menuID ,ItemID_tp itemID, double quantity=1);
    bool RemoveItemFromOrder(OrderID_tp orderID, ItemID_tp itemID);
    bool orderIsInQueue(OrderID_tp orderID) const;
    
    bool AddOrderToQueue(OrderID_tp newOrderID);
    // The input is the ID of the order, 
    // Because we believe that the order has been created before
    // In the 'RestaurantStorage'
    bool removeOrderFromQueue(OrderID_tp removingOrderID);

    bool addItemToMenu(const MenuItem *item);
    // 'addItemToMenu' returns true if the item wasn't in the menu and has been added
    bool removeItemFromMenu(const MenuItem const *item);
    // 'removeItemFromMenu' returns true if the item was in the menu and has been deleted

    ~Restaurant();

};
