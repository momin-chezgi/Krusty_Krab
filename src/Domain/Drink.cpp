# include "Domain/Drink.h"

Drink::Drink(string initName, cost initPrice, double initVolume, string initBio) : 
MenuItem(initName, initPrice, initBio) {
    setItemType("Drink");
    setVolume(initVolume);
}
Drink::Drink(
    string initName,
    cost initPrice,
    double initVolume,
    string initBio,
    size_t initPreparationMinutes
) : MenuItem(initName, initPrice, initBio) {
    setItemType("Drink");
    setVolume(initVolume);
    setPreparationMinutes(initPreparationMinutes);
}
Drink::Drink(Drink *d) : MenuItem(d->getName(), d->getPricePerUnit(), d->getBio()) {
    setItemType("Drink");
    setVolume(d->getVolume());
}
Drink::~Drink() {}


double Drink::getVolume() const {
    return volume;
}

bool Drink::isAvailable(double requested) const {
    return volume >= requested && requested >= 0.0000;
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
    if(getVolume() >= quantity && quantity >= 0.000){
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
