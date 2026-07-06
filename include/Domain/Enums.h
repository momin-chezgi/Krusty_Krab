#pragma once
# include "Common/Types.h"

enum class Role : int {
    ClearScreen = -1,
    Quit = 0,
    Customer = 1,
    Restaurateur = 2,
    SystemAdmin = 3,
};

enum class RestaurantStatus : int {
    Inactive = 0,
    Active = 1
};

enum class ItemType : int {
    Food = 1,
    Drink = 2,
    None = -1,
};

enum class OrderStatus : int {
    InPreparation = 1,
    ReadyToSend = 2,
    Delivered = 3,
    Cancelled = 0,
};

enum class CustomerAction : int {
    ClearScreen = -1,
    Quit = 0,
    PlaceOrder = 1,
    ViewMyOrders = 2,
    DebugStorage = 404,
};

enum class RestaurateurAction : int {
    ClearScreen = -1,
    Quit = 0,
    // restaurant:
    EditRestaurantName = 1,
    EditRestaurantAddress = 2,
    ActivateRestaurant = 3,
    DeactivateRestaurant = 4,
    EditRestaurantMinutes = 5,
    EditRestaurantPhone = 6,
    EditRestaurantBio = 7,
    // menu:
    AddItemToMenu = 11,
    RemoveItemFromMenu = 12,
    // queue:
    AddOrderToQueue = 21,
    RemoveOrderFromQueue = 22,
    SetOrderStatus = 23,
    // Monitoring:
    PrintSaleStatistics = 31,
    PrintCustomerStatistics = 32,
    ShowCurrentOrders = 33,
    ShowOrderHistory = 34,
    ShowMenu = 35,
    // own profile:
    EditRestaurateurName = 41,
    EditManagedRestaurant = 42,
    DebugStorage = 404

};

enum class AdminAction : int {
    ClearScreen = -1,
    Quit = 0,
    CreateRestaurant = 1,
    ActivateRestaurant = 2,
    DeactivateRestaurant = 3,
    CreateRestaurateur = 4,
    PrintTotalSaleStatistics = 31,
    PrintTotalCustomerStatistics = 32,
    PrintRestaurantSaleStatistics = 33,
    PrintRestaurantCustomerStatistics = 34,
    PrintMembershipLevelReport = 41,
    ChangeCustomerMembership = 42,
    ShowMembershipLevelHistory = 43,
    DebugStorage = 404
};


enum class Level : int {
    Normal = 0,
    Silver = 1,
    Gold = 2,
    VIP = 3,
};

enum class Achievement : int {
    FrequentBuyer = 1,
    NightCustomer = 2,
    LaunchEater = 3,
    EarlyBird = 4,
};
