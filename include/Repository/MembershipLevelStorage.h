# pragma once

# include "Common/Types.h"
# include "Domain/MembershipLevel.h"

struct MembershipLevelLogEntry {
    CustID_tp customerID;
    long long changedAt{};
    Level level{Level::Normal};
    point points{};
};

class MembershipLevelStorage{
public:
    MembershipLevel getMembershipLevel(CustID_tp custID);
    bool hasMembershipLevel(CustID_tp custID);

    bool addMembershipLevel(CustID_tp custID, const MembershipLevel& ml);
    bool delMembershipLevel(CustID_tp custID);
    bool updateMembershipLevel(CustID_tp custID, const MembershipLevel& ml);

    map<Level, size_t> countCustomersByLevel();
    vector<MembershipLevelLogEntry> levelHistory(CustID_tp custID);
};

// The membership objects are stored by their customer's ID
