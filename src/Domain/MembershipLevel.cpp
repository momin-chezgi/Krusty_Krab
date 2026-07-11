# include "Domain/MembershipLevel.h"
# include "Domain/Normal.h"
# include "Domain/Silver.h"
# include "Domain/Gold.h"
# include "Domain/VIP.h"
# include "Domain/Order.h"
# include "Repository/MembershipLevelStorage.h"

MembershipState::MembershipState(
    Level l,
    point sp,
    point ep,
    point mp,
    double m,
    double op,
    double dd,
    size_t lt
) :
    level(l),
    startingPoint(sp),
    endingPoint(ep),
    myPoint(mp),
    multiplier(m),
    deliveryDiscount(dd),
    lotteryTicket(lt)
{
    if (op >= 0 && op <= 100){
        offPercentage = op;
    }
}

Level MembershipState::getLevel() const
{
    return level;
}

Level MembershipState::getMyLevel() const
{
    return getLevel();
}

void MembershipState::setLevel(Level l)
{
    level = l;
}

point MembershipState::getStartingPoint() const
{
    return startingPoint;
}

void MembershipState::setStartingPoint(point sp)
{
    startingPoint = sp;
}

point MembershipState::getEndingPoint() const
{
    return endingPoint;
}

void MembershipState::setEndingPoint(point ep)
{
    endingPoint = ep;
}

point MembershipState::getMyPoint() const
{
    return myPoint;
}

void MembershipState::setMyPoint(point mp)
{
    myPoint = mp;
}

double MembershipState::getMultiplier() const
{
    return multiplier;
}

void MembershipState::setMultiplier(double m)
{
    multiplier = m;
}

double MembershipState::getOffPercentage() const
{
    return offPercentage;
}

double MembershipState::getoffPercentage() const
{
    return getOffPercentage();
}

void MembershipState::setOffPercentage(double op)
{
    if (op >= 0 && op <= 100){
        offPercentage = op;
    }
}

double MembershipState::getDeliveryDiscount() const
{
    return deliveryDiscount;
}

void MembershipState::setDeliveryDiscount(double dd)
{
    deliveryDiscount = dd;
}

size_t MembershipState::getLotteryTicket() const
{
    return lotteryTicket;
}

void MembershipState::setLotteryTicket(size_t lt)
{
    lotteryTicket = lt;
}

namespace {
    MembershipState* createStateWithValues(
        Level level,
        point startingPoint,
        point endingPoint,
        point points,
        double multiplier,
        double offPercentage,
        double deliveryDiscount,
        size_t lotteryTicket
    )
    {
        if (level == Level::VIP) {
            return new VIP(
                startingPoint,
                endingPoint,
                points,
                multiplier,
                offPercentage,
                deliveryDiscount,
                lotteryTicket
            );
        } else if (level == Level::Gold) {
            return new Gold(
                startingPoint,
                endingPoint,
                points,
                multiplier,
                offPercentage,
                deliveryDiscount,
                lotteryTicket
            );
        } else if (level == Level::Silver) {
            return new Silver(
                startingPoint,
                endingPoint,
                points,
                multiplier,
                offPercentage,
                deliveryDiscount,
                lotteryTicket
            );
        }
        return new Normal(
            startingPoint,
            endingPoint,
            points,
            multiplier,
            offPercentage,
            deliveryDiscount,
            lotteryTicket
        );
    }
}

MembershipLevel::MembershipLevel(point sp, point ep, point mp, double m, double op, double dd, size_t lt, Level l) :
    currentState(createStateWithValues(l, sp, ep, mp, m, op, dd, lt))
{
}

MembershipLevel::MembershipLevel(MembershipState* state) :
    currentState(state)
{
    if (currentState == nullptr) {
        currentState = createStateByLevel(Level::Normal, 0.0);
    }
}

MembershipLevel::MembershipLevel(const MembershipLevel& other) :
    currentState(other.currentState != nullptr ? other.currentState->clone() : nullptr),
    levelLog(other.levelLog)
{}

MembershipLevel& MembershipLevel::operator=(const MembershipLevel& other)
{
    if (this != &other) {
        setState(other.currentState != nullptr ? other.currentState->clone() : nullptr);
        levelLog = other.levelLog;
    }
    return *this;
}

MembershipLevel::~MembershipLevel()
{
    delete currentState;
}

