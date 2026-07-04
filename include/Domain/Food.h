# pragma once

#include "Domain/MenuItem.h"

class Food : public MenuItem
{
    double weight{};
public:
    Food(Food* f);
    Food(string initName, cost initPrice, double initWeight, string initBio = "");
    Food(
        ItemID_tp initID,
        string initName,
        cost initPrice,
        double initWeight,
        string initBio,
        size_t initPreparationMinutes,
        string initFoodType
    );
    Food(
        string initName,
        cost initPrice,
        double initWeight,
        string initBio,
        size_t initPreparationMinutes,
        string initFoodType
    );

    double getWeight() const;
    bool isAvailable(double requested) const override;
    bool addItemQuantity(double quantity) override;
    bool delItemQuantity(double quantity) override;
    bool setItemQuantity(double quantity) override;
    void clrItemQuantity() override;
    MenuItem* clone() const override;

    void setWeight(double newWeight);
    ~Food();
};
