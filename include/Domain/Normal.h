#pragma once

# include "MembershipLevel.h"

class Normal : public MembershipLevel{
public:
    Normal(
         point mp,
         double m,
         double op,
         double dd,
         size_t lt
        );
};
