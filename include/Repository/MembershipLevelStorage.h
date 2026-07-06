# pragma once

# include "Common/Types.h"
# include "Domain/MembershipLevel.h"

class MembershipLevelStorage{
public:
    MembershipLevel getMembershipLevel(CustID_tp custID);
    bool hasMembershipLevel(CustID_tp custID);

    bool addMembershipLevel(CustID_tp custID, const MembershipLevel& ml);
    bool delMembershipLevel(CustID_tp custID);
    bool updateMembershipLevel(CustID_tp custID, const MembershipLevel& ml);
};

// The membership objects are stored by their customer's ID
