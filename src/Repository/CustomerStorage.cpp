
#include "Repository/CustomerStorage.h"

Customer CustomerStorage::giveCustomer(CustID_tp customerID)
{
    auto it = customers.find(customerID);
    if (it != customers.end()) {
        return it->second;
    }
    return Customer("NotFound"); 
    // We can't use [] operator to find in map 
    // because if the key doesn't exist, 
    // it will create a new element with default value, 
    // but we want to return a default customer with 'NotFound' name 
    // and no orders 
}

bool CustomerStorage::isValidCustomer(CustID_tp customerID)
{
    auto it = customers.find(customerID);
    return it != customers.end();
}
