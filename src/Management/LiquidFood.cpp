# include "LiquidFood.h"

LiquidFood::LiquidFood(   ) : MenuItem() {}
LiquidFood::LiquidFood(   string initName, cost initPrice, double initVolume, string initBio) : 
MenuItem(initName, initPrice, initBio) {
    setVolume(initVolume);
}
double LiquidFood::getVolume() const {
    return volume;
}

bool LiquidFood::isAvailable() const {
    return volume > 0;
}

void LiquidFood::setVolume(double newVolume) {
    volume = newVolume;
}

bool LiquidFood::addFoodQuantity(double quantity) {
    if(quantity < 0){
        return false;
    }
    setVolume(getVolume() + quantity);
    return true;
}


bool LiquidFood::delFoodQuantity(double quantity) {
    if(getVolume() >= quantity){
        setVolume(getVolume() - quantity);
        return true;
    }
    return false;
}

bool LiquidFood::setFoodQuantity(double quantity) {
    if(quantity < 0){
        return false;
    }
    setVolume(quantity);
    return true;
}

void LiquidFood::clrFoodQuantity() {
    setVolume(0);
}

LiquidFood::~LiquidFood() {}
