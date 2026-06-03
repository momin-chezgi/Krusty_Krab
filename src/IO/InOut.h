#pragma once

# include "../neededLibs.h"
# include "../Management/Customer.h"
# include "../Management/Order.h"
# include "../Management/Menu.h"
# include "../Management/Customer.h"
# include "../Management/Restaurant.h"
# include "../Management/Admin.h"


class OrderStorage{
public:
    bool isValidOrder(string orderID);
    bool saveOrder(const Order& newOrder);
    // returns true if the order has been saved successfully, otherwise false
    bool deleteOrder(string orderID);  // returns true if the order with the given ID exists and has been deleted, otherwise returns false
    bool updateOrder(const Order& updatingOrder);  // returns true if the order with the given ID exists and has been updated, otherwise returns false
    bool addItem(string orderID, const MenuItem& newItem, double quantity);
    bool removeItem(string orderID, string itemID);
private:
    static map<string, Order> orders;
};




class MenuStorage{
public:
    bool has(string menuID, string itemID);
    bool isValidMenu(string menuID);
    bool saveMenu(const Menu& newMenu);  // returns true if the menu has been saved successfully, otherwise false
    Menu giveMenu(string menuID);  // returns the menu with the given ID, if it doesn't exist, it returns a default menu with no items
    bool deleteMenu(string menuID);  // returns true if the menu with the given ID exists and has been deleted, otherwise returns false
    bool updateMenu(const Menu& updatingMenu);  // returns true if the menu with the given ID exists and has been updated, otherwise returns false
    bool addItem(string menuID, const MenuItem& newItem);
    bool deleteItem(string menuID, string itemID);
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
    static map<string, Customer> customers;
};



class RestaurateurStorage{
public:
    string getName(string restaurateurID) const;

    bool setName(string restaurateurID, string newName);

    bool isValidRestaurateur(string restaurateurID);
    Restaurateur giveRestaurateur(string restaurateurID);  
    // returns the restaurateur with the given ID, if it doesn't exist, it returns a default restaurateur with empty name and no restaurant
    bool saveRestaurateur(const Restaurateur& newRestaurateur);
    // returns true if the restaurateur has been saved successfully, otherwise false
    bool deleteRestaurateur(string restaurateurID);
    // returns true if the restaurateur with the given ID exists and has been deleted, otherwise returns false
private:
    static map<string, Restaurateur> restaurateurs;
};



class RestaurantStorage{
public:
    string getName(string restaurantID) const;
    string getAddress(string restaurantID) const;
    bool getStatus(string restaurantID) const;
    size_t getPreparationMinutes(string restaurantID) const;
    string getPhoneNumber(string restaurantID) const;
    string getBio(string restaurantID) const;
    string getMenuID(string restaurantID) const;
    vector<string> getOrderIDs(string restaurantID) const;

    bool setName(string restaurantID, string newName);
    bool setAddress(string restaurantID, string newAddress);
    bool activateRestaurant(string restaurantID);
    bool deactivateRestaurant(string restaurantID);
    bool setPreparationTime(string restaurantID, const size_t minutes);
    bool setPhoneNumber(string restaurantID, const string newPhoneNumber);
    bool setBio(string restaurantID, const string newBio);
    
    bool isValidRestaurant(string restaurantID);

    bool addOrderToRestaurant(string restaurantID, string orderID);
    bool deleteOrderFromRestaurant(string restaurantID, string orderID);
    
    Restaurant giveRestaurant(string restaurantID);

    RestID_tp saveRestaurant(const Restaurant& newRestaurant);  // returns true if the restaurant has been saved successfully, otherwise false
    bool deleteRestaurant(string restaurantID);  // returns true if the restaurant with the given ID exists and has been deleted, otherwise returns false
    bool replaceRestaurant(string prerestaurantID, const Restaurant& updatingRestaurant);  // returns true if the restaurant with the given ID exists and has been updated, otherwise returns false

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