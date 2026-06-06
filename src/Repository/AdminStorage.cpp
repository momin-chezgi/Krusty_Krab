
#include "Repository/AdminStorage.h"


map<AdminID_tp, AdminOfSystem> AdminStorage::admins = {
    {"TestAdmin", AdminOfSystem({"TestRestaurateur"}, "TestAdmin")}
};

bool AdminStorage::isValidAdmin(AdminID_tp adminID)
{
    auto it = admins.find(adminID);
    return it != admins.end();
}
