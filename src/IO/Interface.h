# pragma once

# include "../Utils/IDGenerator.h"
# include "../Management/Restaurateur.h"
# include "../Management/Admin.h"

# include "InOut.h"
#include "Printer.h"
#include "GetInformation.h"

int Login();
bool enterAsCustomer(Customer &buffer);
Restaurateur *enterAsRestaurateur();
AdminOfSystem *enterAsAdmin();
string giveMenu(string restaurantID);
bool orderOut(string menuID, Order& resultOrder);
auto restaurateurOptions(string givenName, vector<string> givenAddress, bool givenStatus, size_t givenMinutes);
auto adminOptions();
Restaurateur findForRestaurant(string givenID);
string chooseRestaurant();