#include "UI/Printer.h"
#include "Repository/AdminStorage.h"
#include "Repository/CustomerStorage.h"
#include "Repository/MenuStorage.h"
#include "Repository/OrderStorage.h"
#include "Repository/RestaurantStorage.h"
#include "Repository/RestaurateurStorage.h"

void Printer::InvalidInput()
{
        cout << "Invalid input, please try again" << endl;
}
void Printer::wellcome()
{
    cout << "Wellcome to Krusty Krab management system" << endl;
    cout << "Please choose your rule(by entering the number of each rule):" << endl;
    cout << "1. Customer" << endl;
    cout << "2. Restaurateur/In-charge of a restaurant" << endl;
    cout << "3. Admin of the system" << endl;
    cout << "0. Exit the application" << endl;
}

void Printer::chooseRestaurant()
{
    cout << "Please choose a restaurant by entering its ID: " << endl;
}

void Printer::menu(MenuID_tp menuID)
{
    MenuStorage storage;

    Menu menu = storage.giveMenu(menuID);

    cout << "Menu: " << endl;
    for (const auto& item : menu.getMenu())
    {
        if(item->isAvailable(1)){
            cout << item->getName() << " - " << item->getBio() << " - Cost: "<< item->getPricePerUnit() << " - Time: " << item->getPreparationMinutes() << " - ID: "<< item->getID() << endl;
        }
    }
}

void Printer::orderOut()
{
    cout << "Do you want to order? (y/n) " << endl;
}

void Printer::addItemToCart()
{
    cout << "Enter the ID of the item you want to add to your cart(q for finalizing and quitting): " << endl;
}

void Printer::restaurateurDashboard(string nm,
         string ad
         , bool isactive
         , size_t averaget)
{
    cout << "Welcome to your dashboard, " << nm << "!" << endl;
    cout << "Your restaurant is located at: " << ad << endl;
    cout << "Your restaurant is currently " << (isactive ? "active" : "inactive") << endl;
    cout << "The average preparation time for your restaurant is: " << averaget << " minutes" << endl << endl;

}

void Printer::RestaurateurChoices()
{
    cout << "Please choose an option by entering the option number:" << endl;
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
    cout << "13. Replace an item in menu" << endl;

    cout << "\nOrder editing options:" << endl;
    cout << "21. Add an order to the order queue" << endl;
    cout << "22. Remove an order from the order queue" << endl;
    cout << "23. Replace an order in the order queue" << endl;

    cout << "\nOther options:" << endl;
    cout << "31. Print sale statistics" << endl;
    cout << "32. Print customer statistics" << endl;
    cout << "41. Edit my name" << endl;
    cout << "42. Edit managed restaurant ID" << endl;
    cout << "404. Debug: print in-memory storage" << endl;
    cout << "0. Exit dashboard" << endl;
}

void Printer::adminDashboard(const vector<ManagerID_tp> &restaurateurIDs)
{
    cout << "Welcome to the admin dashboard" << endl;
    cout << "Current restaurateurs:" << endl;
    for (const auto& id : restaurateurIDs) {
        cout << "- " << id << endl;
    }
}

void Printer::adminChoices()
{
    cout << "Please choose an option:" << endl;
    cout << "1. Create restaurant (create a restaurateur before it)" << endl;
    cout << "2. Activate restaurant" << endl;
    cout << "3. Deactivate restaurant" << endl;
    cout << "4. Create restaurateur" << endl;
    cout << "31. Print total sale statistics" << endl;
    cout << "32. Print total customer statistics" << endl;
    cout << "33. Print total sale statistics (legacy code)" << endl;
    cout << "34. Print total customer statistics (legacy code)" << endl;
    cout << "404. Debug: print in-memory storage" << endl;
    cout << "0. Exit dashboard" << endl;
}

void Printer::CustomerDashboard(const string& name)
{
    cout << "Welcome to customer dashboard, " << name << endl;
}

void Printer::CustomerChoices()
{
    cout << "Please choose an option:" << endl;
    cout << "1. Place order" << endl;
    cout << "2. Show order IDs in my profile" << endl;
    cout << "404. Debug: print in-memory storage" << endl;
    cout << "0. Exit dashboard" << endl;
}

void Printer::debugStorage()
{
    CustomerStorage customerStorage;
    RestaurateurStorage restaurateurStorage;
    AdminStorage adminStorage;
    RestaurantStorage restaurantStorage;
    MenuStorage menuStorage;
    OrderStorage orderStorage;

    cout << "\n========== DEBUG STORAGE ==========" << endl;

    cout << "\nCustomers:" << endl;
    map<CustID_tp, Customer> customers = customerStorage.giveAllCustomers();
    if (customers.empty()) {
        cout << "- none" << endl;
    }
    for (const auto& entry : customers) {
        Customer customer = entry.second;
        cout << "- storage key: " << entry.first
             << ", object ID: " << customer.getID()
             << ", name: " << customer.getName() << endl;
        cout << "  orders: ";
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
        cout << "  restaurateurs: ";
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
        cout << "  order queue: ";
        if (restaurant.getOrderIDs().empty()) {
            cout << "none";
        }
        for (const auto& orderID : restaurant.getOrderIDs()) {
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
            cout << "  item ID: " << item->getID()
                 << ", name: " << item->getName()
                 << ", type: " << item->getItemType()
                 << ", price: " << item->getPricePerUnit()
                 << ", minutes: " << item->getPreparationMinutes()
                 << ", bio: " << item->getBio() << endl;
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
            cout << "  items: none" << endl;
        }
        for (const auto& line : items) {
            if (!line.first) {
                continue;
            }
            cout << "  item ID: " << line.first->getID()
                 << ", name: " << line.first->getName()
                 << ", quantity: " << line.second
                 << ", unit price: " << line.first->getPricePerUnit() << endl;
        }
    }

    cout << "======== END DEBUG STORAGE ========\n" << endl;
}
