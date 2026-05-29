#pragma once
#include "../neededLibs.h"
#include "../Management/Restaurant.h"
#include "Printer.h"

namespace GetInf{
    Restaurant *restaurant(); // inputs the id of the restaurant and return a pointer to restaurant
    Restaurateur *restaurateur();
    Customer *customer(); // like restaurant() function
    auto modifyRestaurant();
    Menu menu(string restaurantID);
    bool order(const Order const *resultOrder);
    // returns false if user doesn't want to order
    auto menuItem();
    string customerName();
    int loginRule();
    auto restaurateurAction();
    AdminOfSystem *admin();
    // You can also add the pasword authantication proccess to admin and restaurateurs;
    Restaurateur findRestaurant(string restaurantID);
    size_t chooseRestaurant();
};