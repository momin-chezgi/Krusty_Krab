
#include "Repository/RestaurateurStorage.h"

map<ManagerID_tp, Restaurateur> RestaurateurStorage::restaurateurs = {
    {"TestRestaurateur", Restaurateur("TestRestaurant", "TestRestaurateur")}
};

string RestaurateurStorage::getName(ManagerID_tp restaurateurID) const
{
    auto it = restaurateurs.find(restaurateurID);
    return it != restaurateurs.end() ? it->second.getName() : "NotFound";
}

RestID_tp RestaurateurStorage::getRestaurantID(ManagerID_tp restaurateurID) const
{
    auto it = restaurateurs.find(restaurateurID);
    return it != restaurateurs.end() ? it->second.getRestaurantID() : "NotFound";
}

bool RestaurateurStorage::setName(ManagerID_tp restaurateurID, string newName)
{
    auto it = restaurateurs.find(restaurateurID);
    if (it == restaurateurs.end()) {
        return false;
    }
    it->second.setName(newName);
    return true;
}

bool RestaurateurStorage::isValidRestaurateur(ManagerID_tp restaurateurID)
{
    return restaurateurs.find(restaurateurID) != restaurateurs.end();
}

Restaurateur RestaurateurStorage::giveRestaurateur(ManagerID_tp restaurateurID)
{
    auto it = restaurateurs.find(restaurateurID);
    if (it != restaurateurs.end()) {
        return it->second;
    }
    return Restaurateur("NotFound"); 
}

bool RestaurateurStorage::saveRestaurateur(const Restaurateur& newRestaurateur)
{
    return restaurateurs.insert({newRestaurateur.getID(), newRestaurateur}).second;
}

bool RestaurateurStorage::deleteRestaurateur(ManagerID_tp restaurateurID)
{
    return restaurateurs.erase(restaurateurID) > 0;
}
