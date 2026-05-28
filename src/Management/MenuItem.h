#pragma once

# include "../neededLibs.h"
# include "../IO/Interface.h"

class MenuItem
{
private:
    size_t id{};
    size_t preparationMinutes{};
    string name{};
    // For the type of food(i.e. Pizza)
    string bio{};
    // For more informations (i.e. Vegie Pizza, ...)
    cost pricePerUnit{};
public:
    MenuItem(size_t initID);
    MenuItem(size_t initID, string initName, cost initPrice, string initBio = "");

    size_t getID() const;
    size_t getPreparationMinutes() const;
    string getName() const;
    string getBio() const;
    cost getPricePerUnit() const;

    virtual string getItemType() const = 0;
    virtual string getFoodType() const = 0;
    virtual bool isAvailable() const = 0;

    cost calculatePrice(double quantity, string container);

    void setID(size_t newID);
    void setPreparationMinutes(size_t minutes);
    void setName(string newName);
    void setBio(string newBio);
    void setPricePerUnit(cost newPrice);

    virtual void setItemType(string newType) = 0;
    // We don't have 'setIsAvailable' because it just depends on the quantity

    ~MenuItem();
};
