#include "UI/Printer.h"
#include "Repository/AdminStorage.h"
#include "Repository/CustomerStorage.h"
#include "Repository/MenuStorage.h"
#include "Repository/OrderStorage.h"
#include "Repository/RestaurantStorage.h"
#include "Repository/RestaurateurStorage.h"

namespace {
    const string panelRule = "========================================================";
    const string newSpace  = "________________________________________________________";

    void printHeader(const string& title)
    {
        cout << "\n" << panelRule << "\n";
        cout << title << "\n";
        cout << panelRule << "\n";
    }
    void ShowRestaurants()
    {
        cout << "\n" << "Active Restaurants:" << "\n";
        vector<RestID_tp> list = RestaurantStorage::activeRestaurantList();
        for(const auto & rest : list){
            cout << rest << "-";
        }
        cout << endl;
    }
}

void Printer::InvalidInput()
{
    cout << "Invalid input, please try again." << endl;
}

void Printer::wellcome()
{
    printHeader("Welcome to Krusty Krab management system");
    cout << "Select your role by entering the option number." << endl;
    cout << "1. Customer" << endl;
    cout << "2. Restaurateur / In-charge of a restaurant" << endl;
    cout << "3. Admin of the system" << endl;
    cout << "-1. Clear the screen" << endl;
    cout << "0. Exit the application" << endl;
}

void Printer::chooseRestaurant()
{
    cout << "Choose a restaurant by entering its ID: " << endl;
}

void Printer::menu(MenuID_tp menuID)
{
    MenuStorage storage;

    Menu menu = storage.giveMenu(menuID);

    printHeader("Restaurant Menu");
    for (const auto& item : menu.getMenu())
    {
        if(item->isAvailable(0)){
            if(Food* fptr = dynamic_cast<Food*>(item)){
                cout << "  - " << item->getName()
                    << " | " << item->getBio()
                    << " | Price: " << item->getPricePerUnit()
                    << " | Prep time: " << item->getPreparationMinutes() << " min(s)"
                    << " | weight(kg): " << fptr->getWeight()
                    << " | ID: " << item->getID() << endl;

            }
            if(Drink* dptr = dynamic_cast<Drink*>(item)){
                cout << "  - " << item->getName()
                    << " | " << item->getBio()
                    << " | Price: " << item->getPricePerUnit()
                    << " | Prep time: " << item->getPreparationMinutes() << " min(s)"
                    << " | volume(litre): " << dptr->getVolume()
                    << " | ID: " << item->getID() << endl;
            }
        }
    }
}

void Printer::orderOut()
{
    cout << "Would you like to place an order? (y/n) " << endl;
}

void Printer::addItemToCart()
{
    cout << "Enter item ID to add to your cart (q to finish): " << endl;
}

void Printer::orderID()
{
    cout << "Enter the ID of the order" << endl;
}

void Printer::orderStatus()
{
    cout << "Which status do you want?" << endl;
    cout << "1. In-preparation" << endl;
    cout << "2. Ready to send" << endl;
    cout << "3. Delivered" << endl;
    cout << "0. Cancelled" << endl;
}

void Printer::showCurrentOrders(vector<OrderID_tp> QueueByID)
{
    cout << "\nCurrent orders in queue (IDs):" << endl;
    if (QueueByID.empty()) {
        cout << "- none" << endl;
        return;
    }
    for(auto it : QueueByID) cout << it << ", ";
    cout << endl;
}
void Printer::showOrderHistory(vector<OrderID_tp> OrdersByID)
{
    cout << "\nOrder history (IDs):" << endl;
    if (OrdersByID.empty()) {
        cout << "- none" << endl;
        return;
    }
    for(auto it : OrdersByID) cout << it << ", ";
    cout << endl;
}


void Printer::restaurateurDashboard(string nm,
         string ad
         , bool isactive
         , size_t averaget)
{
    printHeader("Restaurateur Dashboard");
    cout << "Welcome, " << nm << "!" << endl;
    cout << "Restaurant address: " << ad << endl;
    cout << "Restaurant status: " << (isactive ? "active" : "inactive") << endl;
    cout << "Average preparation time: " << averaget << " minutes" << endl << endl;
}

