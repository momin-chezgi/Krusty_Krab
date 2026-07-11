#pragma once

#include "MembershipState.h"

class Silver : public MembershipState {
public:
    Silver(
        point sp,
        point ep,
        point mp,
        double m,
        double op,
        double dd,
        size_t lt
    );

    MembershipState* clone() const override;
    std::string getLevelName() const override;
    std::string getDeliveryBenefit() const override;
};
