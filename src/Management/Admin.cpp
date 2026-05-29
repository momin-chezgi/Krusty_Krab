# include "Admin.h"

AdminOfSystem::AdminOfSystem(string initName=""):
   id(IDGenerator::uuid()), name(initName){}


AdminOfSystem::AdminOfSystem(const vector<Restaurateur *> &initRestaurateurs):
   id(IDGenerator::uuid())
{
   for(auto r : initRestaurateurs){
      restaurateurs[r->getID()] = r;
   }
}

Restaurateur *AdminOfSystem::addRestaurant()
{
   return new Restaurateur(IDGenerator::uuid());
}
Restaurateur *AdminOfSystem::addRestaurant(const Restaurant& copyingRestaurant, string initName = "")
{
   return new Restaurateur(copyingRestaurant, initName);
}
Restaurateur *AdminOfSystem::addRestaurant(string initName, vector<string> initAddress, string initPhoneNumber) 
{
   auto r = new Restaurant(initName, initAddress, initPhoneNumber);
   return new Restaurateur(*r, initName);
}

string AdminOfSystem::getID() const
{
   return id;
}

string AdminOfSystem::getName() const
{
   return name;
}

Restaurateur *AdminOfSystem::accessRestaurant(string restaurantID)
{
   return findForRestaurant(restaurantID);
}

void AdminOfSystem::updateAndPrintTotalSaleStatistics(){}
void AdminOfSystem::updateAndPrintTotalCustomerStatistics(){}

AdminOfSystem::~AdminOfSystem(){}
