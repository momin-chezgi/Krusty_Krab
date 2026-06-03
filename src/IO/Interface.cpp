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

Restaurateur enterAsRestaurateur()
{
    return GetInf::restaurateur();
}

AdminOfSystem enterAsAdmin(AdminOfSystem &buffer)
{
    return GetInf::admin(buffer);
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

int restaurateurOptions(string givenName, const string& givenAddress, bool givenStatus, size_t givenMinutes){
    Printer::restaurateurDashboard(givenName, givenAddress,givenStatus, givenMinutes);
    Printer::RestaurateurChoices();
    return GetInf::restaurateurAction();
}


int adminOptions(vector<ManagerID_tp> restaurateurIDs){
    Printer::adminDashboard(restaurateurIDs);
    return GetInf::adminOptions(restaurateurIDs);
}
{
    Printer::adminDashboard();
    return 0;
}

Restaurateur findForRestaurant(string givenID)
{
    return GetInf::findRestaurant(givenID);
}