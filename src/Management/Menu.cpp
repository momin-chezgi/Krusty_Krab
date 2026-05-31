#include "Menu.h"

Menu::Menu(   Menu *m)
{
    setMenu(m->getMenu());
}
Menu::Menu(const vector<MenuItem *> &initMenu)
{
    setMenu(initMenu);
}

string Menu::getID() const 
{
    return id;
}
vector<MenuItem *> Menu::getMenu() const
{
    return menu;
}


void Menu::setMenu(const vector< MenuItem*>& newMenu)
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
    menu.push_back(newItem);
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

bool Menu::thereIsThatItem(string itemID) const
{
    auto iter = find_if(menu.begin(), menu.end(), [itemID](const MenuItem &mi)
                        { return mi.getID() == itemID; });
    return iter != menu.end();
}

Menu::~Menu()
{
}