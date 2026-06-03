#pragma once
#include "../neededLibs.h"
#include "../Management/Restaurant.h"
#include "Printer.h"
# include "../IO/InOut.h"

namespace GetInf{
    Restaurant *restaurant(); // inputs the id of the restaurant and return a pointer to restaurant
    Restaurant newRestaurant();
    bool customer(Customer &buffer); // like restaurant() function
    // Restaurateur restaurateur();
    ManagerID_tp restaurateur();
    bool admin(AdminOfSystem &buffer);

    int adminOptions(const vector<ManagerID_tp> &restaurateurIDs);
    
    string modifyRestaurantString(int choosenOption);
    size_t modifyRestaurantTime(size_t choosenOption);
    string menu(string restaurantID);
    bool order(const Order const *resultOrder);
    // returns false if user doesn't want to order
    auto menuItem();
    string customerName();
    int loginRule();
    int restaurateurAction();
    // You can also add the pasword authantication proccess to admin and restaurateurs;
    Restaurateur findRestaurant(string restaurantID);
    string chooseRestaurant();
    bool orderOut(string restaurantID, Order& buffer);   
    // returns false if the user hasn't ordered anything 
    // or doesn't want to order, otherwise returns true and saves the order in buffer
    bool addItemToCart(string menuID, Order& resultOrder);
    string OrderID(int option=-1);
};