void Printer::RestaurateurChoices()
{
    cout << endl << newSpace << endl;
    cout << "Select an option by entering the option number:" << endl;
    cout << "1. Edit restaurant name" << endl;
    cout << "2. Edit restaurant address" << endl;
    cout << "3. Activate restaurant" << endl;
    cout << "4. Deactivate restaurant" << endl;
    cout << "5. Edit restaurant preparation time" << endl;
    cout << "6. Edit restaurant phone number" << endl;
    cout << "7. Edit restaurant bio" << endl;

    cout << "\nMenu editing options:" << endl;
    cout << "11. Add an item to menu" << endl;
    cout << "12. Remove an item from menu" << endl;
    cout << "13. Raise item stock" << endl;
    cout << "14. Decrease item stock" << endl;

    cout << "\nOrder editing options:" << endl;
    cout << "21. Add an order to the order queue" << endl;
    cout << "22. Remove an order from the order queue" << endl;
    cout << "23. Set the status of an order" << endl;

    cout << "\nMonitoring:" << endl;
    cout << "31. Print sale statistics" << endl;
    cout << "32. Print customer statistics" << endl;
    cout << "33. Show current orders in queue" << endl;
    cout << "34. Show the history of orders" << endl;
    cout << "35. Show the menu of the restaurant" << endl;
    cout << "\nprofile:" << endl;
    cout << "41. Edit my name" << endl;
    cout << "42. Edit managed restaurant ID" << endl;
    cout <<"\nOthers:" << endl;
    cout << "404. Debug: print database storage" << endl;
    cout << "0. Exit dashboard" << endl;
}

void Printer::adminDashboard(const vector<ManagerID_tp> &restaurateurIDs)
{
    printHeader("Admin Dashboard");
    cout << "Current restaurateurs:" << endl;
    for (const auto& id : restaurateurIDs) {
        cout << "- " << id << endl;
    }
}

void Printer::adminChoices()
{
    cout << endl << newSpace << endl;
    cout << "Select an option:" << endl;
    cout << "1. Create restaurant (create a restaurateur before it)" << endl;
    cout << "2. Activate restaurant" << endl;
    cout << "3. Deactivate restaurant" << endl;
    cout << "4. Create restaurateur" << endl;
    cout << "31. Print total sale statistics" << endl;
    cout << "32. Print total customer statistics" << endl;
    cout << "33. Print total sale statistics (legacy code)" << endl;
    cout << "34. Print total customer statistics (legacy code)" << endl;
    cout << "\nMembership management:" << endl;
    cout << "41. Print membership level report" << endl;
    cout << "42. Change a customer's membership" << endl;
    cout << "43. Show a customer's membership level history" << endl;
    cout << "404. Debug: print database storage" << endl;
    cout << "0. Exit dashboard" << endl;
}

void Printer::CustomerDashboard(
    const Customer& customer,
    const MembershipSummary& summary
)
{
    printHeader("Customer Dashboard");
    cout << "Welcome, " << customer.getName() << endl;
    cout << "Membership level: " << summary.levelName << " Member" << endl;
    cout << "Current points: " << summary.currentPoints << endl;
    if (summary.level == Level::VIP) {
        cout << "Points needed for next level: top level reached" << endl;
    } else {
        cout << "Points needed for next level: " << summary.pointsToNextLevel << endl;
    }
    cout << "Order discount: " << summary.discountPercent << "%" << endl;
    cout << "Delivery benefit: " << summary.deliveryBenefit << endl;
    cout << "Lottery tickets: " << summary.lotteryTickets << endl;
    ShowRestaurants();
}

