#pragma once

# include "Common/Types.h"
# include "Domain/Customer.h"
# include "Domain/Order.h"
# include "Domain/Menu.h"
# include "Domain/Restaurant.h"
# include "Domain/Admin.h"




class InOut{
    virtual int reader() = 0;
    virtual int writer() = 0;
};

class SQLReader : public InOut{

public:
    Menu getMenu();
    bool getCustomer(CustID_tp CustomerID, Customer &buffer);  // returns true if the customer with the given ID exists and saves it in buffer, otherwise returns false
    bool getRestaurant(RestID_tp restaurantID, Restaurant &buffer);  // like getCustomer() function
    // You can also add the password authentication process to getRestaurant and getCustomer functions
    bool isValidRestaurant(RestID_tp restaurantID);    
};

class SQLWriter : public InOut{
};
