#pragma once

#include "MembershipLevel.h"

class Gold : public MembershipLevel {
public:
    Gold(
        point mp,
        double m,
        double op,
        double dd,
        size_t lt
    );
};
