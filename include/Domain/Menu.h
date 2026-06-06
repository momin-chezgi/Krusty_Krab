#pragma once
# include "Domain/MenuItem.h"
# include "Utility/IDGenerator.h"

class Menu
{
    MenuID_tp id = IDGenerator::uuid();
    vector<MenuItem *> menu;

    void clear();
    void copyFrom(const Menu& other);
public:
    Menu() = default;
    Menu(Menu *m);
    Menu(const vector<MenuItem*> &initMenu);
    Menu(const Menu& other);

    Menu& operator=(const Menu& other);
        
    // getters:
    MenuID_tp getID() const;
    vector<MenuItem*> getMenu() const;

    // setters:
    void setMenu(const vector<MenuItem*>& newMenu);

    // modifying menu:
    bool addItem(const MenuItem* newItem);       // If the item hasn't been in the menu returns true, otherwise false
    bool removeItem(const MenuItem* item);       // If the item has been in the menu returns true, otherwise false
    
    bool has(ItemID_tp itemID) const;     // returns true if there is an item with the given ID in the menu, otherwise false

    ~Menu();
    
};
