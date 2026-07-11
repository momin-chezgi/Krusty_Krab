#include "UI/Interface.h"

Role Login()
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

AdminOfSystem enterAsAdmin(AdminID_tp &adminID)
{
    AdminOfSystem buffer;
    if (GetInf::admin(buffer, adminID)) {
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

bool orderOut(MenuID_tp menuID, Order& resultOrder)
{
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
    return true;
}

CustomerAction customerActions(
    const Customer& user,
    const MembershipLevel::Report& loyaltySummary
)
{
    Printer::CustomerDashboard(user, loyaltySummary);
    Printer::CustomerChoices();
    return GetInf::customerAction();
}

RestaurateurAction restaurateurOptions(string restName, RestID_tp restID,string givenName, const string& givenAddress, bool givenStatus, size_t givenMinutes){
    Printer::restaurateurDashboard(restName, restID, givenName, givenAddress,givenStatus, givenMinutes);
    Printer::RestaurateurChoices();
    return GetInf::restaurateurAction();
}

AdminAction adminOptions(vector<ManagerID_tp> restaurateurIDs){
    Printer::adminDashboard(restaurateurIDs);
    Printer::adminChoices();
    return GetInf::adminOptions(restaurateurIDs);
}

Restaurateur findForRestaurant(RestID_tp givenID)
{
    return GetInf::findRestaurant(givenID);
}
