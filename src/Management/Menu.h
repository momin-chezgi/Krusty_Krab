#pragma once
# include "MenuItem.h"
# include "Utils/IDGen.h"
class Menu
{
    const string id = IDGen::uuid();
    vector<MenuItem> menu;
public:
    Menu(Menu *m);
    Menu(const vector<MenuItem> &initMenu);
    
    string getID() const;
    vector<MenuItem> getMenu() const;

    void setMenu(const vector< MenuItem>& newMenu);

    bool addItem(const MenuItem const *newItem);       // If the item hasn't been in the menu returns true, otherwise false
    bool removeItem(const MenuItem const *removingItem);       // If the item has been in the menu returns true, otherwise false
    
    ~Menu();
};