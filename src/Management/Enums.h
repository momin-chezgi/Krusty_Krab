#pragma once
# include "../neededLibs.h"


enum class Role { Customer, RestaurantManager, SystemAdmin };

enum class RestaurantStatus { Active, Inactive };

enum class ItemType { Food, Drink };


enum OrderStatus{
    InPreparation,
    ReadyToSend,
    Delivered,
    Cancelled
};