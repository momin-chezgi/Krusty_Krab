#pragma once
# include "MenuItem.h"
# include "IDInitializer.h"
class Menu
{
    size_t id;
    vector<MenuItem> menu;
public:
    Menu(size_t initID, Menu *m);
    Menu(size_t initID, const vector<MenuItem> &initMenu);
    
    size_t getID() const;
    vector<MenuItem> getMenu() const;

    void setID(const size_t newID);
    void setMenu(const vector< MenuItem>& newMenu);

    bool addItem(const MenuItem const *newItem);       // If the item hasn't been in the menu returns true, otherwise false
    bool removeItem(const MenuItem const *removingItem);       // If the item has been in the menu returns true, otherwise false
    
    ~Menu();
};