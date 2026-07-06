#pragma once

#include "MembershipLevel.h"

class VIP : public MembershipLevel {
public:
    VIP(
        point mp,
        double m,
        double op,
        double dd,
        size_t lt
    );
};
