# pragma once

#include "MenuItem.h"

class Food : public MenuItem
{
    double weight{};
public:
    Food(size_t initID);
    Food(size_t initID, string initName, cost initPrice, double initWeight, string initBio = "");

    double getWeight() const;
    bool isAvailable() const override;
    bool addFoodQuantity(double quantity) override;
    bool delFoodQuantity(double quantity) override;
    bool setFoodQuantity(double quantity) override;
    void clrFoodQuantity() override;

    void setWeight(double newWeight);
    ~Food();
};