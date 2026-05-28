# include "Food.h"

Food::Food(size_t initID) : MenuItem(initID) {}
Food::Food(size_t initID, string initName, cost initPrice, double initWeight, string initBio) : 
MenuItem(initID, initName, initPrice, initBio) {
    setWeight(initWeight);
}

double Food::getWeight() const {
    return weight;
}

bool Food::isAvailable() const {
    return weight > 0;
}

void Food::setWeight(double newWeight) {
    weight = newWeight;
}

bool Food::addFoodQuantity(double quantity) {
    if(quantity < 0){
        return false;
    }
    setWeight(getWeight() + quantity);
    return true;
}
bool Food::delFoodQuantity(double quantity) {
    if(getWeight() >= quantity){
        setWeight(getWeight() - quantity);
        return true;
    }
    return false;
}
bool Food::setFoodQuantity(double quantity) {
    if(quantity < 0){
        return false;
    }
    setWeight(quantity);
    return true;
}
void Food::clrFoodQuantity() {
    setWeight(0);
}

void Food::setWeight(double newWeight) {
    weight = newWeight;
}
Food::~Food() {}
