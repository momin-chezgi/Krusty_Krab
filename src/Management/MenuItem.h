#pragma once

# include "../neededLibs.h"
# include "../IO/Interface.h"
# include "../IO/InOut.h"

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
    MenuItem(string initName, cost initPrice, string initBio = "");

    string getID() const;
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
    virtual bool addItemQuantity(double quantity) = 0;
    virtual bool delItemQuantity(double quantity) = 0;
    virtual bool setItemQuantity(double quantity) = 0;
    virtual void clrItemQuantity() = 0;
    virtual MenuItem* clone() const = 0;

    virtual ~MenuItem();
protected:
    string id = IDGenerator::uuid();
    
    string itemType{};      // For the type of food(i.e. Pizza)

    string foodType{};      // For more informations (i.e. Vegie Pizza, ...)
};

typedef pair<MenuItem*, double> OrderLine;
