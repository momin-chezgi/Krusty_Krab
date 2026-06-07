#include "UI/GetInformation.h"
#include "Repository/CustomerStorage.h"
#include "Repository/MenuStorage.h"
#include "Repository/OrderStorage.h"
#include "Repository/RestaurantStorage.h"
#include "Repository/RestaurateurStorage.h"
#include "Domain/Drink.h"
#include "Domain/Food.h"


int GetInf::loginRule()
{
    int chosenRule{-1};
    cin >> chosenRule;
    while(chosenRule < 0 || chosenRule > 3){
        Printer::InvalidInput();
        cin >> chosenRule;
    }
    return chosenRule;
}

// Customer:---------------------------------------------

bool GetInf::customer(Customer &buffer){
    
    CustomerStorage storage;
    //SQLReader reader;

    CustID_tp givenID;

    while(true){
        cout << "Enter your ID: ";
        cin >> givenID;
        if(givenID == "q" || givenID == "Q" || givenID == "quit" || givenID == "QUIT"){
            return false;
        }
        //if (reader.getCustomer(givenID, buffer)){
        if(storage.isValidCustomer(givenID)){
            buffer = storage.giveCustomer(givenID);
            return true;
        }
        cout << "Invalid ID, ";
    }
    // Note: Add user creation---------------
    // We can also add the password authentication process here
    // But for now, we just return the customer with the given ID
}

string GetInf::customerName(){
    cout << "What is your name? ";
    string enteredName;
    cin >> enteredName;
    return enteredName;
}


// restaurant:--------------------------------------------

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
    cin >> name;
    cout << "Enter address: ";
    cin >> address;
    cout << "Enter phone number: ";
    cin >> phone;
    cout << "Enter bio: ";
    cin >> bio;
    cout << "Enter preparation minutes: ";
    cin >> minutes;

    return Restaurant(menuID, name, address, phone, bio, minutes);
}

RestID_tp GetInf::chooseRestaurant()
{
    // SQLReader reader;
    RestaurantStorage storage;


    RestID_tp restaurantID;

    cin >> restaurantID;
    // while(!reader.isValidRestaurant(restaurantID)){
    while(!storage.isValidRestaurant(restaurantID)){
        Printer::InvalidInput();
        cin >> restaurantID;
    }
    return restaurantID;
}

string GetInf::modifyRestaurantString(int choosenOption)
{
    string value;
    switch(choosenOption){
        case 1:
            cout << "Enter the new restaurant name: ";
            break;
        case 2:
            cout << "Enter the new restaurant address: ";
            break;
        case 6:
            cout << "Enter the new phone number: ";
            break;
        case 7:
            cout << "Enter the new bio: ";
            break;
        default:
            cout << "Enter the ID/value: ";
            break;
    }
    cin >> value;
    return value;
}

size_t GetInf::modifyRestaurantTime(size_t choosenOption)
{
    (void)choosenOption;
    size_t minutes{};
    cout << "Enter preparation minutes: ";
    cin >> minutes;
    return minutes;
}


// resturateur:---------------------------------------------------------

Restaurateur GetInf::restaurateur(){
    RestaurateurStorage storage;
    //SQLReader reader;

    ManagerID_tp givenID;

    while(true){
        cout << "Enter your ID: ";
        cin >> givenID;
        if(givenID == "q" || givenID == "Q" || givenID == "quit" || givenID == "QUIT"){
            return Restaurateur("Quit", "Quit");
        }
        //if (reader.getRestaurateur(givenID, buffer)){
        if(storage.isValidRestaurateur(givenID)){
            return storage.giveRestaurateur(givenID);
        }
        cout << "Invalid ID, ";
    }
    // We can also add the password authentication process here
    // But for now, we just return the restaurateur with the given ID
}

int GetInf::restaurateurAction()
{
    int action{-1};
    cin >> action;
    while(action < 1 || action > 23){
        Printer::InvalidInput();
        cin >> action;
    }
    return action;
}

