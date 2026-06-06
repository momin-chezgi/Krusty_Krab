
#include "Repository/RestaurateurStorage.h"

Restaurateur RestaurateurStorage::giveRestaurateur(ManagerID_tp restaurateurID)
{
    auto it = restaurateurs.find(restaurateurID);
    if (it != restaurateurs.end()) {
        return it->second;
    }
    return Restaurateur("NotFound"); 
}
