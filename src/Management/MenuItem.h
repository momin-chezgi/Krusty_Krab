#pragma once

# include "../neededLibs.h"
# include "../IO/Interface.h"

class MenuItem
{
private:
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

    cost calculatePrice(double quantity);

    void setPreparationMinutes(size_t minutes);
    void setName(string newName);
    void setBio(string newBio);
    void setPricePerUnit(cost newPrice);
    void setItemType(string newType);
    // We don't have 'setIsAvailable' because it just depends on the quantity
    
    virtual string getItemType();
    virtual string getFoodType();
    virtual bool isAvailable() const = 0;
    virtual bool addFoodQuantity(double quantity) = 0;
    virtual bool delFoodQuantity(double quantity) = 0;
    virtual bool setFoodQuantity(double quantity) = 0;
    virtual void clrFoodQuantity() = 0;

    ~MenuItem();
protected:
    size_t id{};
    // For the type of food(i.e. Pizza)
    string itemType{};
    // For more informations (i.e. Vegie Pizza, ...)
    string foodType{};
};
