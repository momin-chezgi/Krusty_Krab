#include "Domain/MenuItem.h"

MenuItem::MenuItem(string initName, cost initPrice, string initBio){
    setName(initName);
    setPricePerUnit(initPrice);
    setBio(initBio);
}

ItemID_tp MenuItem::getID() const{
    return id;
}
size_t MenuItem::getPreparationMinutes() const{
    return preparationMinutes;
}
string MenuItem::getName() const{
    return name;
}
string MenuItem::getBio() const{
    return bio;
}
cost MenuItem::getPricePerUnit() const{
    return pricePerUnit;
}

void MenuItem::setPreparationMinutes(size_t minutes)
{
    preparationMinutes = minutes;
}
void MenuItem::setName(string newName)
{
    if(newName != ""){
        name = newName;
    }
}
void MenuItem::setBio(string newBio){
    if(newBio != ""){
        bio = newBio;
    }
}
void MenuItem::setPricePerUnit(cost newPrice){
    if(newPrice != 0.00){
        pricePerUnit = newPrice;
    }
}

string MenuItem::getItemType() const {
    return itemType;
}
string MenuItem::getFoodType() const {
    return foodType;
}


cost MenuItem::calculatePrice(double quantity) {
    cost foodPrice = quantity * getPricePerUnit();
    cost otherPrices = 0.00;
    // 'otherPrices' may vary from containers price to electricity, water, etc 
    // bills per customer.
    return foodPrice + otherPrices;
}

void MenuItem::setItemType(string newType) {
    itemType = newType;
}

void MenuItem::setFoodType(string newType) {
    foodType = newType;
}

MenuItem::~MenuItem(){}
