#include "Interface.h"


int Login()
{
    Printer::wellcome();
    return GetInf::loginRule();
}

bool enterAsCustomer(Customer &buffer)
{
    return GetInf::customer(buffer);
}
string chooseRestaurant(){
    Printer::chooseRestaurant();
    return GetInf::chooseRestaurant();
}
string giveMenu(string restaurantID){
    return GetInf::menu(restaurantID);
}

bool orderOut(string RestaurantID, string menuID, Order& resultOrder){
    OrderStorage storage;
    RestaurantStorage restaurantStorage;
    Printer::orderOut();
    string decision;
    cin >> decision;
    if(decision == "y" || decision == "Y" || decision == "yes" || decision == "YES"){
        bool adding = true;
        while(adding){
            Printer::addItemToCart();
            adding = GetInf::addItemToCart(menuID, resultOrder);
        }
    }
    if (resultOrder.getOrder().empty()) {
        return false;
    }
    storage.saveOrder(resultOrder);
    restaurantStorage.AddOrderToRestaurant(RestaurantID, resultOrder.getID());
    return true;
}

auto restaurateurOptions(string givenName, const vector<string>& givenAddress, bool givenStatus, size_t givenMinutes){
    Printer::restaurateurDashboard(givenName, givenAddress,givenStatus, givenMinutes);
    return GetInf::restaurateurAction();
}
Restaurateur *enterAsRestaurateur()
{
    return GetInf::restaurateur();
}

AdminOfSystem *enterAsAdmin()
{
    return GetInf::admin();
}

auto adminOptions()
{
    Printer::adminDashboard();
}

Restaurateur findForRestaurant(string givenID)
{
    return GetInf::findRestaurant(givenID);
}