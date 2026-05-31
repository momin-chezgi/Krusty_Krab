#include "../neededLibs.h"
#include "../Management/Menu.h"

namespace Printer{
    void wellcome();
    void menu(string menuID);
    void restaurateurDashboard(string nm,
         string ad
         , bool isactive
         , size_t averaget);
    void InvalidInput();
    void RestaurateurChoices();
    void totalSaleStatistics(const map<Restaurant *, map<MenuItem, cost>> &tss);
    void totalCustomerStatistics(const map<Restaurant *, map<Customer, vector<Order>>> &tcs);
    void saleStatistics(const map<MenuItem, cost> &ss);
    void customerStatistics(const map<Customer, vector<Order>> &cs);
    void chooseRestaurant();
    void orderOut();
    void addItemToCart();
};
