#include "Common/Types.h"
#include "Domain/Customer.h"
#include "Domain/Menu.h"
#include "Domain/Order.h"
#include "Domain/Restaurant.h"
#include "Domain/Enums.h"

namespace Printer{
    void wellcome();
    void menu(MenuID_tp menuID);
    void restaurateurDashboard(string nm,
         string ad
         , bool isactive
         , size_t averaget);
    void InvalidInput();
    void RestaurateurChoices();
    void CustomerDashboard(const string& name);
    void CustomerChoices();
    void totalSaleStatistics(const map<Restaurant *, map<MenuItem, cost>> &tss);
    void totalCustomerStatistics(const map<Restaurant *, map<Customer, vector<Order>>> &tcs);
    void saleStatistics(const map<MenuItem, cost> &ss);
    void customerStatistics(const map<Customer, vector<Order>> &cs);
    void chooseRestaurant();
    void orderOut();
    void addItemToCart();
    void adminDashboard(const vector<ManagerID_tp> &restaurateurIDs);
    void adminChoices();
    void debugStorage();
};
