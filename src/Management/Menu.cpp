#include "Menu.h"

Menu::Menu(size_t initID, Menu *m)
{
    setID(initID);
    setMenu(m->getMenu());
    // no suitable user-defined conversion from 
    // "std::vector<MenuItem, std::allocator<MenuItem>>" 
    // to "const std::vector<const MenuItem, std::allocator<const MenuItem>>" 
    // exists
}
Menu::Menu(size_t initID, const vector<MenuItem> &initMenu)
{
    setID(initID);
    setMenu(initMenu);
}

size_t Menu::getID() const 
{
    return id;
}
vector<MenuItem> Menu::getMenu() const
{
    return menu;
}

void Menu::setID(const size_t newID)
{
    if(IDInitializer::Menu(1, newID)==1){
        id = newID;
    }
}
void Menu::setMenu(const vector< MenuItem>& newMenu)
{
    menu = newMenu;
}
bool Menu::addItem(const MenuItem const *newItem)
{
    auto iter = find_if(menu.begin(), menu.end(), [newItem](const MenuItem &mi)
                      { return mi.getName() == newItem->getName(); });
    if(iter != menu.end()){
        return false;
    }
    menu.push_back(*newItem);
    return true;
}
bool Menu::removeItem(const MenuItem const *removingItem)
{
    auto iter = find_if(menu.begin(), menu.end(), [removingItem](const MenuItem &mi)
                        { return mi.getName() == removingItem->getName(); });
    if (iter == menu.end())
    {
        return false;
    }
    menu.erase(iter);
    return true;
}

Menu::~Menu()
{
}