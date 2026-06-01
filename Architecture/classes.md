# C++ Mini-Project

_Date: May 14, 2026 / Ordibehesht 24 1405_

## Restaurant:                                  class
* attributes:
1. id                                                   size_t
2. name                                             string
3. address : {City, Street, Building No}           vector
4. activationStatus = {active, not_active}          bool
5. standardTimeOfPreparation         size_t per minutes
6. phoneNumber                               string
7. bio                                                 string
8. Orders                                           vector of Orders

## Menu:                                           class
* attributes:
1. id           size_t
2. menu     vector of MenuItems
## Order:                                           class
* attributes:
1. id           size_t
2. orderer  Customer*
3. order     vector of MenuItems
## MenuItem:                                    class
* attributes:
1. id                                                   size_t
2. name                                             string
3. bio                                                 string
4. baseCost                                      double
5. isAvailable                                    bool
> Note: classes like beverage, water, food, dessert, ... inherit from menuIte,
## Customer:                                    class
* attributes:
1. id                           size_t
2. name                     string 
3. myOrders                                                           vector of Orders
* functions:
1. void order (Order* o);                                              
2. Menu  getMenu () const;                                       
3. Order  getMyOrder () const;                                
4. bool deleteFromMyOrder (MenuItem mi)               
5. bool addToMyOrder (MenuItem mi )                       
6. vector of Orders HistoryOfOrders () const                
7. bool isReady (MenuItem mi)                                  

## Restaurateur:                                     class

> About term: A restaurateur is a person who opens and runs restaurants professionally. Although over time the term has come to describe any person who owns a restaurant, it traditionally refers to a highly skilled professional who is proficient in all aspects of the restaurant business. (Source: Wikipedia)

* attributes:
1. id        size_t
2. name     string
3. restaurant                                   Restaurant
4. saleStatisics map of MenuItems and their total purchases 
5. customerStatistics map of Customers and their order history 

* functions:
0. Restaurant* myRestaurant () const;            
1. size_t getRestaurantID () const;             
2. string getRestaurantName () const;       
3. vector getRestaurantAddress () const;                       
4. bool  getRestaurantStatus () const;                          
5. Menu  getMenu () const
6. vector of Orders  getCurrentOrders () const

7. void editRestaurantID (size_t newID);         
8. void editRestaurantName (string newName);     
9. void editRestaurantAddress (vector newAddress);   
10. void editRestaurantStatus (bool isActive);       

11.  bool addItemToMenu (MenuItem mi );          
12. bool removeItemFromMenu (MenuItem mi)  
13. bool replaceItemInMenu (MenuItem previousItem, replacedItem)  

14. bool editThisOrder (Order* previousOrder, replacedOrder)  

15. void updateSaleStatistics ();
16. void updateCustomersStatistics ();

## AdminOfSystem                                            class
* attributes:
0. id               size_t
1. name         string
2. restaurants                                  vector of 
3. totalSaleStatistics map of restaurnts and their sale statistics 
4. totalCustomerStatistics map of restaurants and their customer statistics
Restaurant *

* functions:
0. Restaursnt* addRestaurant();
1. Restaursnt* addRestaurant(Restaurnt r);
2. Restaurant* addRestaurant( size_t id, string name,vector address , string phoneNumber, string bio)

3. void updateSaleStatistics ();
4. void updateCustomerStatistics ();