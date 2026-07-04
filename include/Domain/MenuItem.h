#pragma once

# include "Common/Types.h"
# include "Utility/IDGenerator.h"

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
    MenuItem(ItemID_tp initID, string initName, cost initPrice, string initBio = "");

    ItemID_tp getID() const;
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
    void setFoodType(string newType);
    // We don't have 'setIsAvailable' because it just depends on the quantity
    
    virtual string getItemType() const;
    virtual string getFoodType() const;
    virtual bool isAvailable(double requested) const = 0;
    virtual bool addItemQuantity(double quantity) = 0;
    virtual bool delItemQuantity(double quantity) = 0;
    virtual bool setItemQuantity(double quantity) = 0;
    virtual void clrItemQuantity() = 0;
    virtual MenuItem* clone() const = 0;

    virtual ~MenuItem();
protected:
    ItemID_tp id = IDGenerator::uuid();
    string itemType{};      // For the type of food(i.e. Pizza)

    string foodType{};      // For more informations (i.e. Vegie Pizza, ...)
};

typedef pair<MenuItem*, double> OrderLine;
