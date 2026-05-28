# pragma once

# include "MenuItem.h"

class LiquidFood : public MenuItem
{
    double volume{};

public:
    LiquidFood(size_t initID);
    LiquidFood(size_t initID, string initName, cost initPrice, double initVolume, string initBio = "");

    double getVolume() const;
    bool isAvailable() const override;

    void setVolume(double newVolume);
    bool addFoodQuantity(double quantity) override;
    bool delFoodQuantity(double quantity) override;
    bool setFoodQuantity(double quantity) override;
    void clrFoodQuantity() override;
    
    ~LiquidFood();
};