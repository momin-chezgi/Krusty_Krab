#pragma once
# include "Common/Types.h"


enum class Role { Customer, RestaurantManager, SystemAdmin };

enum class RestaurantStatus { Active, Inactive };

enum class ItemType { Food, Drink };


enum OrderStatus{
    InPreparation,
    ReadyToSend,
    Delivered,
    Cancelled
};

enum CustomerOpt{
    OrderOut,
    EditOrder,
    RemoveOrder,

    Quit,
};

enum ManagerOpt{
    EditName,
    EditLinkedRest,
    // restaurant:
    EditRestName,
    EditAddress,
    Activate,
    Deactivate,
    EditMinutes,
    EditPhone,
    EditBio,
    // menu:
    AddItem2Menu,
    DelItemFromMenu,
    ReplaceItemInMenu,
};
// remember to update Admin Options when you update an option in ManagerOpt
enum AdminOpt{
    EditName,
    // restaurant
    AddRest,
    Activate,
    Deactivate,
    
};
