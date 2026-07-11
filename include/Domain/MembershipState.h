#pragma once

#include <string>

#include "Common/Types.h"
#include "Enums.h"

class MembershipState {
protected:
    Level level{Level::Normal};
    point startingPoint{};
    point endingPoint{};
    point myPoint{};
    double multiplier{1.0};
    double offPercentage{};
    double deliveryDiscount{};
    size_t lotteryTicket{};

public:
    MembershipState(
        Level l,
        point sp,
        point ep,
        point mp,
        double m,
        double op,
        double dd,
        size_t lt
    );
    virtual ~MembershipState() = default;

    virtual MembershipState* clone() const = 0;
    virtual std::string getLevelName() const = 0;
    virtual std::string getDeliveryBenefit() const = 0;

    virtual Level getLevel() const;
    virtual Level getMyLevel() const;
    void setLevel(Level l);

    virtual point getStartingPoint() const;
    void setStartingPoint(point sp);

    virtual point getEndingPoint() const;
    void setEndingPoint(point ep);

    virtual point getMyPoint() const;
    void setMyPoint(point mp);

    virtual double getMultiplier() const;
    void setMultiplier(double m);

    virtual double getOffPercentage() const;
    virtual double getoffPercentage() const;
    void setOffPercentage(double op);

    virtual double getDeliveryDiscount() const;
    void setDeliveryDiscount(double dd);

    virtual size_t getLotteryTicket() const;
    void setLotteryTicket(size_t lt);
};
