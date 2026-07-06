#pragma once

# include <iostream>
# include <ctime>
# include <map>
# include "Common/Types.h"
# include "Enums.h"

class MembershipLevel{
protected:
    Level myLevel {Level::Normal};
    point startingPoint {};
    point endingPoint {};
    point myPoint {};
    double multiplier {1.0};
    double offPercentage {};
    double deliveryDiscount {};
    size_t lotteryTicket {};

    std::map<time_t, Level> levelLog;
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
         Level l
        );
    virtual ~MembershipLevel() = default;

    point getStartingPoint() const;
    point getEndingPoint() const;
    point getMyPoint() const;
    Level getMyLevel() const;
    double getMultiplier() const;
    double getoffPercentage() const;
    double getDeliveryDiscount() const;
    size_t getLotteryTicket() const;

    bool saveForCustomer(CustID_tp custID) const;
    bool updateForCustomer(CustID_tp custID) const;
    static MembershipLevel loadForCustomer(CustID_tp custID);
    static bool deleteForCustomer(CustID_tp custID);
    
    Level upgrade(cost orderedPrice);
    Level upgrade(CustID_tp custID, cost orderedPrice);
    Level downgrade(cost cancelledPrice);
    Level downgrade(CustID_tp custID, cost cancelledPrice);

};

// lottery business logic, WON'T BE IMPLEMENTED IN THIS PHASE (phase 2)

point cost2point(cost price);
cost point2cost(point thePoint);
