#include "Domain/Silver.h"

Silver::Silver(point mp, double m, double op, double dd, size_t lt) :
    MembershipLevel(100, 300, mp, m, op, dd, lt, Level::Silver)
{}
