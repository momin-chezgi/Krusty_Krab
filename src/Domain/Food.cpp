# include "Domain/Food.h"

Food::Food(Food* f) : MenuItem(f->getName(), f->getPricePerUnit(), f->getBio()) {
    setItemType("Food");
    setWeight(f->getWeight());
}
Food::Food(string initName, cost initPrice, double initWeight, string initBio) : 
MenuItem(initName, initPrice, initBio) {
    setItemType("Food");
    setWeight(initWeight);
}

Food::Food(
    string initName,
    cost initPrice,
    double initWeight,
    string initBio,
    size_t initPreparationMinutes,
    string initFoodType
) : MenuItem(initName, initPrice, initBio) {
    setItemType("Food");
    setWeight(initWeight);
    setPreparationMinutes(initPreparationMinutes);
    setFoodType(initFoodType);
}

double Food::getWeight() const {
    return weight;
}

bool Food::isAvailable(double requested) const {
    return weight >= requested && requested >= 0.0000;
}

void Food::setWeight(double newWeight) {
    weight = newWeight;
}

bool Food::addItemQuantity(double quantity) {
    if(quantity < 0){
        return false;
    }
    setWeight(getWeight() + quantity);
    return true;
}
bool Food::delItemQuantity(double quantity) {
    if(getWeight() >= quantity && quantity > 0){
        setWeight(getWeight() - quantity);
        return true;
    }
    return false;
}
bool Food::setItemQuantity(double quantity) {
    if(quantity < 0){
        return false;
    }
    setWeight(quantity);
    return true;
}
void Food::clrItemQuantity() {
    setWeight(0);
}

MenuItem* Food::clone() const {
    return new Food(*this);
}

Food::~Food() {}
