#include "Interface.h"


int Login()
{
    Printer::wellcome();
    return GetInf::loginRule();
}

Customer* enterAsCustomer(){
    return GetInf::customer();
}

Menu giveMenu(){
    return GetInf::menu();
}

bool orderAtGivenBuffer(const Order const *resultOrder){
    return GetInf::order(resultOrder);
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
    
}

Restaurateur findForRestaurant(size_t givenID)
{
    return GetInf::findRestaurant(givenID);
}