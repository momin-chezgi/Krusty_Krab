#include "Domain/Silver.h"

Silver::Silver(point sp, point ep, point mp, double m, double op, double dd, size_t lt) :
    MembershipState(Level::Silver, sp, ep, mp, m, op, dd, lt)
{}

MembershipState* Silver::clone() const
{
    return new Silver(*this);
}

std::string Silver::getLevelName() const
{
    return "Silver";
}

std::string Silver::getDeliveryBenefit() const
{
    return "Reduced delivery fee for high-value(>50$) orders";
}
