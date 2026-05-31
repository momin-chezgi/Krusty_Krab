#pragma once
#include "../neededLibs.h"
#include "../Management/Restaurant.h"
#include "Printer.h"

namespace GetInf{
    Restaurant *restaurant(); // inputs the id of the restaurant and return a pointer to restaurant
    Restaurateur *restaurateur();
    bool customer(Customer &buffer); // like restaurant() function
    auto modifyRestaurant();
    string menu(string restaurantID);
    bool order(const Order const *resultOrder);
    // returns false if user doesn't want to order
    auto menuItem();
    string customerName();
    int loginRule();
    auto restaurateurAction();
    AdminOfSystem *admin();
    // You can also add the pasword authantication proccess to admin and restaurateurs;
    Restaurateur findRestaurant(string restaurantID);
    string chooseRestaurant();
    bool orderOut(string restaurantID, Order& buffer);   // returns false if the customer hasn't ordered anything or doesn't want to order, otherwise returns true and saves the order in buffer
    bool addItemToCart(string menuID, Order& resultOrder);
};