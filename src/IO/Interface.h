# pragma once

# include "../Utils/IDGen.h"
# include "../Management/Restaurateur.h"
# include "../Management/Admin.h"

# include "InOut.h"
#include "Printer.h"
#include "GetInformation.h"

int Login();
Customer *enterAsCustomer();
Restaurateur *enterAsRestaurateur();
AdminOfSystem *enterAsAdmin();
Menu giveMenu(size_t restaurantID);
bool orderAtGivenBuffer(const Order const *resultOrder);
auto restaurateurOptions(string givenName, vector<string> givenAddress, bool givenStatus, size_t givenMinutes);
auto adminOptions();
Restaurateur findForRestaurant(size_t givenID);
size_t chooseRestaurant();