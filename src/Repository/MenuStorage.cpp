
#include "Repository/MenuStorage.h"
#include "Domain/Drink.h"

map<MenuID_tp, Menu> MenuStorage::menus = [] {
    map<MenuID_tp, Menu> result;
    Menu testMenu;
    testMenu.addItem(Drink("TestMenuItem", 10.0, 20.0));
    result.emplace("TestMenu", testMenu);
    return result;
}();


bool MenuStorage::isValidMenu(MenuID_tp menuID)
{
    auto it = menus.find(menuID);
    return it != menus.end();
}


Menu MenuStorage::giveMenu(MenuID_tp menuID)
{
    auto it = menus.find(menuID);
    if (it != menus.end()) {
        return it->second;
    }
    return Menu();
    // We can't use [] operator to find in map 
    // because if the key doesn't exist, 
    // it will create a new element with default value, 
    // but we want to return a default menu with no items 
}
