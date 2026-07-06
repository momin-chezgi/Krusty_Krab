#include "UI/GetInformation.h"
#include "Repository/AdminStorage.h"
#include "Repository/CustomerStorage.h"
#include "Repository/MenuStorage.h"
#include "Repository/OrderStorage.h"
#include "Repository/RestaurantStorage.h"
#include "Repository/RestaurateurStorage.h"
#include "Domain/Drink.h"
#include "Domain/Food.h"
#include <limits>

static bool readInt(int &value)
{
    cin >> value;
    if(!cin){
        cin.clear();
        cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        return false;
    }
    return true;
}

static Role readRole()
{
    while (true) {
        int raw{};
        if (!readInt(raw)) {
            cin.clear();
            continue;
        }
        Role value = static_cast<Role>(raw);
        if (value == Role::Customer
            || value == Role::Restaurateur
            || value == Role::SystemAdmin
            || value == Role::Quit
            || value == Role::ClearScreen) {
            return value;
        }
        Printer::InvalidInput();
    }
}

static CustomerAction readCustomerAction()
{
    while (true) {
        cout << "Select an option: ";
        int raw{};
        if (!readInt(raw)) {
            cin.clear();
            continue;
        }
        CustomerAction value = static_cast<CustomerAction>(raw);
        if (value == CustomerAction::ClearScreen
            ||value == CustomerAction::Quit
            || value == CustomerAction::PlaceOrder
            || value == CustomerAction::ViewMyOrders
            || value == CustomerAction::DebugStorage) {
            return value;
        }
        Printer::InvalidInput();
    }
}

static RestaurateurAction readRestaurateurAction()
{
    while (true) {
        cout << "Select an option: ";
        int raw{};
        if (!readInt(raw)) {
            cin.clear();
            continue;
        }
        RestaurateurAction value = static_cast<RestaurateurAction>(raw);
        if (value == RestaurateurAction::ClearScreen
            || value == RestaurateurAction::EditRestaurantName
            || value == RestaurateurAction::EditRestaurantAddress
            || value == RestaurateurAction::ActivateRestaurant
            || value == RestaurateurAction::DeactivateRestaurant
            || value == RestaurateurAction::EditRestaurantMinutes
            || value == RestaurateurAction::EditRestaurantPhone
            || value == RestaurateurAction::EditRestaurantBio
            || value == RestaurateurAction::AddItemToMenu
            || value == RestaurateurAction::RemoveItemFromMenu
            || value == RestaurateurAction::AddOrderToQueue
            || value == RestaurateurAction::RemoveOrderFromQueue
            || value == RestaurateurAction::SetOrderStatus
            || value == RestaurateurAction::PrintSaleStatistics
            || value == RestaurateurAction::PrintCustomerStatistics
            || value == RestaurateurAction::ShowCurrentOrders
            || value == RestaurateurAction::ShowOrderHistory
            || value == RestaurateurAction::ShowMenu
            || value == RestaurateurAction::EditRestaurateurName
            || value == RestaurateurAction::EditManagedRestaurant
            ||value == RestaurateurAction::Quit
            || value == RestaurateurAction::DebugStorage) {
            return value;
        }
        Printer::InvalidInput();
    }
}

static AdminAction readAdminAction()
{
    while (true) {
        cout << "Select an option: ";
        int raw{};
        if (!readInt(raw)) {
            cin.clear();
            continue;
        }
        AdminAction value = static_cast<AdminAction>(raw);
        if (value == AdminAction::ClearScreen
            || value == AdminAction::Quit
            || value == AdminAction::CreateRestaurant
            || value == AdminAction::ActivateRestaurant
            || value == AdminAction::DeactivateRestaurant
            || value == AdminAction::CreateRestaurateur
            || value == AdminAction::PrintTotalSaleStatistics
            || value == AdminAction::PrintTotalCustomerStatistics
            || value == AdminAction::PrintRestaurantSaleStatistics
            || value == AdminAction::PrintRestaurantCustomerStatistics
            || value == AdminAction::PrintMembershipLevelReport
            || value == AdminAction::ChangeCustomerMembership
            || value == AdminAction::ShowMembershipLevelHistory
            || value == AdminAction::DebugStorage) {
            return value;
        }
        Printer::InvalidInput();
    }
}

