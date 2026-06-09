# pragma once

# include "Domain/Restaurateur.h"

// typedef map<string, Restaurateur *> ResturateurMap; // first: the name of the option, second: the description of the option

class AdminOfSystem{
    AdminID_tp id = IDGenerator::uuid();
    string name;
    vector<ManagerID_tp> restaurateurIDs{};
    map<RestID_tp, map<ItemID_tp, cost>> totalSaleStatistics{};
    map<RestID_tp, map<CustID_tp, vector<OrderID_tp>>> totalCustomerStatistics{};


    bool hasRestaurateur(ManagerID_tp restaurateurID) const
    {
        return find(restaurateurIDs.begin(), restaurateurIDs.end(), restaurateurID) != restaurateurIDs.end();
    }

    // restaurateur accesses:
    // All of the accesses of the restaurateur are done via the 'Restaurateur' class, so we just return a pointer to the restaurant
    Restaurateur *accessRestaurant(ManagerID_tp restauraurateurID);   // Under-destruction :)
    // That time we don't use pointer,
    // instead we get the restaurant id via restaurateur id
    // and then edit/access 

public:
    AdminOfSystem() = default;
    AdminOfSystem(const vector<ManagerID_tp> &initRestaurateurIDs, string initName="");
    AdminOfSystem(const AdminOfSystem& other) = default;
    ~AdminOfSystem();

    AdminOfSystem& operator=(const AdminOfSystem& newAdmin);

    // Getters and Setters:---------------------------------

    // itself:
    AdminID_tp getID() const;
    string getName() const;

    void setName(string newName);



    // restaurateurs:
    
    vector<ManagerID_tp> getRestaurateurIDs() const;

    bool addRestaurateur(ManagerID_tp restaurateurID);
    // addRestaurateur returns true if the restaurantID hasn't been in the restaurateurs
    bool removeRestaurateur(ManagerID_tp restaurateurID);
    // removeRestaurateur returns true if the restaurantID has been in the restaurateurs
    bool replaceRestaurateur(ManagerID_tp previousRestaurteurID, ManagerID_tp newRestaurateurID);
    // replaceRestaurateur returns true if the previous restaurantID has been in the restaurateurs
    // (distinguished by id) and the new restaurant



    // restaurant:

    // There is a very important rule that you CAN'T 
    // define a restaurant without a restauratur, 
    // so the restaurant is added after adding a restaurateur
    // BUT DOESN'T REMOVE AFTER REMOVING A RESTAURATEUR,
    // it will be LOST.
    // So be careful when you remove a restaurateur
    // and take sure that you don't need the restaurant anymore 
    // OR you have added another restaurateur for the restaurant before removing the previous restaurateur
    RestID_tp addRestaurant(ManagerID_tp restaurateurID, const Restaurant& copyingRestaurant, string initName = "");    
    bool removeRestaurant(RestID_tp restaurantID);

    // restaurateur options:
        
        // itself:
        string getRestaurteurName(ManagerID_tp restaurateurID) const;

        bool setRestaurateurName(ManagerID_tp restaurateurID, string newName);


        // restaurant:
        RestID_tp getRestaurantID(ManagerID_tp restaurateurID) const;
        string getRestaurantName(RestID_tp restaurantID) const;
        string getRestaurantAddress(RestID_tp restaurantID) const;
        bool getRestaurantStatus(RestID_tp restaurantID) const;
        size_t getRestaurantPreparationMinutes(RestID_tp restaurantID) const;
        string getRestaurantPhone(RestID_tp restaurantID) const;
        string getRestaurantBio(RestID_tp restaurantID) const;

        bool editRestaurantName(ManagerID_tp restaurateurID, string newName);
        bool editRestaurantAddress(ManagerID_tp restaurateurID, string newAddress);
        bool activateRestaurant(ManagerID_tp restaurateurID);
        bool deactivateRestaurant(ManagerID_tp restaurateurID);
        bool setPreparationTime(ManagerID_tp restaurateurID, const size_t minutes);
        bool setPhoneNumber(ManagerID_tp restaurateurID, const string newPhoneNumber);
        bool setBio(ManagerID_tp restaurateurID, const string newBio);


        // menu:
        MenuID_tp getMenuID(ManagerID_tp restaurateurID) const;

        bool addItemToMenu(ManagerID_tp restaurateurID, const MenuItem* item);
        // 'addItemToMenu' returns true if the item wasn't in the menu and has been added
        bool removeItemFromMenu(ManagerID_tp restaurateurID, ItemID_tp itemID);
        // 'removeItemFromMenu' returns true if the item was in the menu and has been deleted
        bool replaceItemInMenu(ManagerID_tp restaurateurID, ItemID_tp previousItemID, const MenuItem* replacedItem);
        // 'replaceItemInMenu' returns true if the previous item has been in the menu and the replaced item hasn't been in the menu and carefully replaced


        // order:
        bool addItemToOrder(ManagerID_tp restaurateurID, OrderID_tp orderID, ItemID_tp itemID, double quantity = 1);
        bool removeItemFromOrder(ManagerID_tp restaurateurID, OrderID_tp orderID, ItemID_tp itemID);   


        // queue:
        vector<OrderID_tp> getOrderHistoryIDs(ManagerID_tp restaurateurID) const;

        bool addOrderToQueue(ManagerID_tp restaurateurID, OrderID_tp orderID);
        // addOrderToQueue returns true if the order hasn't been in the orders(distinguished by id) and carefully added
        bool removeOrderFromQueue(ManagerID_tp restaurateurID, OrderID_tp orderID);
        // removeOrderFromQueue returns true if the order has been in the orders(distinguished by id) and carefully removed
        bool replaceOrderInQueue(ManagerID_tp restaurateurID, OrderID_tp previousOrderID, OrderID_tp newOrderID);
        // replaceOrderInQueue returns true if the previous order has been in the orders(distinguished by id) and the new order hasn't

    // statistics:
    void updateAndPrintTotalSaleStatistics();
    void updateAndPrintTotalCustomerStatistics();
};

// Admin adds restaurant via adding a restaurateur
// and removes a restaurant via removing a restaurateur
// This method enables to have all of the abilities of the
// restaurateur into the restaurant
