#pragma once

#include "../neededLibs.h"
#include "../Management/Customer.h"

class InOut{
    virtual int reader() = 0;
    virtual int writer() = 0;
};

class SQLReader : public InOut{

public:
    Menu getMenu();
    bool getCustomer(string CustomerID, Customer &buffer);  // returns true if the customer with the given ID exists and saves it in buffer, otherwise returns false
    bool getRestaurant(string restaurantID, Restaurant &buffer);  // like getCustomer() function
    // You can also add the password authentication process to getRestaurant and getCustomer functions
    bool isValidRestaurant(string restaurantID);    
};

class SQLWriter : public InOut{
};

class OrderStorage{
public:
    bool saveOrder(const Order& newOrder);
    // returns true if the order has been saved successfully, otherwise false
    bool deleteOrder(string orderID);  // returns true if the order with the given ID exists and has been deleted, otherwise returns false
    bool updateOrder(const Order& updatingOrder);  // returns true if the order with the given ID exists and has been updated, otherwise returns false
private:
    bool isValidOrder(string orderID);
    static map<string, Order> orders;
};

class MenuStorage{
public:
bool isValidMenu(string menuID);
    bool saveMenu(const Menu& newMenu);  // returns true if the menu has been saved successfully, otherwise false
    Menu giveMenu(string menuID);  // returns the menu with the given ID, if it doesn't exist, it returns a default menu with no items
    bool deleteMenu(string menuID);  // returns true if the menu with the given ID exists and has been deleted, otherwise returns false
    bool updateMenu(const Menu& updatingMenu);  // returns true if the menu with the given ID exists and has been updated, otherwise returns false
private:
    static map<string, Menu> menus;
};

class CustomerStorage{
public:
    bool isValidCustomer(string customerID);
    Customer giveCustomer(string customerID);  
    // returns the customer with the given ID, if it doesn't exist, it returns a default customer with empty name and no orders
    bool saveCustomer(const Customer& newCustomer);
    // returns true if the customer has been saved successfully, otherwise false
    bool deleteCustomer(string customerID);
    // returns true if the customer with the given ID exists and has been deleted, otherwise returns false
    
private:
    static map<string, Customer> customers{{"TestCustomer", Customer("TestCustomer")}};
};

class RestaurantStorage{
public:
    bool addOrderToRestaurant(string restaurantID, string orderID);
    bool isValidRestaurant(string restaurantID);
    Restaurant giveRestaurant(string restaurantID);
    bool saveRestaurant(const Restaurant& newRestaurant);  // returns true if the restaurant has been saved successfully, otherwise false
    bool deleteRestaurant(string restaurantID);  // returns true if the restaurant with the given ID exists and has been deleted, otherwise returns false
    bool updateRestaurant(const Restaurant& updatingRestaurant);  // returns true if the restaurant with the given ID exists and has been updated, otherwise returns false
private:
    static map<string, Restaurant> restaurants;
};

class AdminStorage{
public:
    bool saveAdmin(const AdminOfSystem& newAdmin);  // returns true if the admin has been saved successfully, otherwise false
    bool deleteAdmin(string adminID);  // returns true if the admin with the given ID exists and has been deleted, otherwise returns false
    bool updateAdmin(const AdminOfSystem& updatingAdmin);  // returns true if the admin with the given ID exists and has been updated, otherwise returns false
private:
    bool isValidAdmin(string adminID);
    static map<string, AdminOfSystem> admins;
};