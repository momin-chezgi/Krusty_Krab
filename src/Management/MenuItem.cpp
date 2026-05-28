#include "MenuItem.h"

MenuItem::MenuItem(size_t initID){
    setID(initID);
}
MenuItem::MenuItem(size_t initID, string initName, cost initPrice, string initBio = ""){
    setID(initID);
    setName(initName);
    setPricePerUnit(initPrice);
    setBio(initBio);
}

size_t MenuItem::getID() const{
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


void MenuItem::setID(size_t newID){
    if(newID != 0){
        id = newID;
    }
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

cost MenuItem::calculatePrice(double quantity, string container) {
    if(allowedContainers[container]==nullptr){
        return 0.00;
    }
    cost foodPrice = quantity * getPricePerUnit();
    cost otherPrices = allowedContainers[container]->getPrice();
    // 'otherPrices' may vary from containers price to electricity, water, etc bills per customer.
    return foodPrice + otherPrices;
}


MenuItem::~MenuItem(){}
