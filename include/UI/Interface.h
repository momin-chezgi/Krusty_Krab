# pragma once

# include "Common/Types.h"
# include "Domain/Admin.h"
# include "Domain/Customer.h"
# include "Domain/Order.h"
# include "Domain/Restaurateur.h"

# include "UI/Printer.h"
# include "UI/GetInformation.h"

int Login();
bool enterAsCustomer(Customer &buffer);
Restaurateur enterAsRestaurateur();
AdminOfSystem enterAsAdmin();
MenuID_tp giveMenu(RestID_tp restaurantID);
bool orderOut(RestID_tp RestaurantID, MenuID_tp menuID, Order& resultOrder);
int restaurateurOptions(string givenName, const string& givenAddress, bool givenStatus, size_t givenMinutes);
int adminOptions(vector<ManagerID_tp> restaurateurIDs);
Restaurateur findForRestaurant(RestID_tp givenID);
RestID_tp chooseRestaurant();
