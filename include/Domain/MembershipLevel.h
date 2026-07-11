#pragma once

# include <iostream>
# include <ctime>
# include <string>
# include <map>
# include "Common/Types.h"
# include "Enums.h"
# include "MembershipState.h"

class Order;

class MembershipLevel{
public:
    class Report {
    public:
        Level level{Level::Normal};
        std::string levelName;
        point currentPoints{};
        point pointsToNextLevel{};
        double discountPercent{};
        double deliveryDiscountRate{};
        std::string deliveryBenefit;
        size_t lotteryTickets{};

        cost baseTotal{};
        cost discountAmount{};
        cost baseDeliveryFee{};
        cost deliveryDiscountAmount{};
        cost finalDeliveryFee{};
        cost finalTotal{};
        point earnedPoints{};

        bool levelUp{};
        Level previousLevel{Level::Normal};
        Level nextLevel{Level::Normal};
    };
protected:
    MembershipState* currentState{};

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
    explicit MembershipLevel(MembershipState* state);
    MembershipLevel(const MembershipLevel& other);
    MembershipLevel& operator=(const MembershipLevel& other);
    virtual ~MembershipLevel();

    point getStartingPoint() const;
    void setStartingPoint(point sp);
    point getEndingPoint() const;
    void setEndingPoint(point ep);
    point getMyPoint() const;
    void setMyPoint(point mp);
    Level getMyLevel() const;
    void setMyLevel(Level level);
    double getMultiplier() const;
    void setMultiplier(double m);
    double getoffPercentage() const;
    void setOffPercentage(double op);
    double getDeliveryDiscount() const;
    void setDeliveryDiscount(double dd);
    size_t getLotteryTicket() const;
    void setLotteryTicket(size_t lt);
    std::string getLevelName() const;
    std::string getDeliveryBenefit() const;
    void setState(MembershipState* state);
    MembershipState* getState() const;

    bool saveForCustomer(CustID_tp custID) const;
    bool updateForCustomer(CustID_tp custID) const;
    static MembershipLevel loadForCustomer(CustID_tp custID);
    static bool deleteForCustomer(CustID_tp custID);
    
    Level upgrade(cost orderedPrice);
    Level upgrade(CustID_tp custID, cost orderedPrice);
    Level downgrade(cost cancelledPrice);
    Level downgrade(CustID_tp custID, cost cancelledPrice);

};

// Unified membership view type used for summary, checkout, and transition results.

// lottery business logic, WON'T BE IMPLEMENTED IN THIS PHASE (phase 2)

std::string levelToString(Level level);
MembershipState* createStateByLevel(Level level, point points);
MembershipState* createStateByPoints(point points);
MembershipLevel::Report buildSummary(const MembershipLevel& membership);
MembershipLevel::Report buildCheckoutSummary(const MembershipLevel& membership, const Order& order);
bool isUpgrade(Level before, Level after);
bool ensureRecordForCustomer(const CustID_tp& customerID, MembershipLevel& membership);
bool persistMembershipForCustomer(const CustID_tp& customerID, const MembershipLevel& membership);
MembershipLevel loadMembershipForCustomer(const CustID_tp& customerID);
std::string upgradeMessage(Level before, Level after);
MembershipLevel applyOrderToMembership(const MembershipLevel& membership, cost orderBaseTotal);
MembershipLevel::Report applyOrder(const MembershipLevel& membership, cost orderBaseTotal);

point cost2point(cost price);
cost point2cost(point thePoint);
