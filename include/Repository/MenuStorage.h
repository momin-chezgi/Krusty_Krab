# pragma once

# include "Common/Types.h"
# include "Domain/Menu.h"

class MenuStorage{
public:
    bool has(MenuID_tp menuID, ItemID_tp itemID);
    bool isValidMenu(MenuID_tp menuID);
    bool saveMenu(const Menu& newMenu);  // returns true if the menu has been saved successfully, otherwise false
    Menu giveMenu(MenuID_tp menuID);  // returns the menu with the given ID, if it doesn't exist, it returns a default menu with no items
    bool deleteMenu(MenuID_tp menuID);  // returns true if the menu with the given ID exists and has been deleted, otherwise returns false
    bool updateMenu(const Menu& updatingMenu);  // returns true if the menu with the given ID exists and has been updated, otherwise returns false
    bool addItem(MenuID_tp menuID, const MenuItem& newItem);
    bool deleteItem(MenuID_tp menuID, ItemID_tp itemID);
    MenuItem* cloneItem(ItemID_tp itemID);
private:
    static map<MenuID_tp, Menu> menus;
};