void Printer::checkoutInvoice(const CheckoutSummary& summary)
{
    printHeader("Checkout Invoice");
    cout << "Base total: " << summary.baseTotal << endl;
    cout << "Membership discount (" << summary.discountPercent << "%): -" << summary.discountAmount << endl;
    cout << "Delivery base fee: " << summary.baseDeliveryFee << endl;
    cout << "Delivery discount: -" << summary.deliveryDiscountAmount << endl;
    cout << "Final delivery fee: " << summary.finalDeliveryFee << endl;
    cout << "Final payable total: " << summary.finalTotal << endl;
    cout << "Points earned from this order: " << summary.earnedPoints << endl;
}

void Printer::membershipUpgrade(const string& message)
{
    cout << endl << message << endl;
}

void Printer::membershipLevelReport(const map<Level, size_t>& counts)
{
    printHeader("Membership Level Report");
    cout << "Normal: " << counts.at(Level::Normal) << endl;
    cout << "Silver: " << counts.at(Level::Silver) << endl;
    cout << "Gold: " << counts.at(Level::Gold) << endl;
    cout << "VIP: " << counts.at(Level::VIP) << endl;
}

void Printer::membershipLevelHistory(const vector<MembershipLevelLogEntry>& history)
{
    printHeader("Membership Level History");
    if (history.empty()) {
        cout << "No history found." << endl;
        return;
    }

    for (const auto& entry : history) {
        cout << "- customer: " << entry.customerID
             << ", level: " << levelToString(entry.level)
             << ", points: " << entry.points
             << ", changed_at: " << entry.changedAt << endl;
    }
}

void Printer::CustomerChoices()
{
    cout << endl << newSpace << endl;
    cout << "Select an option:" << endl;
    cout << "1. Place order" << endl;
    cout << "2. View my orders and status" << endl;
    cout << "404. Debug: print database storage" << endl;
    cout << "0. Exit dashboard" << endl;
    cout << "-1. Clear the screen" << endl;
}

void Printer::customerOrders(const vector<Order>& orders)
{
    if (orders.empty()) {
        cout << "No orders found for this customer." << endl;
        return;
    }

    cout << "My orders:" << endl;
    for (const auto& order : orders) {
        cout << "- Order ID: " << order.getID()
             << ", status: " << orderStatus2String(order.getOrderStatus())
             << ", total: " << order.getTotalPrice() << endl;
    }
}

