#pragma once

#include "Domain/Restaurant.h"

class Restaurateur{
    ManagerID_tp id = IDGenerator::uuid();
    
    string name;
    RestID_tp restaurantID;

    map<ItemID_tp, cost> saleStatisics{}; // Some of costs of a 'MenuItem'(distinguished by id)
    map<CustID_tp, vector<Order>> customerStatistics{}; // Orders of a 'Customer'(distinguished by id)

public:
    Restaurateur() = default;
    Restaurateur(RestID_tp initRestaurantID, string initName="");
    Restaurateur(const Restaurateur& other) = default;
    ~Restaurateur();

    Restaurateur& operator=(const Restaurateur& newRestaurateur);

    // Getters and Setters:---------------------------------
    
    // Itself:
    
    ManagerID_tp getID() const;
    string getName() const;

    void setName(string newName);

    // Restaurant:

    RestID_tp getRestaurantID() const;
    string getRestaurantName() const;
    string getRestaurantAddress() const;
    bool getRestaurantStatus() const;
    size_t getRestaurantPreparationMinutes() const;
    string getRestaurantPhone() const;
    string getRestaurantBio() const;

    bool setRestaurantID(RestID_tp newRestaurantID); // And updates statistics, so on.
    bool editRestaurantName(string newName);
    bool editRestaurantAddress(string newAddress);
    bool activateRestaurant();
    bool deactivateRestaurant();
    bool setPreparationTime(const size_t minutes);
    bool setPhoneNumber(const string newPhoneNumber);
    bool setBio(const string newBio);

    // Menu:
    
    MenuID_tp getMenuID() const;
    
    bool addItemToMenu(const MenuItem* item);
    // 'addItemToMenu' returns true if the item wasn't in the menu and has been added
    bool removeItemFromMenu(ItemID_tp itemID);
    // 'removeItemFromMenu' returns true if the item was in the menu and has been deleted

    // Order
    
    
    bool addItemToOrder(OrderID_tp orderID, MenuID_tp menuID, ItemID_tp itemID, double quantity = 1);
    bool removeItemFromOrder(OrderID_tp orderID, ItemID_tp itemID);

    // Queue:

    vector<OrderID_tp> getOrderHistoryIDs() const;

    bool addOrderToQueue(OrderID_tp orderID);
    // addOrderToQueue returns true if the order hasn't been in the orders(distinguished by id)
    // and carefully added
    bool removeOrderFromQueue(OrderID_tp orderID);
    // removeOrderFromQueue returns true if the order has been in the orders(distinguished by id)
    // and carefully removed
    bool setOrderStatus();

    // Monitoring:

    bool updateAndPrintSaleStatistics();
    bool updateAndPrintCustomerStatistics();
    void showCurrentOrders();
    void showOrderHistory();
    void showMenu();

};
