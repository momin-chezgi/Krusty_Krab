#pragma once

#include "../neededLibs.h"
#include "../Management/Customer.h"

class InOut{
    virtual int reader() = 0;
    virtual int writer() = 0;
};

class SQLReader{

public:
    Menu getMenu();
};
class SQLWriter{
};