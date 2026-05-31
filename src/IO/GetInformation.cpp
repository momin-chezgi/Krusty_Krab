#include "GetInformation.h"

bool GetInf::customer(Customer &buffer){
    
    CustomerStorage storage;
    //SQLReader reader;

    string givenID;

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

bool GetInf::restaurateur(Restaurateur &buffer){
    RestaurateurStorage storage;
    //SQLReader reader;

    string givenID;

    while(true){
        cout << "Enter your ID: ";
        cin >> givenID;
        if(givenID == "q" || givenID == "Q" || givenID == "quit" || givenID == "QUIT"){
            return false;
        }
        //if (reader.getRestaurateur(givenID, buffer)){
        if(storage.isValidRestaurateur(givenID)){
            buffer = storage.giveRestaurateur(givenID);
            return true;
        }
        cout << "Invalid ID, ";
    }
    // We can also add the password authentication process here
    // But for now, we just return the restaurateur with the given ID
}

string GetInf::customerName(){
    cout << "What is your name? ";
    string enteredName;
    cin >> enteredName;
    return enteredName;
}
int GetInf::loginRule()
{
    int chosenRule{-1};
    cin >> chosenRule;
    while(chosenRule < 1 || chosenRule > 3){
        Printer::InvalidInput();
        cin >> chosenRule;
    }
    return chosenRule;
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

string GetInf::chooseRestaurant()
{
    // SQLReader reader;
    RestaurantStorage storage;


    string restaurantID;

    cin >> restaurantID;
    // while(!reader.isValidRestaurant(restaurantID)){
    while(!storage.isValidRestaurant(restaurantID)){
        Printer::InvalidInput();
        cin >> restaurantID;
    }
    return restaurantID;
}

bool GetInf::orderOut(Order& buffer)
{
    
}

string GetInf::menu(string restaurantID)
{
    RestaurantStorage storage;

    Restaurant restaurant = storage.giveRestaurant(restaurantID);
    return restaurant.getMenu().getID();
}

bool GetInf::addItemToCart(string menuID, Order& resultOrder)
{
    string itemID;
    cin >> itemID;
    if(itemID == "q" || itemID == "Q" || itemID == "quit" || itemID == "QUIT"){
        return false;
    }
    MenuStorage storage;
    // SQLReader reader;

    // if (!reader.isValidMenuItem(itemID)){
    if (!storage.isValidMenu(menuID) || !storage.giveMenu(menuID).thereIsThatItem(itemID)){
        Printer::InvalidInput();
        return true; 
        // we return true because the user can still
        //  add items to the cart
    }
    double quantity;
    cout << "Enter the quantity: ";
    cin >> quantity;

    vector <MenuItem*> menu = storage.giveMenu(menuID).getMenu();
    for (const auto& item : menu){
        if(item->getID() == itemID){
             if(!item->isAvailable() || !item->addItemQuantity(quantity)){
                cout << "Sorry, the item is not available in the kitchen right now" << endl;
                return true; 
                // we return true because the user can still
                // add items to the cart
            }
            resultOrder.addItem(*item, quantity);
            return true;
        }
    }

    return true;
}
