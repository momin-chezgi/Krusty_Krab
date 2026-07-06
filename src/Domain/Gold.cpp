#include "Domain/Gold.h"

Gold::Gold(point mp, double m, double op, double dd, size_t lt) :
    MembershipLevel(300, 700, mp, m, op, dd, lt, Level::Gold)
{}
