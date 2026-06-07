# pragma once

# include "Common/Types.h"
# include "Domain/Customer.h"

class CustomerStorage{
public:
    bool isValidCustomer(CustID_tp customerID);
    Customer giveCustomer(CustID_tp customerID);  
    // returns the customer with the given ID, if it doesn't exist, it returns a default customer with empty name and no orders
    bool saveCustomer(const Customer& newCustomer);
    // returns true if the customer has been saved successfully, otherwise false
    bool updateCustomer(const Customer& updatingCustomer);
    // returns true if the customer with the given ID exists and has been updated, otherwise false
    bool deleteCustomer(CustID_tp customerID);
    // returns true if the customer with the given ID exists and has been deleted, otherwise returns false
    
private:
    static map<CustID_tp, Customer> customers;
};
