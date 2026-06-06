#include "UI/Interface.h"
#include "Repository/OrderStorage.h"
#include "Repository/RestaurantStorage.h"


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

AdminOfSystem enterAsAdmin()
{
    AdminOfSystem buffer;
    if (GetInf::admin(buffer)) {
        return buffer;
    }
    return AdminOfSystem({}, "Quit");
}

RestID_tp chooseRestaurant(){
    Printer::chooseRestaurant();
    return GetInf::chooseRestaurant();
}
MenuID_tp giveMenu(RestID_tp restaurantID){
    return GetInf::menu(restaurantID);
}

bool orderOut(RestID_tp RestaurantID, MenuID_tp menuID, Order& resultOrder){
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
    restaurantStorage.addOrderToRestaurant(RestaurantID, resultOrder.getID());
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

Restaurateur findForRestaurant(RestID_tp givenID)
{
    return GetInf::findRestaurant(givenID);
}
