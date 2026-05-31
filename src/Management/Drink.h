# pragma once

# include "MenuItem.h"

class Drink : public MenuItem
{
    double volume{};

public:
    Drink();
    Drink(Drink *d);
    Drink(string initName, cost initPrice, double initVolume, string initBio = "");

    double getVolume() const;
    bool isAvailable() const override;

    void setVolume(double newVolume);
    bool addItemQuantity(double quantity) override;
    bool delItemQuantity(double quantity) override;
    bool setItemQuantity(double quantity) override;
    void clrItemQuantity() override;
    MenuItem* clone() const override;

    ~Drink();
};