Restaurateur GetInf::findRestaurant(RestID_tp restaurantID)
{
    RestaurateurStorage storage;
    if (storage.getRestaurantID("TestRestaurateur") == restaurantID) {
        return storage.giveRestaurateur("TestRestaurateur");
    }
    return Restaurateur("NotFound", "NotFound");
}


// admin:--------------------------------------------------------------

bool GetInf::admin(AdminOfSystem &buffer)
{
    AdminID_tp givenID;

    while(true){
        cout << "Enter admin ID: ";
        cin >> givenID;
        if(givenID == "q" || givenID == "Q" || givenID == "quit" || givenID == "QUIT"){
            buffer = AdminOfSystem({}, "Quit");
            return false;
        }
        if(givenID == "TestAdmin"){
            buffer = AdminOfSystem({"TestRestaurateur"}, "TestAdmin");
            return true;
        }
        cout << "Invalid ID, ";
    }
}

int GetInf::adminOptions(const vector<ManagerID_tp> &restaurateurIDs)
{
    (void)restaurateurIDs;
    int action{-1};
    cin >> action;
    while(action != 1 && action != 2 && action != 3 &&
          action != 31 && action != 32 && action != 33 && action != 34){
        Printer::InvalidInput();
        cin >> action;
    }
    return action;
}



// menu:--------------------------------------------------------------------------

MenuID_tp GetInf::menu(RestID_tp restaurantID)
{
    RestaurantStorage storage;
    return storage.getMenuID(restaurantID);
}

// menuItem:----------------------------------------------------------------------
MenuItem* GetInf::menuItem()
{
    int type{};
    string name;
    string bio;
    cost price{};
    double quantity{};

    cout << "Choose item type (1. Food, 2. Drink): ";
    cin >> type;
    cout << "Enter item name: ";
    cin >> name;
    cout << "Enter item price: ";
    cin >> price;
    cout << "Enter item quantity/volume: ";
    cin >> quantity;
    cout << "Enter item bio: ";
    cin >> bio;

    if(type == 2){
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

// order:------------------------------------------------------------------------

bool GetInf::addItemToCart(MenuID_tp menuID, Order& resultOrder)
{
    ItemID_tp itemID;
    cin >> itemID;
    if(itemID == "q" || itemID == "Q" || itemID == "quit" || itemID == "QUIT"){
        return false;
    }
    MenuStorage storage;
    // SQLReader reader;

    // if (!reader.isValidMenuItem(itemID)){
    if (!storage.isValidMenu(menuID) || !storage.giveMenu(menuID).has(itemID)){
        Printer::InvalidInput();
        return true; 
        // we return true because the user can still
        //  add items to the cart
    }
    double quantity;
    cout << "Enter the quantity: ";
    cin >> quantity;

    Menu menuObject = storage.giveMenu(menuID);
    vector <MenuItem*> menu = menuObject.getMenu();
    for (const auto& item : menu){
        if(item->getID() == itemID){
             if(!item->isAvailable(quantity) || quantity <= 0){
                cout << "Sorry, the item is not available in the kitchen right now" << endl;
                return true; 
                // we return true because the user can still
                // add items to the cart
            }
            resultOrder.addItem(menuID, itemID, quantity);
            return true;
        }
    }

    return true;
}


OrderID_tp GetInf::OrderID(int option)
{
    OrderStorage storage;
    // SQLReader reader;

    OrderID_tp orderID;

    while(true){
        switch(option){
            case 22:
                cout << "Enter the ID of the order you want to delete: ";
                break;
            case 23:
                cout << "Enter the ID of the order you want to replace: ";
                break;
            default:
                cout << "Enter the ID of the order: ";
        }
        cout << "Enter the ID of the order: ";
        cin >> orderID;

        if(orderID == "q" || orderID == "Q" || orderID == "quit" || orderID == "QUIT"){
            return "";
        }

        // if (reader.isValidOrder(orderID)){
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