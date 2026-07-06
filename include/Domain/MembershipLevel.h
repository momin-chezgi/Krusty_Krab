#pragma once

# include <iostream>
# include <ctime>
# include <map>
# include "Enums.h"

class MembershipLevel{
protected:
    level myLevel {level::Normal};
    point startingPoint {};
    point endingPoint {};
    point myPoint {};
    double multiplier {1.0};
    double offPercentage {};
    double deliveryDiscount {};
    size_t lotteryTicket {};

    std::map<time_t, level> levelLog;
    // It updates by the prrevious class' levelLog;
public:
    MembershipLevel(
         point sp,
         point ep,
         point mp,
         double m, 
         double op, 
         double dd, 
         size_t lt,
         level l
        );
    virtual ~MembershipLevel() = default;

    point getStartingPoint();
    level getMyLevel();
    double getMultiplier();
    double getoffPercentage();
    double getDeliveryDiscount();
    size_t getLotteryTicket();
    
    level upgrade(cost orderedPrice);
    level downgrade(cost cancelledPrice);

};

// lottery business logic, WON'T BE IMPLEMENTED IN THIS PHASE (phase 2)

point cost2point(cost price);
cost point2cost(point thePoint);
