# include "Domain/MembershipLevel.h"
# include "Repository/MembershipLevelStorage.h"

MembershipLevel::MembershipLevel(point sp, point ep, point mp, double m, double op, double dd, size_t lt, Level l) :
    myLevel (l),
    startingPoint (sp),
    endingPoint (ep),
    myPoint (mp),
    multiplier (m),
    deliveryDiscount (dd),
    lotteryTicket (lt)
{
    if (op >= 0 && op <= 100){
        offPercentage = op;
    }
}

point MembershipLevel::getStartingPoint() const
{
    return startingPoint;
}
point MembershipLevel::getEndingPoint() const
{
    return endingPoint;
}
point MembershipLevel::getMyPoint() const
{
    return myPoint;
}
Level MembershipLevel::getMyLevel() const
{
    return myLevel;
}
double MembershipLevel::getMultiplier() const
{
    return multiplier;
}
double MembershipLevel::getoffPercentage() const
{
    return offPercentage;
}
double MembershipLevel::getDeliveryDiscount() const
{
    return deliveryDiscount;
}
size_t MembershipLevel::getLotteryTicket() const
{
    return lotteryTicket;
}

bool MembershipLevel::saveForCustomer(CustID_tp custID) const
{
    MembershipLevelStorage storage;
    return storage.addMembershipLevel(custID, *this);
}

bool MembershipLevel::updateForCustomer(CustID_tp custID) const
{
    MembershipLevelStorage storage;
    return storage.updateMembershipLevel(custID, *this);
}

MembershipLevel MembershipLevel::loadForCustomer(CustID_tp custID)
{
    MembershipLevelStorage storage;
    return storage.getMembershipLevel(custID);
}

bool MembershipLevel::deleteForCustomer(CustID_tp custID)
{
    MembershipLevelStorage storage;
    return storage.delMembershipLevel(custID);
}

Level MembershipLevel::upgrade(cost orderedPrice)
{
    myPoint += cost2point(orderedPrice)*getMultiplier();
    if (myPoint > endingPoint){
        if (myLevel != Level::VIP) {
            myLevel = static_cast<Level>(static_cast<int>(myLevel) + 1);
        }
    }
    return myLevel;
}

Level MembershipLevel::upgrade(CustID_tp custID, cost orderedPrice)
{
    Level level = upgrade(orderedPrice);
    updateForCustomer(custID);
    return level;
}

// remember you should invoke this function JUST like this to work correctly:
/*
x.upgrade(cost ...)
while(x.getMyLevel != upgrade(0));
*/

Level MembershipLevel::downgrade(cost cancelledPrice)
{
    myPoint -= cost2point(cancelledPrice)*getMultiplier();
    if (myPoint < startingPoint){
        if (myLevel != Level::Normal) {
            myLevel = static_cast<Level>(static_cast<int>(myLevel) - 1);
        }
    }
    return myLevel;
}

Level MembershipLevel::downgrade(CustID_tp custID, cost cancelledPrice)
{
    Level level = downgrade(cancelledPrice);
    updateForCustomer(custID);
    return level;
}

// remember you should invoke this function JUST like this to work correctly:
/*
x.upgrade(cost ...)
while(x.getMyLevel != upgrade(0));
*/

point cost2point(cost price)
{
    return price * 0.4;
}

cost point2cost(point thePoint)
{
    return thePoint * 2.5;
}
