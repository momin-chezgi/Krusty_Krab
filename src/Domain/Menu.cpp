#include "Domain/Menu.h"

Menu::Menu(Menu *m)
{
    if (m) {
        copyFrom(*m);
    }
}

Menu::Menu(const vector<MenuItem*> &initMenu)
{
    setMenu(initMenu);
}

Menu::Menu(const Menu& other)
{
    copyFrom(other);
}

Menu& Menu::operator=(const Menu& other)
{
    if (this != &other) {
        clear();
        copyFrom(other);
    }
    return *this;
}

Menu::~Menu()
{
    clear();
}


// privates:

void Menu::clear()
{
    for(auto item : menu) {
        delete item;
    }
    menu.clear();
}

void Menu::copyFrom(const Menu& other)
{
    id = other.id;
    setMenu(other.getMenu());
}



// getters:

MenuID_tp Menu::getID() const
{
    return id;
}

vector<MenuItem*> Menu::getMenu() const
{
    vector<MenuItem*> result;
    result.reserve(menu.size());
    for (const auto& item : menu) {
        result.push_back(item);
    }
    return result;
}


// setters:

void Menu::setMenu(const vector<MenuItem*>& newMenu)
{
    clear();
    for (auto item : newMenu) {
        if (item) {
            menu.emplace_back(item->clone());
        }
    }
}



// modifying menu:

bool Menu::addItem(const MenuItem* newItem)
{
    if (!newItem) {
        return false;
    }

    auto iter = find_if(menu.begin(), menu.end(), [&newItem](const MenuItem* mi)
    {
        return mi && mi->getID() == newItem->getID();
    });

    if (iter != menu.end()) {
        return false;
    }

    menu.emplace_back(newItem->clone());
    return true;
}

bool Menu::removeItem(const MenuItem* item)
{
    if (!item) {
        return false;
    }

    auto iter = find_if(menu.begin(), menu.end(), [item](const MenuItem* mi)
    {
        return mi && mi->getID() == item->getID();
    });

    if (iter == menu.end()) {
        return false;
    }

    delete *iter;
    menu.erase(iter);
    return true;
}



bool Menu::has(ItemID_tp itemID) const
{
    auto iter = find_if(menu.begin(), menu.end(), [itemID](const MenuItem* mi)
    {
        return mi && mi->getID() == itemID;
    });
    return iter != menu.end();
}
