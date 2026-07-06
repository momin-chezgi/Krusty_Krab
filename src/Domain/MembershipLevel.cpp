# include "Domain/MembershipLevel.h"
# include "Domain/Order.h"
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

namespace {
    struct LevelTemplate {
        Level level;
        const char* name;
        point startingPoint;
        point endingPoint;
        double multiplier;
        double discountPercent;
        double deliveryDiscountRate;
        size_t lotteryTickets;
        const char* deliveryBenefit;
    };

    const LevelTemplate templates[] = {
        {Level::Normal, "Normal", 0.0, 100.0, 1.0, 0.0, 0.0, 0, "No delivery discount"},
        {Level::Silver, "Silver", 100.0, 300.0, 1.2, 5.0, 0.5, 1, "Reduced delivery fee for high-value orders"},
        {Level::Gold, "Gold", 300.0, 700.0, 1.5, 10.0, 1.0, 2, "Half-price delivery"},
        {Level::VIP, "VIP", 700.0, 1000000.0, 2.0, 15.0, 1.0, 3, "Free delivery"},
    };

    const LevelTemplate& templateFor(Level level)
    {
        const int index = static_cast<int>(level);
        if (index < static_cast<int>(Level::Normal)) {
            return templates[0];
        }
        if (index > static_cast<int>(Level::VIP)) {
            return templates[3];
        }
        return templates[index];
    }

    Level levelFromPoints(point points)
    {
        if (points >= templates[3].startingPoint) {
            return Level::VIP;
        }
        if (points >= templates[2].startingPoint) {
            return Level::Gold;
        }
        if (points >= templates[1].startingPoint) {
            return Level::Silver;
        }
        return Level::Normal;
    }
}

string levelToString(Level level)
{
    return templateFor(level).name;
}

MembershipLevel applyLevelTemplate(Level level, point points)
{
    const LevelTemplate& rules = templateFor(level);
    return MembershipLevel(
        rules.startingPoint,
        rules.endingPoint,
        points,
        rules.multiplier,
        rules.discountPercent,
        rules.deliveryDiscountRate,
        rules.lotteryTickets,
        levelFromPoints(points)
    );
}

MembershipSummary buildSummary(const MembershipLevel& membership)
{
    const LevelTemplate& rules = templateFor(membership.getMyLevel());
    MembershipSummary summary;
    summary.level = membership.getMyLevel();
    summary.levelName = rules.name;
    summary.currentPoints = membership.getMyPoint();
    summary.pointsToNextLevel = summary.level == Level::VIP
        ? 0.0
        : rules.endingPoint - membership.getMyPoint();
    if (summary.pointsToNextLevel < 0) {
        summary.pointsToNextLevel = 0.0;
    }
    summary.discountPercent = membership.getoffPercentage();
    summary.deliveryDiscountRate = membership.getDeliveryDiscount();
    summary.deliveryBenefit = rules.deliveryBenefit;
    summary.lotteryTickets = membership.getLotteryTicket();
    return summary;
}

CheckoutSummary buildCheckoutSummary(
    const MembershipLevel& membership,
    const Order& order
)
{
    CheckoutSummary summary{};
    summary.baseTotal = order.getTotalPrice();
    summary.discountPercent = membership.getoffPercentage();
    summary.discountAmount = summary.baseTotal * summary.discountPercent / 100.0;
    summary.baseDeliveryFee = 10.0;
    summary.deliveryDiscountAmount = summary.baseDeliveryFee * membership.getDeliveryDiscount();
    summary.finalDeliveryFee = summary.baseDeliveryFee - summary.deliveryDiscountAmount;
    if (summary.finalDeliveryFee < 0) {
        summary.finalDeliveryFee = 0.0;
    }
    summary.finalTotal = summary.baseTotal - summary.discountAmount + summary.finalDeliveryFee;
    summary.earnedPoints = cost2point(summary.baseTotal) * membership.getMultiplier();
    return summary;
}

bool isUpgrade(Level before, Level after)
{
    return static_cast<int>(after) > static_cast<int>(before);
}

bool ensureRecordForCustomer(const CustID_tp& customerID, MembershipLevel& membership)
{
    MembershipLevelStorage storage;
    if (storage.hasMembershipLevel(customerID)) {
        membership = storage.getMembershipLevel(customerID);
        return true;
    }

    MembershipLevel initial = applyLevelTemplate(Level::Normal, 0.0);
    if (storage.addMembershipLevel(customerID, initial)) {
        membership = initial;
        return true;
    }

    membership = initial;
    return false;
}

bool persistMembershipForCustomer(const CustID_tp& customerID, const MembershipLevel& membership)
{
    MembershipLevelStorage storage;
    if (!storage.hasMembershipLevel(customerID)) {
        return storage.addMembershipLevel(customerID, membership);
    }
    return storage.updateMembershipLevel(customerID, membership);
}

MembershipLevel loadMembershipForCustomer(const CustID_tp& customerID)
{
    MembershipLevelStorage storage;
    return storage.getMembershipLevel(customerID);
}

string upgradeMessage(Level before, Level after)
{
    return "Congratulations! Your membership level increased from "
        + levelToString(before)
        + " to "
        + levelToString(after)
        + ".";
}

MembershipLevel applyOrderToMembership(const MembershipLevel& membership, cost orderBaseTotal)
{
    const point projectedPoints = membership.getMyPoint()
        + cost2point(orderBaseTotal) * membership.getMultiplier();
    return applyLevelTemplate(levelFromPoints(projectedPoints), projectedPoints);
}

MembershipUpdateResult applyOrder(const MembershipLevel& membership, cost orderBaseTotal)
{
    const Level previousLevel = membership.getMyLevel();
    const MembershipLevel next = applyOrderToMembership(membership, orderBaseTotal);

    return {
        next,
        isUpgrade(previousLevel, next.getMyLevel()),
        previousLevel,
        next.getMyLevel()
    };
}
