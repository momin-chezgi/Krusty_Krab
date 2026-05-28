#pragma once

#include "Restaurant.h"

class Restaurateur{
    size_t id;
    Restaurant *restaurant;
    string name;
    map<MenuItem, cost> saleStatisics{};
    map<Customer, vector<Order>> customerStatistics{};

public:
    Restaurateur(size_t newID);
    Restaurateur(size_t newID, const Restaurant const *initRestaurant, string initName="");

    Restaurant *myRestaurant() const;
    size_t getRestaurantID() const;
    string getRestaurntName() const;
    vector<string> getRestaurantAddress() const;
    bool getRestaurantStatus() const;
    size_t getRestaurantPreparationMinutes() const;
    Menu getMenu() const;
    vector<Order> getOrders() const;
    size_t getID() const;
    string getName() const;

    void setID(size_t newID);
    void setName(string newName);

    void editRestaurantID(size_t newID);
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

    bool AddItemToOrder(size_t orderID, const MenuItem const* item);
    bool RemoveItemFromOrder(size_t orderID, const MenuItem const* item);

    bool addItemToMenu( const MenuItem const *item);
    // 'addItemToMenu' returns true if the item wasn't in the menu and has been added
    bool removeItemFromMenu(const MenuItem const *item);
    // 'removeItemFromMenu' returns true if the item was in the menu and has been deleted
    
    bool AddOrderToQueue( const Order order);
    // AddToQueue returns true if the order hasn't been in the orders(distinguished by id)
    // and carefully added
    bool removeOrderFromQueue( const Order order);

    ~Restaurateur();

};
