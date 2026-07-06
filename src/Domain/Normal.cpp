# include "Domain/Normal.h"


Normal::Normal(point mp, double m, double op, double dd, size_t lt) :
    MembershipLevel(0.0, 100.0, mp, m, op, dd, lt, level::Normal)
{}
