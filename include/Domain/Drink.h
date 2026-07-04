# pragma once

# include "Domain/MenuItem.h"

class Drink : public MenuItem
{
    double volume{};

public:
    Drink(Drink *d);
    Drink(string initName, cost initPrice, double initVolume, string initBio = "");
    Drink(
        ItemID_tp initID,
        string initName,
        cost initPrice,
        double initVolume,
        string initBio,
        size_t initPreparationMinutes
    );
    Drink(
        string initName,
        cost initPrice,
        double initVolume,
        string initBio,
        size_t initPreparationMinutes
    );

    double getVolume() const;
    bool isAvailable(double requested) const override;

    void setVolume(double newVolume);
    bool addItemQuantity(double quantity) override;
    bool delItemQuantity(double quantity) override;
    bool setItemQuantity(double quantity) override;
    void clrItemQuantity() override;
    MenuItem* clone() const override;

    ~Drink();
};