point MembershipLevel::getStartingPoint() const
{
    return currentState != nullptr ? currentState->getStartingPoint() : 0.0;
}
void MembershipLevel::setStartingPoint(point sp)
{
    if (currentState != nullptr) {
        currentState->setStartingPoint(sp);
    }
}
point MembershipLevel::getEndingPoint() const
{
    return currentState != nullptr ? currentState->getEndingPoint() : 0.0;
}
void MembershipLevel::setEndingPoint(point ep)
{
    if (currentState != nullptr) {
        currentState->setEndingPoint(ep);
    }
}
point MembershipLevel::getMyPoint() const
{
    return currentState != nullptr ? currentState->getMyPoint() : 0.0;
}
void MembershipLevel::setMyPoint(point mp)
{
    if (currentState != nullptr) {
        currentState->setMyPoint(mp);
    }
}
Level MembershipLevel::getMyLevel() const
{
    return currentState != nullptr ? currentState->getLevel() : Level::Normal;
}
void MembershipLevel::setMyLevel(Level level)
{
    if (currentState == nullptr) {
        setState(createStateByLevel(level, 0.0));
        return;
    }
    setState(
        createStateWithValues(
            level,
            currentState->getStartingPoint(),
            currentState->getEndingPoint(),
            currentState->getMyPoint(),
            currentState->getMultiplier(),
            currentState->getOffPercentage(),
            currentState->getDeliveryDiscount(),
            currentState->getLotteryTicket()
        )
    );
}
double MembershipLevel::getMultiplier() const
{
    return currentState != nullptr ? currentState->getMultiplier() : 1.0;
}
void MembershipLevel::setMultiplier(double m)
{
    if (currentState != nullptr) {
        currentState->setMultiplier(m);
    }
}
double MembershipLevel::getoffPercentage() const
{
    return currentState != nullptr ? currentState->getOffPercentage() : 0.0;
}
void MembershipLevel::setOffPercentage(double op)
{
    if (currentState != nullptr) {
        currentState->setOffPercentage(op);
    }
}
double MembershipLevel::getDeliveryDiscount() const
{
    return currentState != nullptr ? currentState->getDeliveryDiscount() : 0.0;
}
void MembershipLevel::setDeliveryDiscount(double dd)
{
    if (currentState != nullptr) {
        currentState->setDeliveryDiscount(dd);
    }
}
size_t MembershipLevel::getLotteryTicket() const
{
    return currentState != nullptr ? currentState->getLotteryTicket() : 0;
}
void MembershipLevel::setLotteryTicket(size_t lt)
{
    if (currentState != nullptr) {
        currentState->setLotteryTicket(lt);
    }
}
std::string MembershipLevel::getLevelName() const
{
    return currentState != nullptr ? currentState->getLevelName() : "Normal";
}
std::string MembershipLevel::getDeliveryBenefit() const
{
    return currentState != nullptr ? currentState->getDeliveryBenefit() : "No delivery discount";
}
void MembershipLevel::setState(MembershipState* state)
{
    if (currentState == state) {
        return;
    }
    delete currentState;
    currentState = state;
}
MembershipState* MembershipLevel::getState() const
{
    return currentState;
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
    const point projectedPoints = getMyPoint() + cost2point(orderedPrice) * getMultiplier();
    setState(createStateByPoints(projectedPoints));
    return getMyLevel();
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
    const point projectedPoints = getMyPoint() - cost2point(cancelledPrice) * getMultiplier();
    setState(createStateByPoints(projectedPoints));
    return getMyLevel();
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

string levelToString(Level level)
{
    if (level == Level::VIP) {
        return "VIP";
    } else if (level == Level::Gold) {
        return "Gold";
    } else if (level == Level::Silver) {
        return "Silver";
    }
    return "Normal";
}

MembershipState* createStateByLevel(Level level, point points)
{
    if (level == Level::VIP) {
        return new VIP(700.0, 1000.0, points, 2.0, 15.0, 1.0, 3);
    } else if (level == Level::Gold) {
        return new Gold(300.0, 700.0, points, 1.5, 10.0, 1.0, 2);
    } else if (level == Level::Silver) {
        return new Silver(100.0, 300.0, points, 1.2, 5.0, 0.5, 1);
    }
    return new Normal(0.0, 100.0, points, 1.0, 0.0, 0.0, 0);
}

MembershipState* createStateByPoints(point points)
{
    if (points >= 700.0) {
        return createStateByLevel(Level::VIP, points);
    } else if (points >= 300.0) {
        return createStateByLevel(Level::Gold, points);
    } else if (points >= 100.0) {
        return createStateByLevel(Level::Silver, points);
    }
    return createStateByLevel(Level::Normal, points);
}

MembershipLevel::Report buildSummary(const MembershipLevel& membership)
{
    MembershipLevel::Report summary;
    summary.level = membership.getMyLevel();
    summary.levelName = membership.getLevelName();
    summary.currentPoints = membership.getMyPoint();
    summary.pointsToNextLevel = summary.level == Level::VIP
        ? 0.0
        : membership.getEndingPoint() - membership.getMyPoint();
    if (summary.pointsToNextLevel < 0) {
        summary.pointsToNextLevel = 0.0;
    }
    summary.discountPercent = membership.getoffPercentage();
    summary.deliveryDiscountRate = membership.getDeliveryDiscount();
    summary.deliveryBenefit = membership.getDeliveryBenefit();
    summary.lotteryTickets = membership.getLotteryTicket();
    return summary;
}

MembershipLevel::Report buildCheckoutSummary(
    const MembershipLevel& membership,
    const Order& order
)
{
    MembershipLevel::Report summary{};
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

    MembershipLevel initial(createStateByLevel(Level::Normal, 0.0));
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
    return MembershipLevel(createStateByPoints(projectedPoints));
}

MembershipLevel::Report applyOrder(const MembershipLevel& membership, cost orderBaseTotal)
{
    const Level previousLevel = membership.getMyLevel();
    const MembershipLevel next = applyOrderToMembership(membership, orderBaseTotal);
    MembershipLevel::Report summary = buildSummary(next);
    summary.levelUp = isUpgrade(previousLevel, next.getMyLevel());
    summary.previousLevel = previousLevel;
    summary.nextLevel = next.getMyLevel();
    return summary;
}
