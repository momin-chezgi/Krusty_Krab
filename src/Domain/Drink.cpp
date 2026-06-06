# include "Domain/Drink.h"

Drink::Drink(string initName, cost initPrice, double initVolume, string initBio) : 
MenuItem(initName, initPrice, initBio) {
    setVolume(initVolume);
}
Drink::Drink(Drink *d) : MenuItem(d->getName(), d->getPricePerUnit(), d->getBio()) {
    setVolume(d->getVolume());
}
Drink::~Drink() {}


double Drink::getVolume() const {
    return volume;
}

bool Drink::isAvailable() const {
    return volume > 0;
}

void Drink::setVolume(double newVolume) {
    volume = newVolume;
}

bool Drink::addItemQuantity(double quantity) {
    if(quantity < 0){
        return false;
    }
    setVolume(getVolume() + quantity);
    return true;
}


bool Drink::delItemQuantity(double quantity) {
    if(getVolume() >= quantity){
        setVolume(getVolume() - quantity);
        return true;
    }
    return false;
}

bool Drink::setItemQuantity(double quantity) {
    if(quantity < 0){
        return false;
    }
    setVolume(quantity);
    return true;
}

void Drink::clrItemQuantity() {
    setVolume(0);
}

MenuItem* Drink::clone() const
{   
    return new Drink(*this);
}
