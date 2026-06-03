#include "Menu.h"

Menu::Menu(   Menu *m)
{
    if(m){
        copyFrom(*m);
    }
}

Menu::Menu(const vector<MenuItem *> &initMenu)
{
    setMenu(initMenu);
}

Menu::Menu(const Menu& other)
{
    copyFrom(other);
}

Menu& Menu::operator=(const Menu& other)
{
    if(this != &other){
        clear();
        copyFrom(other);
    }
    return *this;
}

Menu::~Menu()
{
    clear();
}

void Menu::clear()
{
    for (auto item : menu){
        delete item;
    }
    menu.clear();
}

void Menu::copyFrom(const Menu& other)
{
    id = other.id;
    setMenu(other.menu);
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
    clear();
    for(auto item : newMenu){
        if(item){
            menu.push_back(item->clone());
        }
    }
}
bool Menu::addItem(const MenuItem& newItem)
{
    auto iter = find_if(menu.begin(), menu.end(), [&newItem](const MenuItem *mi)
                      { return mi && mi->getName() == newItem.getName(); });
    if(iter != menu.end()){
        return false;
    }
    menu.push_back(newItem.clone());
    return true;
}
bool Menu::removeItem(string removingItemID)
{
    auto iter = find_if(menu.begin(), menu.end(), [removingItemID](const MenuItem *mi)
                        { return mi && mi->getID() == removingItemID; });
    if (iter == menu.end())
    {
        return false;
    }
    delete *iter;
    menu.erase(iter);
    return true;
}

bool Menu::has(string itemID) const
{
    auto iter = find_if(menu.begin(), menu.end(), [itemID](const MenuItem *mi)
                        { return mi && mi->getID() == itemID; });
    return iter != menu.end();
}
