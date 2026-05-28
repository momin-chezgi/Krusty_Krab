# include "Admin.h"

AdminOfSystem::AdminOfSystem(size_t initID, string initName=""):
   id(initID), name(initName){}

AdminOfSystem::AdminOfSystem(size_t initID, const vector<Restaurateur *> &initRestaurateurs):
   id(initID)
{
   for(auto r : initRestaurateurs){
      restaurateurs[r->getID()] = r;
   }
}

Restaurateur *AdminOfSystem::addRestaurant()
{
   return new Restaurateur(IDInitializer::Restaurateur());
}
Restaurateur *AdminOfSystem::addRestaurant(Restaurant *r, string initName = "")
{
   return new Restaurateur(IDInitializer::Restaurateur(), r);
}
Restaurateur *AdminOfSystem::addRestaurant(size_t initID, string initName, vector<string> initAddress, string initPhoneNumber) 
{
   auto r = new Restaurant(initID, initName, initAddress, initPhoneNumber);
   return new Restaurateur(IDInitializer::Restaurateur(), r);
}

Restaurateur *AdminOfSystem::accessRestaurant(size_t restaurantID)
{
   return findForRestaurant(restaurantID);
}

void AdminOfSystem::updateAndPrintTotalSaleStatistics(){}
void AdminOfSystem::updateAndPrintTotalCustomerStatistics(){}

AdminOfSystem::~AdminOfSystem(){}
