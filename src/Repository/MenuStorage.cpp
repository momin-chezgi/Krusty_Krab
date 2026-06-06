
#include "Repository/MenuStorage.h"
#include "Domain/Drink.h"
#include "Domain/Food.h"

map<MenuID_tp, Menu> MenuStorage::menus = [] {
    map<MenuID_tp, Menu> result;
    Menu testMenu;
    Drink testDrink("Krusty Cola", 10.0, 20.0, "Cold drink");
    Food testFood("Krabby Patty", 25.0, 20.0, "Classic burger");
    testMenu.addItem(&testDrink);
    testMenu.addItem(&testFood);
    result.emplace("TestMenu", testMenu);
    return result;
}();

bool MenuStorage::has(MenuID_tp menuID, ItemID_tp itemID)
{
    auto it = menus.find(menuID);
    return it != menus.end() && it->second.has(itemID);
}

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

bool MenuStorage::saveMenu(const Menu& newMenu)
{
    return menus.insert({newMenu.getID(), newMenu}).second;
}

bool MenuStorage::deleteMenu(MenuID_tp menuID)
{
    return menus.erase(menuID) > 0;
}

bool MenuStorage::updateMenu(const Menu& updatingMenu)
{
    auto it = menus.find(updatingMenu.getID());
    if (it == menus.end()) {
        return false;
    }
    it->second = updatingMenu;
    return true;
}

bool MenuStorage::addItem(MenuID_tp menuID, const MenuItem * newItem)
{
    auto it = menus.find(menuID);
    if (it == menus.end()) {
        return false;
    }
    return it->second.addItem(newItem);
}

bool MenuStorage::deleteItem(MenuID_tp menuID, ItemID_tp itemID)
{
    auto it = menus.find(menuID);
    if (it == menus.end()) {
        return false;
    }

    vector<MenuItem*> items = it->second.getMenu();
    auto itemIt = find_if(items.begin(), items.end(), [itemID](const MenuItem* item) {
        return item && item->getID() == itemID;
    });
    if (itemIt == items.end()) {
        return false;
    }

    return it->second.removeItem(*itemIt);
}

MenuItem* MenuStorage::cloneItem(ItemID_tp itemID)
{
    for (const auto& menuEntry : menus) {
        vector<MenuItem*> items = menuEntry.second.getMenu();
        auto itemIt = find_if(items.begin(), items.end(), [itemID](const MenuItem* item) {
            return item && item->getID() == itemID;
        });
        if (itemIt != items.end()) {
            return (*itemIt)->clone();
        }
    }
    return nullptr;
}
