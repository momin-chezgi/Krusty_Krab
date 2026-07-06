# pragma once

# include "Common/Types.h"
# include "Domain/Admin.h"
# include "Domain/Customer.h"
# include "Domain/Order.h"
# include "Domain/Restaurateur.h"
# include "Domain/Enums.h"
# include "Domain/MembershipLevel.h"

# include "UI/Printer.h"
# include "UI/GetInformation.h"

Role Login();
bool enterAsCustomer(Customer &buffer);
Restaurateur enterAsRestaurateur();
AdminOfSystem enterAsAdmin(AdminID_tp &adminID);
MenuID_tp giveMenu(RestID_tp restaurantID);
bool orderOut(MenuID_tp menuID, Order& resultOrder);
RestaurateurAction restaurateurOptions(string restName, RestID_tp restID,string givenName, const string& givenAddress, bool givenStatus, size_t givenMinutes);
AdminAction adminOptions(vector<ManagerID_tp> restaurateurIDs);
CustomerAction customerActions(
    const Customer& user,
    const MembershipSummary& membershipSummary
);
Restaurateur findForRestaurant(RestID_tp givenID);
RestID_tp chooseRestaurant();
