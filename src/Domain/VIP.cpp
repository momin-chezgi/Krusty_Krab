#include "Domain/VIP.h"

VIP::VIP(point sp, point ep, point mp, double m, double op, double dd, size_t lt) :
    MembershipState(Level::VIP, sp, ep, mp, m, op, dd, lt)
{}

MembershipState* VIP::clone() const
{
    return new VIP(*this);
}

std::string VIP::getLevelName() const
{
    return "VIP";
}

std::string VIP::getDeliveryBenefit() const
{
    return "Free delivery";
}
