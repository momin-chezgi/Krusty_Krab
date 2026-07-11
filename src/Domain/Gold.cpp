#include "Domain/Gold.h"

Gold::Gold(point sp, point ep, point mp, double m, double op, double dd, size_t lt) :
    MembershipState(Level::Gold, sp, ep, mp, m, op, dd, lt)
{}

MembershipState* Gold::clone() const
{
    return new Gold(*this);
}

std::string Gold::getLevelName() const
{
    return "Gold";
}

std::string Gold::getDeliveryBenefit() const
{
    return "Half-price delivery";
}
