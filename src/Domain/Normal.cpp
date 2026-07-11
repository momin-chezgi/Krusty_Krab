# include "Domain/Normal.h"


Normal::Normal(point sp, point ep, point mp, double m, double op, double dd, size_t lt) :
    MembershipState(Level::Normal, sp, ep, mp, m, op, dd, lt)
{}

MembershipState* Normal::clone() const
{
    return new Normal(*this);
}

std::string Normal::getLevelName() const
{
    return "Normal";
}

std::string Normal::getDeliveryBenefit() const
{
    return "No delivery discount";
}
