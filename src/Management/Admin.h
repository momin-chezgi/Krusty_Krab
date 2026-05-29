# pragma once

# include "Restaurateur.h"


class AdminOfSystem{
    const const string id = IDGen::uuid();
    string name;
    map<string, Restaurateur *> restaurateurs;
    map<Restaurant *, map<MenuItem, cost>> totalSaleStatistics{};
    map<Restaurant *, map<Customer, vector<Order>>> totalCustomerStatistics{};

public:
    AdminOfSystem(string initName="");
    AdminOfSystem(string initName="");
    AdminOfSystem(const vector<Restaurateur *> &initRestaurateurs);

    string getID() const;
    string getName() const;
    Restaurateur *addRestaurant();
    //  Maybe this would be better to return the id instead of the pointer,
    // but we can find the restaurant by its id in the map of restaurateurs
    Restaurateur *addRestaurant(const Restaurant& copyingRestaurant, string initName = "");
    Restaurateur *addRestaurant(string initName, vector<string> initAddress, string initPhoneNumber);

    Restaurateur *accessRestaurant(string restaurantID);

    void updateAndPrintTotalSaleStatistics();
    void updateAndPrintTotalCustomerStatistics();

    ~AdminOfSystem();
};

// Admin adds restaurant via adding a restaurateur
// and removes a restaurant via removing a restaurateur
// This method enables to have all of the abilities of the
// restaurateur into the restaurant