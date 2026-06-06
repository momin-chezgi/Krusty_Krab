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
    Restaurant *restaurant(); // inputs the id of the restaurant and return a pointer to restaurant
    Restaurant newRestaurant();
    bool customer(Customer &buffer); // like restaurant() function
    Restaurateur restaurateur();
    bool admin(AdminOfSystem &buffer);

    int adminOptions(const vector<ManagerID_tp> &restaurateurIDs);
    
    string modifyRestaurantString(int choosenOption);
    size_t modifyRestaurantTime(size_t choosenOption);
    MenuID_tp menu(RestID_tp restaurantID);
    bool order(const Order *resultOrder);
    // returns false if user doesn't want to order
    MenuItem* menuItem();
    ItemID_tp menuItemID();
    string customerName();
    int loginRule();
    int restaurateurAction();
    // You can also add the pasword authantication proccess to admin and restaurateurs;
    Restaurateur findRestaurant(RestID_tp restaurantID);
    RestID_tp chooseRestaurant();
    bool orderOut(RestID_tp restaurantID, Order& buffer);   
    // returns false if the user hasn't ordered anything 
    // or doesn't want to order, otherwise returns true and saves the order in buffer
    bool addItemToCart(MenuID_tp menuID, Order& resultOrder);
    OrderID_tp OrderID(int option=-1);
};
