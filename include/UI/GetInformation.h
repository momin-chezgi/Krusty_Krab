#pragma once
#include "Common/Types.h"
#include "Domain/Admin.h"
#include "Domain/Customer.h"
#include "Domain/MenuItem.h"
#include "Domain/Order.h"
#include "Domain/Restaurant.h"
#include "Domain/Restaurateur.h"
#include "UI/Printer.h"

namespace GetInf{
    int loginRule();

    // customer:----------------------------------------
    bool customer(Customer &buffer); // like restaurant() function
    string customerName();

    // restaurant:--------------------------------------
    Restaurant *restaurant(); // inputs the id of the restaurant and return a pointer to restaurant
    Restaurant newRestaurant();
    string modifyRestaurantString(int choosenOption);
    size_t modifyRestaurantTime(size_t choosenOption);
    Restaurateur findRestaurant(RestID_tp restaurantID);
    RestID_tp chooseRestaurant();

    // restaurateur:------------------------------------
    Restaurateur restaurateur();
    int restaurateurAction();
    // You can also add the pasword authantication proccess to admin and restaurateurs;

    // admin:-------------------------------------------
    bool admin(AdminOfSystem &buffer);
    int adminOptions(const vector<ManagerID_tp> &restaurateurIDs);

    // menu:--------------------------------------------
    MenuID_tp menu(RestID_tp restaurantID);

    // menuItem:----------------------------------------
    MenuItem* menuItem();
    ItemID_tp menuItemID();

    // order:-------------------------------------------
    bool orderOut(RestID_tp restaurantID, Order& buffer);   
    // returns false if the user hasn't ordered anything 
    // or doesn't want to order, otherwise returns true and saves the order in buffer
    bool addItemToCart(MenuID_tp menuID, Order& resultOrder);
    OrderID_tp OrderID(int option=-1);

    // not implemented yet:
    bool order(const Order *resultOrder);
    // returns false if user doesn't want to order
};
