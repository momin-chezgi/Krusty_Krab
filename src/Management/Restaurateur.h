#pragma once

#include "Restaurant.h"

class Restaurateur{
    const string id = IDGenerator::uuid();
    
    string name;
    string restaurantID;

    map<string, cost> saleStatisics{};
    // Some of costs of a 'MenuItem'(distinguished by id)
    map<string, vector<Order>> customerStatistics{};
    // Orders of a 'Customer'(distinguished by id)
public:
    Restaurateur(string initName="");
    Restaurateur(const Restaurant& initRestaurant, string initName="");

    Restaurateur& operator=(const Restaurateur& newRestaurateur);

    string myRestaurant() const;
    Restaurant getRestaurant() const;
    string getRestaurantName() const;
    string getRestaurantAddress() const;
    bool getRestaurantStatus() const;
    size_t getRestaurantPreparationMinutes() const;
    Menu getMenu() const;
    vector<string> getOrderIDs() const;
    string getID() const;
    string getName() const;

    void setName(string newName);

    void editRestaurantName(string newName);
    void editRestaurantAddress(vector<string> newAddress);
    void activateRestaurant();
    void deactivateRestaurant();

    // bool addItemToMenu(const MenuItem const *mi);
    // bool removeItemFromMenu(MenuItem previousOrder, MenuItem replacedOrder);
    // bool replaceItemFromMenu(MenuItem previousItem, MenuItem replacedItem);

    // bool editThisOrder(Order *previousOrderptr, Order replacedOrder);

    void updateAndPrintSaleStatistics();
    void updateAndPrintCustomerStatistics();

    bool AddItemToOrder(string orderID, const MenuItem const* item);
    bool RemoveItemFromOrder(string orderID, string itemID);

    bool addItemToMenu( const MenuItem const *item);
    // 'addItemToMenu' returns true if the item wasn't in the menu and has been added
    bool removeItemFromMenu(const MenuItem const *item);
    // 'removeItemFromMenu' returns true if the item was in the menu and has been deleted
    
    bool AddOrderToQueue(string orderID);
    // AddToQueue returns true if the order hasn't been in the orders(distinguished by id)
    // and carefully added
    bool removeOrderFromQueue( string orderID);

    ~Restaurateur();

};