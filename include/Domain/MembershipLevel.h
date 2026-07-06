#pragma once

# include <iostream>
# include <ctime>
# include <string>
# include <map>
# include "Common/Types.h"
# include "Enums.h"

class Order;

struct MembershipSummary {
    Level level{Level::Normal};
    std::string levelName;
    point currentPoints{};
    point pointsToNextLevel{};
    double discountPercent{};
    double deliveryDiscountRate{};
    std::string deliveryBenefit;
    size_t lotteryTickets{};
};

struct CheckoutSummary {
    cost baseTotal{};
    double discountPercent{};
    cost discountAmount{};
    cost baseDeliveryFee{};
    cost deliveryDiscountAmount{};
    cost finalDeliveryFee{};
    cost finalTotal{};
    point earnedPoints{};
};

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

struct MembershipUpdateResult {
    MembershipLevel next;
    bool levelUp{};
    Level previousLevel{Level::Normal};
    Level nextLevel{Level::Normal};
};

// lottery business logic, WON'T BE IMPLEMENTED IN THIS PHASE (phase 2)

std::string levelToString(Level level);
MembershipLevel applyLevelTemplate(Level level, point points);
MembershipSummary buildSummary(const MembershipLevel& membership);
CheckoutSummary buildCheckoutSummary(const MembershipLevel& membership, const Order& order);
bool isUpgrade(Level before, Level after);
bool ensureRecordForCustomer(const CustID_tp& customerID, MembershipLevel& membership);
bool persistMembershipForCustomer(const CustID_tp& customerID, const MembershipLevel& membership);
MembershipLevel loadMembershipForCustomer(const CustID_tp& customerID);
std::string upgradeMessage(Level before, Level after);
MembershipLevel applyOrderToMembership(const MembershipLevel& membership, cost orderBaseTotal);
MembershipUpdateResult applyOrder(const MembershipLevel& membership, cost orderBaseTotal);

point cost2point(cost price);
cost point2cost(point thePoint);
