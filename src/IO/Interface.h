# pragma once

# include "../Utils/IDGenerator.h"
# include "../Management/Restaurateur.h"
# include "../Management/Admin.h"

# include "InOut.h"
#include "Printer.h"
#include "GetInformation.h"

int Login();
bool enterAsCustomer(Customer &buffer);
bool enterAsRestaurateur(Restaurateur &buffer);
AdminOfSystem *enterAsAdmin();
string giveMenu(string restaurantID);
bool orderOut(string menuID, Order& resultOrder);
int restaurateurOptions(string givenName, const string& givenAddress, bool givenStatus, size_t givenMinutes);
auto adminOptions();
Restaurateur findForRestaurant(string givenID);
string chooseRestaurant();