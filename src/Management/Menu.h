#pragma once
# include "MenuItem.h"
# include "../Utils/IDGenerator.h"
class Menu
{
    string id = IDGenerator::uuid();
    vector<MenuItem> menu;

    void clear();
    void copyFrom(const Menu& other);
public:
    Menu(Menu *m);
    Menu(const vector<MenuItem> &initMenu);
    Menu(const Menu& other);
    Menu& operator=(const Menu& other);
    
    string getID() const;
    vector<MenuItem> getMenu() const;

    void setMenu(const vector< MenuItem>& newMenu);

    bool addItem(const MenuItem& newItem);       // If the item hasn't been in the menu returns true, otherwise false
    bool removeItem(string removingItemID);       // If the item has been in the menu returns true, otherwise false
    
    bool has(string itemID) const;     // returns true if there is an item with the given ID in the menu, otherwise false

    ~Menu();
};
