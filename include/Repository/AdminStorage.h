# pragma once 

# include "Common/Types.h"
# include "Domain/Admin.h"

class AdminStorage{
public:
    bool saveAdmin(const AdminOfSystem& newAdmin);  // returns true if the admin has been saved successfully, otherwise false
    bool deleteAdmin(AdminID_tp adminID);  // returns true if the admin with the given ID exists and has been deleted, otherwise returns false
    bool updateAdmin(const AdminOfSystem& updatingAdmin);  // returns true if the admin with the given ID exists and has been updated, otherwise returns false
private:
    bool isValidAdmin(AdminID_tp adminID);
    static map<AdminID_tp, AdminOfSystem> admins;
};
