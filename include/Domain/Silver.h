#pragma once

#include "MembershipLevel.h"

class Silver : public MembershipLevel {
public:
    Silver(
        point mp,
        double m,
        double op,
        double dd,
        size_t lt
    );
};
