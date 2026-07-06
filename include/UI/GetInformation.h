#pragma once
#include "Common/Types.h"
#include "Domain/Admin.h"
#include "Domain/Customer.h"
#include "Domain/MenuItem.h"
#include "Domain/Order.h"
#include "Domain/Restaurant.h"
#include "Domain/Restaurateur.h"
#include "Domain/Enums.h"
#include "UI/Printer.h"

namespace GetInf{
    Role loginRule();

    // customer:----------------------------------------
    bool customer(Customer &buffer); // like restaurant() function
    string customerName();
    Customer customerFactory();
    CustomerAction customerAction();

    // restaurant:--------------------------------------
    Restaurant *restaurant(); // inputs the id of the restaurant and return a pointer to restaurant
    Restaurant newRestaurant();
    string modifyRestaurantString(RestaurateurAction choosenOption);
    size_t modifyRestaurantTime(RestaurateurAction choosenOption);
    Restaurateur findRestaurant(RestID_tp restaurantID);
    RestID_tp chooseRestaurant();

    // restaurateur:------------------------------------
    Restaurateur restaurateur();
    RestaurateurAction restaurateurAction();
    // You can also add the pasword authantication proccess to admin and restaurateurs;

    // admin:-------------------------------------------
    bool admin(AdminOfSystem &buffer, AdminID_tp &adminID);
    AdminAction adminOptions(const vector<ManagerID_tp> &restaurateurIDs);
    Restaurateur restaurateurFactory();
    CustID_tp customerID();
    Level membershipLevel();
    point membershipPoints();

    // menu:--------------------------------------------
    MenuID_tp menu(RestID_tp restaurantID);

    // menuItem:----------------------------------------
    MenuItem* menuItem();
    ItemID_tp menuItemID();
    double menuItemQuantity(RestaurateurAction option);

    // order:-------------------------------------------
    bool orderOut(RestID_tp restaurantID, Order& buffer);   
    // returns false if the user hasn't ordered anything 
    // or doesn't want to order, otherwise returns true and saves the order in buffer
    bool addItemToCart(MenuID_tp menuID, Order& resultOrder);
    OrderID_tp OrderID(RestaurateurAction option = RestaurateurAction::Quit);
    OrderStatus orderStatus(OrderID_tp& buffer);

    // not implemented yet:
    bool order(const Order *resultOrder);
    // returns false if user doesn't want to order
};
