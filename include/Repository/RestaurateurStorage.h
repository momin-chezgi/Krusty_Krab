# pragma once

# include "Common/Types.h"
# include "Domain/Restaurateur.h"

class RestaurateurStorage{
public:
    string getName(ManagerID_tp restaurateurID) const;
    RestID_tp getRestaurantID(ManagerID_tp restaurateurID) const;

    bool setName(ManagerID_tp restaurateurID, string newName);
    bool updateRestaurateur(const Restaurateur& updatingRestaurateur);
    bool isValidRestaurateur(ManagerID_tp restaurateurID);
    Restaurateur giveRestaurateur(ManagerID_tp restaurateurID);  
    // returns the restaurateur with the given ID, if it doesn't exist, it returns a default restaurateur with empty name and no restaurant
    bool saveRestaurateur(const Restaurateur& newRestaurateur);
    // returns true if the restaurateur has been saved successfully, otherwise false
    bool deleteRestaurateur(ManagerID_tp restaurateurID);
    // returns true if the restaurateur with the given ID exists and has been deleted, otherwise returns false
    map<ManagerID_tp, Restaurateur> giveAllRestaurateurs() const;
};