static void printInvalidInputAndFlush()
{
    Printer::InvalidInput();
    cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

Role GetInf::loginRule()
{
    return readRole();
}

bool GetInf::customer(Customer &buffer){
    CustomerStorage storage;
    string givenID;

    while(true){
        cout << "Enter your customer ID, 'new' to create an account, or q to quit: ";
        cin >> givenID;
        if(givenID == "q" || givenID == "Q" || givenID == "quit" || givenID == "QUIT"){
            return false;
        }

        if(givenID == "new" || givenID == "NEW" || givenID == "create" || givenID == "CREATE"){
            Customer newCustomer = GetInf::customerFactory();
            if(storage.saveCustomer(newCustomer)){
                buffer = newCustomer;
                cout << endl << "----------------------------------------" << endl;
                cout << "New customer created. Your ID is: " << newCustomer.getID() << endl;
                cout << "----------------------------------------" << endl;
                return true;
            }
            cout << "Could not create customer at this time. Try again." << endl;
            continue;
        }
        if(storage.isValidCustomer(givenID)){
            buffer = storage.giveCustomer(givenID);
            return true;
        }
        cout << "Invalid ID. ";
    }
}

string GetInf::customerName(){
    while (true) {
        cout << "Enter your name: ";
        string enteredName ;
        getline(cin >> std::ws, enteredName);
        if (!enteredName.empty()) {
            return enteredName;
        }
        Printer::InvalidInput();
    }
}

Customer GetInf::customerFactory()
{
    return Customer(customerName());
}

CustomerAction GetInf::customerAction()
{
    return readCustomerAction();
}

Restaurant* GetInf::restaurant()
{
    RestaurantStorage storage;
    RestID_tp restaurantID = chooseRestaurant();
    Restaurant* result = new Restaurant(storage.giveRestaurant(restaurantID));
    return result;
}

Restaurant GetInf::newRestaurant()
{
    string menuID;
    string name;
    string address;
    string phone;
    string bio;
    size_t minutes;

    cout << "Enter menu ID: ";
    cin >> menuID;
    cout << "Enter restaurant name: ";
    getline(cin >> std::ws, name);
    cout << "Enter address: ";
    getline(cin >> std::ws, address);
    cout << "Enter phone number: ";
    getline(cin >> std::ws, phone);
    cout << "Enter bio: ";
    getline(cin >> std::ws, bio);
    cout << "Enter preparation minutes: ";
    cin >> minutes;

    return Restaurant(menuID, name, address, phone, bio, minutes);
}

RestID_tp GetInf::chooseRestaurant()
{
    RestaurantStorage storage;
    RestID_tp restaurantID;
    cin >> restaurantID;
    while(!storage.isValidRestaurant(restaurantID)){
        Printer::InvalidInput();
        cin >> restaurantID;
    }
    return restaurantID;
}

string GetInf::modifyRestaurantString(RestaurateurAction choosenOption)
{
    string value;
    switch(choosenOption){
        case RestaurateurAction::EditRestaurantName:
            cout << "Enter the new restaurant name: ";
            break;
        case RestaurateurAction::EditRestaurantAddress:
            cout << "Enter the new restaurant address: ";
            break;
        case RestaurateurAction::EditRestaurantPhone:
            cout << "Enter the new phone number: ";
            break;
        case RestaurateurAction::EditRestaurantBio:
            cout << "Enter the new bio: ";
            break;
        case RestaurateurAction::EditRestaurateurName:
            cout << "Enter the new restaurateur name: ";
            break;
        case RestaurateurAction::EditManagedRestaurant:
            cout << "Enter the new managed restaurant ID: ";
            break;
        default:
            cout << "Enter an ID/value: ";
            break;
    }
    getline(cin >> std::ws, value);
    return value;
}

size_t GetInf::modifyRestaurantTime(RestaurateurAction choosenOption)
{
    (void)choosenOption;
    size_t minutes{};
    cout << "Enter preparation minutes: ";
    cin >> minutes;
    return minutes;
}

Restaurateur GetInf::restaurateur()
{
    RestaurateurStorage storage;
    ManagerID_tp givenID;

    while(true){
        cout << "Enter your ID: ";
        cin >> givenID;
        if(givenID == "q" || givenID == "Q" || givenID == "quit" || givenID == "QUIT"){
            return Restaurateur();
        }
        if(storage.isValidRestaurateur(givenID)){
            return storage.giveRestaurateur(givenID);
        }
        cout << "Invalid ID. ";
    }
}

Restaurateur GetInf::restaurateurFactory()
{
    string restaurateurName;
    cout << "Enter restaurateur name: ";
    getline(cin >> std::ws, restaurateurName);

    string restaurantID;
    cout << "Enter managed restaurant ID (or N if none): ";
    cin >> restaurantID;
    if (restaurantID == "n" || restaurantID == "N" || restaurantID == "none" || restaurantID == "NONE" || restaurantID == "-") {
        return Restaurateur({}, restaurateurName);
    }
    return Restaurateur(restaurantID, restaurateurName);
}

RestaurateurAction GetInf::restaurateurAction()
{
    return readRestaurateurAction();
}

Restaurateur GetInf::findRestaurant(RestID_tp restaurantID)
{
    RestaurateurStorage storage;
    if (storage.getRestaurantID("TestRestaurateur") == restaurantID) {
        return storage.giveRestaurateur("TestRestaurateur");
    }
    return Restaurateur();
}

bool GetInf::admin(AdminOfSystem &buffer, AdminID_tp &adminID)
{
    AdminStorage storage;
    AdminID_tp givenID;

    while(true){
        cout << "Enter admin ID: ";
        cin >> givenID;
        if(givenID == "q" || givenID == "Q" || givenID == "quit" || givenID == "QUIT"){
            buffer = AdminOfSystem({}, "Quit");
            return false;
        }
        if(storage.isValidAdmin(givenID)){
            buffer = storage.giveAdmin(givenID);
            adminID = givenID;
            return true;
        }
        cout << "Invalid ID. ";
    }
}

CustID_tp GetInf::customerID()
{
    CustID_tp id;
    cin >> id;
    return id;
}

Level GetInf::membershipLevel()
{
    while (true) {
        cout << "Enter new membership level (0=Normal, 1=Silver, 2=Gold, 3=VIP): ";
        int raw{};
        if (!readInt(raw)) {
            cin.clear();
            continue;
        }
        if (raw == static_cast<int>(Level::Normal)
            || raw == static_cast<int>(Level::Silver)
            || raw == static_cast<int>(Level::Gold)
            || raw == static_cast<int>(Level::VIP)) {
            return static_cast<Level>(raw);
        }
        Printer::InvalidInput();
    }
}

point GetInf::membershipPoints()
{
    point value{};
    while (true) {
        cout << "Enter membership points: ";
        cin >> value;
        if (cin) {
            return value;
        }
        cin.clear();
        cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        Printer::InvalidInput();
    }
}

AdminAction GetInf::adminOptions(const vector<ManagerID_tp> &restaurateurIDs)
{
    (void)restaurateurIDs;
    return readAdminAction();
}

MenuID_tp GetInf::menu(RestID_tp restaurantID)
{
    RestaurantStorage storage;
    return storage.getMenuID(restaurantID);
}

MenuItem* GetInf::menuItem()
{
    ItemType type;
    string name;
    string bio;
    cost price{};
    double quantity{};

    while(true){
        cout << "Choose item type (1. Food, 2. Drink): ";
        int rawType{};
        if(!readInt(rawType)){
            continue;
        }
        type = static_cast<ItemType>(rawType);
        if(type == ItemType::Food || type == ItemType::Drink){
            break;
        }
        printInvalidInputAndFlush();
    }

    cout << "Enter item name: ";
    getline(cin >> std::ws, name);
    cout << "Enter item price: ";
    cin >> price;
    if(type == ItemType::Food){
        cout << "Enter the weight(kg):";
    }else{
        cout << "Enter the volume(Litre):";
    }
    cin >> quantity;
    cout << "Enter item description: ";
    getline(cin >> std::ws, bio);

    if(type == ItemType::Drink){
        return new Drink(name, price, quantity, bio);
    }
    return new Food(name, price, quantity, bio);
}

ItemID_tp GetInf::menuItemID()
{
    ItemID_tp itemID;
    cout << "Enter item ID: ";
    cin >> itemID;
    return itemID;
}

bool GetInf::addItemToCart(MenuID_tp menuID, Order& resultOrder)
{
    ItemID_tp itemID;
    cin >> itemID;
    if(itemID == "q" || itemID == "Q" || itemID == "quit" || itemID == "QUIT"){
        return false;
    }
    MenuStorage storage;
    if (!storage.isValidMenu(menuID) || !storage.giveMenu(menuID).has(itemID)){
        Printer::InvalidInput();
        return true;
    }
    double quantity{};
    if(storage.type(menuID, itemID)==ItemType::Food){
        cout << "Enter the weight (kg):";
    }if(storage.type(menuID, itemID)==ItemType::Drink){
        cout << "Enter the volume (Litre):";
    }
    cin >> quantity;

    Menu menu = storage.giveMenu(menuID);
    for (const auto& item : menu.getMenu()){
        if(item->getID() == itemID){
            if(!item->isAvailable(quantity) || quantity <= 0){
                cout << "Item is currently unavailable for the requested amount." << endl;
                return true;
            }
            MenuStorage mstorage;
            if(!mstorage.reduceItemQuantity(menuID, itemID, quantity)){
                cout << "There isn't enough in the stock for your request." << endl;
                return false;
            }
            resultOrder.addItem(menuID, itemID, quantity);
            cout << "Current total price: " << resultOrder.getTotalPrice() << endl;
            return true;
        }
    }
    return true;
}

OrderID_tp GetInf::OrderID(RestaurateurAction option)
{
    OrderStorage storage;
    OrderID_tp orderID;

    while(true){
        switch(option){
            case RestaurateurAction::RemoveOrderFromQueue:
                cout << "Enter the ID of the order want to delete: ";
                break;
            // case RestaurateurAction::ReplaceOrderInQueue:
            //     cout << "Enter the ID of the order want to replace: ";
            //     break;
            default:
                cout << "Enter the ID of the order: ";
        }
        cin >> orderID;

        if(orderID == "q" || orderID == "Q" || orderID == "quit" || orderID == "QUIT"){
            return "";
        }

        if (storage.isValidOrder(orderID)){
            return orderID;
        }
        Printer::InvalidInput();
    }   
}

bool GetInf::orderOut(RestID_tp restaurantID, Order& buffer)
{
    (void)restaurantID;
    return !buffer.getOrder().empty();
}


OrderStatus GetInf::orderStatus(OrderID_tp& buffer)
{
    while (true)
    {        
        OrderID_tp tempID;
        Printer::orderID();
        cin >> tempID;
        Printer::orderStatus();
        int stat;
        cin >> stat;
        buffer = tempID;
        switch(stat){
            case 1:
                return OrderStatus::InPreparation;
            case 2:
                return OrderStatus::ReadyToSend;
            case 3:
                return OrderStatus::Delivered;
            case 0:
                return OrderStatus::Cancelled;
            default:
                continue;
        }
    }
}
