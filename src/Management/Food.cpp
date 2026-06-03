# include "Food.h"

Food::Food(Food* f) : MenuItem(f->getName(), f->getPricePerUnit(), f->getBio()) {
    setWeight(f->getWeight());
}
Food::Food(string initName, cost initPrice, double initWeight, string initBio) : 
MenuItem(initName, initPrice, initBio) {
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

bool Food::addItemQuantity(double quantity) {
    if(quantity < 0){
        return false;
    }
    setWeight(getWeight() + quantity);
    return true;
}
bool Food::delItemQuantity(double quantity) {
    if(getWeight() >= quantity){
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