void Printer::debugStorage()
{
    CustomerStorage customerStorage;
    RestaurateurStorage restaurateurStorage;
    AdminStorage adminStorage;
    RestaurantStorage restaurantStorage;
    MenuStorage menuStorage;
    OrderStorage orderStorage;

    printHeader("DEBUG STORAGE");

    cout << "Customers:" << endl;
    map<CustID_tp, Customer> customers = customerStorage.giveAllCustomers();
    if (customers.empty()) {
        cout << "- none" << endl;
    }
    for (const auto& entry : customers) {
        Customer customer = entry.second;
        cout << "- storage key: " << entry.first
             << ", object ID: " << customer.getID()
             << ", name: " << customer.getName() << endl;
        cout << "  Orders: ";
        if (customer.getMyOrders().empty()) {
            cout << "none";
        }
        for (const auto& orderID : customer.getMyOrders()) {
            cout << orderID << " ";
        }
        cout << endl;
    }

    cout << "\nRestaurateurs:" << endl;
    map<ManagerID_tp, Restaurateur> restaurateurs = restaurateurStorage.giveAllRestaurateurs();
    if (restaurateurs.empty()) {
        cout << "- none" << endl;
    }
    for (const auto& entry : restaurateurs) {
        Restaurateur restaurateur = entry.second;
        cout << "- storage key: " << entry.first
             << ", object ID: " << restaurateur.getID()
             << ", name: " << restaurateur.getName()
             << ", restaurant ID: " << restaurateur.getRestaurantID() << endl;
    }

    cout << "\nAdmins:" << endl;
    map<AdminID_tp, AdminOfSystem> admins = adminStorage.giveAllAdmins();
    if (admins.empty()) {
        cout << "- none" << endl;
    }
    for (const auto& entry : admins) {
        AdminOfSystem admin = entry.second;
        cout << "- storage key: " << entry.first
             << ", object ID: " << admin.getID()
             << ", name: " << admin.getName() << endl;
        cout << "  Restaurateurs: ";
        if (admin.getRestaurateurIDs().empty()) {
            cout << "none";
        }
        for (const auto& restaurateurID : admin.getRestaurateurIDs()) {
            cout << restaurateurID << " ";
        }
        cout << endl;
    }

    cout << "\nRestaurants:" << endl;
    map<RestID_tp, Restaurant> restaurants = restaurantStorage.giveAllRestaurants();
    if (restaurants.empty()) {
        cout << "- none" << endl;
    }
    for (const auto& entry : restaurants) {
        Restaurant restaurant = entry.second;
        cout << "- storage key: " << entry.first
             << ", object ID: " << restaurant.getID()
             << ", name: " << restaurant.getName()
             << ", address: " << restaurant.getAddress()
             << ", status: " << (restaurant.isActive() ? "active" : "inactive")
             << ", minutes: " << restaurant.getMinutesFoodPrepare()
             << ", phone: " << restaurant.getPhone()
             << ", menu ID: " << restaurant.getMenuID()
             << ", bio: " << restaurant.getBio() << endl;
        cout << "  Order queue: ";
        if (restaurant.getOrderHistoryIDs().empty()) {
            cout << "none";
        }
        for (const auto& orderID : restaurant.getOrderHistoryIDs()) {
            cout << orderID << " ";
        }
        cout << endl;
    }

    cout << "\nMenus:" << endl;
    map<MenuID_tp, Menu> menus = menuStorage.giveAllMenus();
    if (menus.empty()) {
        cout << "- none" << endl;
    }
    for (const auto& entry : menus) {
        Menu menu = entry.second;
        cout << "- storage key: " << entry.first
             << ", object ID: " << menu.getID() << endl;
        vector<MenuItem*> items = menu.getMenu();
        if (items.empty()) {
            cout << "  items: none" << endl;
        }
        for (const auto& item : items) {
            if (!item) {
                continue;
            }
            if(Food* fptr = dynamic_cast<Food*>(item)){
                cout << "  Item ID: " << item->getID()
                    << ", name: " << item->getName()
                    << ", type: " << item->getItemType()
                    << ", price: " << item->getPricePerUnit()
                    << ", minutes: " << item->getPreparationMinutes()
                    << ", reminded: " << fptr->getWeight() << " kgs"
                    << ", bio: " << item->getBio() << endl;
            }
            if(Drink* dptr = dynamic_cast<Drink*>(item)){
                cout << "  Item ID: " << item->getID()
                    << ", name: " << item->getName()
                    << ", type: " << item->getItemType()
                    << ", price: " << item->getPricePerUnit()
                    << ", minutes: " << item->getPreparationMinutes()
                    << ", reminded: " << dptr->getVolume() << " litres"
                    << ", bio: " << item->getBio() << endl;
            }
        }
    }

    cout << "\nOrders:" << endl;
    map<OrderID_tp, Order> orders = orderStorage.giveAllOrders();
    if (orders.empty()) {
        cout << "- none" << endl;
    }
    for (const auto& entry : orders) {
        Order order = entry.second;
        cout << "- storage key: " << entry.first
             << ", object ID: " << order.getID()
             << ", customer ID: " << order.getOrderer()
             << ", status: " << orderStatus2String(order.getOrderStatus())
             << ", total: " << order.getTotalPrice() << endl;
        vector<OrderLine> items = order.getOrder();
        if (items.empty()) {
            cout << "  Items: none" << endl;
        }
        for (const auto& line : items) {
            if (!line.first) {
                continue;
            }
            cout << "  Item ID: " << line.first->getID()
                 << ", name: " << line.first->getName()
                 << ", quantity: " << line.second
                 << ", unit price: " << line.first->getPricePerUnit() << endl;
        }
    }

    cout << panelRule << "\n" << endl;
}

void Printer::clearScreen()
{
    cout << "\033[2J\033[H";
    cout.flush();
}
