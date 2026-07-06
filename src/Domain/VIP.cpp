#include "Domain/VIP.h"

VIP::VIP(point mp, double m, double op, double dd, size_t lt) :
    MembershipLevel(700, 1e6, mp, m, op, dd, lt, Level::VIP)
{}
