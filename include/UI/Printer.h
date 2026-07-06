#include "Common/Types.h"
#include "Domain/Customer.h"
#include "Domain/Menu.h"
#include "Domain/Order.h"
#include "Domain/Restaurant.h"
#include "Domain/Enums.h"
#include "Domain/Food.h"
#include "Domain/Drink.h"
#include "Domain/MembershipLevel.h"
#include "Repository/MembershipLevelStorage.h"

namespace Printer{
    void wellcome();
    void menu(MenuID_tp menuID);
    void restaurateurDashboard(string nm,
         string ad
         , bool isactive
         , size_t averaget);
    void InvalidInput();
    void RestaurateurChoices();
void CustomerDashboard(
    const Customer& customer,
    const MembershipSummary& summary
);
    void checkoutInvoice(const CheckoutSummary& summary);
    void membershipUpgrade(const string& message);
    void membershipLevelReport(const map<Level, size_t>& counts);
    void membershipLevelHistory(const vector<MembershipLevelLogEntry>& history);
    void CustomerChoices();
    void chooseRestaurant();
    void orderOut();
    void orderID();
    void orderStatus();
    void addItemToCart();
    void customerOrders(const vector<Order>& orders);
    void showCurrentOrders(vector<OrderID_tp> QueueByID);
    void showOrderHistory(vector<OrderID_tp> OrdersByID);
    void adminDashboard(const vector<ManagerID_tp> &restaurateurIDs);
    void adminChoices();
    void debugStorage();
    void clearScreen();
};
