
#include "Repository/AdminStorage.h"


map<AdminID_tp, AdminOfSystem> AdminStorage::admins = {
    {"TestAdmin", AdminOfSystem({"TestRestaurateur"}, "TestAdmin")}
};

AdminOfSystem AdminStorage::giveAdmin(AdminID_tp adminID) const
{
    auto it = admins.find(adminID);
    if (it != admins.end()) {
        return it->second;
    }
    return AdminOfSystem({}, "");
}

bool AdminStorage::isValidAdmin(AdminID_tp adminID) const
{
    auto it = admins.find(adminID);
    return it != admins.end();
}

bool AdminStorage::saveAdmin(const AdminOfSystem& newAdmin)
{
    return admins.insert({newAdmin.getID(), newAdmin}).second;
}

bool AdminStorage::deleteAdmin(AdminID_tp adminID)
{
    return admins.erase(adminID) > 0;
}

bool AdminStorage::updateAdmin(const AdminOfSystem& updatingAdmin)
{
    auto it = admins.find(updatingAdmin.getID());
    if (it == admins.end()) {
        return false;
    }
    it->second = updatingAdmin;
    return true;
}
