# include "Domain/MembershipLevel.h"

MembershipLevel::MembershipLevel(point sp, point ep, point mp, double m, double op, double dd, size_t lt, level l) :
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

point MembershipLevel::getStartingPoint()
{
    return startingPoint;
}
level MembershipLevel::getMyLevel()
{
    return myLevel;
}
double MembershipLevel::getMultiplier()
{
    return multiplier;
}
double MembershipLevel::getoffPercentage()
{
    return offPercentage;
}
double MembershipLevel::getDeliveryDiscount()
{
    return deliveryDiscount;
}
size_t MembershipLevel::getLotteryTicket()
{
    return lotteryTicket;
}


level MembershipLevel::upgrade(cost orderedPrice)
{
    myPoint += cost2point(orderedPrice)*getMultiplier();
    if (myPoint > endingPoint){
        return static_cast<level>(static_cast<int>(myLevel) + 1);
    }
    return myLevel;
}
// remember you should invoke this function JUST like this to work correctly:
/*
x.upgrade(cost ...)
while(x.getMyLevel != upgrade(0));
*/
level MembershipLevel::downgrade(cost cancelledPrice)
{
    myPoint -= cost2point(cancelledPrice)*getMultiplier();
    if (myPoint > endingPoint){
        return static_cast<level>(static_cast<int>(myLevel) - 1);
    }
    return myLevel;
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
