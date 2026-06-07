
#include "Repository/CustomerStorage.h"

map<CustID_tp, Customer> CustomerStorage::customers = {
    {"TestCustomer", Customer("TestCustomer")}
};

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

bool CustomerStorage::saveCustomer(const Customer& newCustomer)
{
    return customers.insert({newCustomer.getID(), newCustomer}).second;
}

bool CustomerStorage::updateCustomer(const Customer& updatingCustomer)
{
    auto it = customers.find(updatingCustomer.getID());
    if (it == customers.end()) {
        return false;
    }
    it->second = updatingCustomer;
    return true;
}

bool CustomerStorage::deleteCustomer(CustID_tp customerID)
{
    return customers.erase(customerID) > 0;
}

map<CustID_tp, Customer> CustomerStorage::giveAllCustomers() const
{
    return customers;
}
