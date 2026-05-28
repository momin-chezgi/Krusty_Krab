# pragma once

# include "Restaurateur.h"


class AdminOfSystem{
    size_t id;
    string name;
    map<size_t, Restaurateur *> restaurateurs;
    map<Restaurant *, map<MenuItem, cost>> totalSaleStatistics{};
    map<Restaurant *, map<Customer, vector<Order>>> totalCustomerStatistics{};

public:
    AdminOfSystem(size_t initID, string initName="");
    AdminOfSystem(size_t initID, const vector<Restaurateur *> &initRestaurateurs);

    Restaurateur *addRestaurant();
    Restaurateur *addRestaurant(Restaurant *r, string initName = "");
    Restaurateur *addRestaurant(size_t initID, string initName, vector<string> initAddress, string initPhoneNumber);

    Restaurateur *accessRestaurant(size_t restaurantID);

    void updateAndPrintTotalSaleStatistics();
    void updateAndPrintTotalCustomerStatistics();

    ~AdminOfSystem();
};

// Admin adds restaurant via adding a restaurateur
// and removes a restaurant via removing a restaurateur
// This method enables to have all of the abilities of the
// restaurateur into the restaurant