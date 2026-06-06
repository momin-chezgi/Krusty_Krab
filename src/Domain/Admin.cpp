# include "Domain/Admin.h"
# include "Repository/MenuStorage.h"
# include "Repository/OrderStorage.h"
# include "Repository/RestaurantStorage.h"
# include "Repository/RestaurateurStorage.h"

AdminOfSystem::AdminOfSystem(const vector<ManagerID_tp> &initRestaurateurIDs,
   string initName) :
   id(IDGenerator::uuid()), name(initName)
{
   for(auto r : initRestaurateurIDs){
      restaurateurIDs.push_back(r); 
   }
}

AdminOfSystem::~AdminOfSystem(){}


AdminOfSystem& AdminOfSystem::operator=(const AdminOfSystem& newAdmin)
{
   if(this != &newAdmin){
      id = newAdmin.id;
      name = newAdmin.name;
      restaurateurIDs = newAdmin.restaurateurIDs;
      totalSaleStatistics = newAdmin.totalSaleStatistics;
      totalCustomerStatistics = newAdmin.totalCustomerStatistics;
   }
   return *this;
}




// Getters and Setters:---------------------------------

// itself:

AdminID_tp AdminOfSystem::getID() const
{
   return id;
}

string AdminOfSystem::getName() const
{
   return name;
}

void AdminOfSystem::setName(string newName)
{
   if(newName != "" && newName != " "){
      name = newName;
   }
}


// restaurateurs:
vector<ManagerID_tp> AdminOfSystem::getRestaurateurIDs() const
{
   return restaurateurIDs;
}


bool AdminOfSystem::addRestaurateur(ManagerID_tp restaurateurID)
{
   if(hasRestaurateur(restaurateurID)) return false;
   restaurateurIDs.push_back(restaurateurID);
   return true;
}

bool AdminOfSystem::removeRestaurateur(ManagerID_tp restaurateurID)
{
   auto it = find(restaurateurIDs.begin(), restaurateurIDs.end(), restaurateurID);
   if(it == restaurateurIDs.end()){
      return false;
   }
   restaurateurIDs.erase(it);
   return true;
}

bool AdminOfSystem::replaceRestaurateur(ManagerID_tp previousRestaurteurID, ManagerID_tp newRestaurateurID)
{   
   auto it = find(restaurateurIDs.begin(), restaurateurIDs.end(), previousRestaurteurID);
   if(it == restaurateurIDs.end() || find(restaurateurIDs.begin(), restaurateurIDs.end(), newRestaurateurID) != restaurateurIDs.end()){
      return false;
   }
   *it = newRestaurateurID;
   return true;
}


// restaurant:
RestID_tp AdminOfSystem::addRestaurant(ManagerID_tp restaurateurID,
   const Restaurant& copyingRestaurant,
   string initName)
{
   if(!hasRestaurateur(restaurateurID)) return "Admin doesn't have the restaurateur with the given ID.";
   RestaurantStorage storage;
   RestID_tp newRestaurantID = storage.saveRestaurant(copyingRestaurant);
   if(newRestaurantID == ""){
      return "";
   }
   if(initName != "" && initName != " "){
      storage.setName(newRestaurantID, initName);
   }
   return newRestaurantID;
}

bool AdminOfSystem::removeRestaurant(RestID_tp restaurantID)
{
   RestaurantStorage storage;
   return storage.deleteRestaurant(restaurantID);
}


// restaurateur options:
      
   // itself:

   string AdminOfSystem::getRestaurteurName(ManagerID_tp restaurateurID) const
   {
      RestaurateurStorage storage;
      return storage.getName(restaurateurID);
   }


   bool AdminOfSystem::setRestaurateurName(ManagerID_tp restaurateurID, string newName)
   {
      RestaurateurStorage storage;
      if(storage.isValidRestaurateur(restaurateurID)){
         storage.setName(restaurateurID, newName); 
         return true;
      }
      return false;
   }

   // restaurant:

   RestID_tp AdminOfSystem::getRestaurantID(ManagerID_tp restaurateurID) const
   {
      RestaurateurStorage storage;
      Restaurateur restaurateur = storage.giveRestaurateur(restaurateurID);
      return restaurateur.getRestaurantID();
   }

   string AdminOfSystem::getRestaurantName(RestID_tp restaurantID) const
   {
      RestaurantStorage storage;
      return storage.getName(restaurantID);
   }

   string AdminOfSystem::getRestaurantAddress(RestID_tp restaurantID) const
   {
      RestaurantStorage storage;
      return storage.getAddress(restaurantID);
   }

   bool AdminOfSystem::getRestaurantStatus(RestID_tp restaurantID) const
   {
      RestaurantStorage storage;
      return storage.getStatus(restaurantID);
   }

   size_t AdminOfSystem::getRestaurantPreparationMinutes(RestID_tp restaurantID) const
   {
      RestaurantStorage storage;
      return storage.getPreparationMinutes(restaurantID);
   }

   string AdminOfSystem::getRestaurantPhone(RestID_tp restaurantID) const
   {
      RestaurantStorage storage;
      return storage.getPhoneNumber(restaurantID);
   }

   string AdminOfSystem::getRestaurantBio(RestID_tp restaurantID) const
   {
      RestaurantStorage storage;
      return storage.getBio(restaurantID);
   }


   bool AdminOfSystem::editRestaurantName(ManagerID_tp restaurateurID, string newName)
   {
   if(!hasRestaurateur(restaurateurID)) return false;
   RestaurantStorage storage;
   RestID_tp restaurantID = getRestaurantID(restaurateurID);
   return storage.setName(restaurantID, newName);
   }

   bool AdminOfSystem::editRestaurantAddress(ManagerID_tp restaurateurID, string newAddress)
   {
   if(!hasRestaurateur(restaurateurID)) return false;
   RestaurantStorage storage;
   RestID_tp restaurantID = getRestaurantID(restaurateurID);
   return storage.setAddress(restaurantID, newAddress);
   }

   bool AdminOfSystem::activateRestaurant(ManagerID_tp restaurateurID)
   {   
      if(!hasRestaurateur(restaurateurID)) return false;
      RestaurantStorage storage;
      RestID_tp restaurantID = getRestaurantID(restaurateurID);
      return storage.activateRestaurant(restaurantID);
   }

   bool AdminOfSystem::deactivateRestaurant(ManagerID_tp restaurateurID)
   {   
      if(!hasRestaurateur(restaurateurID)) return false;
      RestaurantStorage storage;
      RestID_tp restaurantID = getRestaurantID(restaurateurID);
      return storage.deactivateRestaurant(restaurantID);
   }

   bool AdminOfSystem::setPreparationTime(ManagerID_tp restaurateurID, const size_t minutes)
   {   
      if(!hasRestaurateur(restaurateurID)) return false;
      RestaurantStorage storage;
      RestID_tp restaurantID = getRestaurantID(restaurateurID);
      return storage.setPreparationTime(restaurantID, minutes);
   }

   bool AdminOfSystem::setPhoneNumber(ManagerID_tp restaurateurID, const string newPhoneNumber)
   {   
      if(!hasRestaurateur(restaurateurID)) return false;
      RestaurantStorage storage;
      RestID_tp restaurantID = getRestaurantID(restaurateurID);
      return storage.setPhoneNumber(restaurantID, newPhoneNumber);
   }

   bool AdminOfSystem::setBio(ManagerID_tp restaurateurID, const string newBio)
   {   
      if(!hasRestaurateur(restaurateurID)) return false;
      RestaurantStorage storage;   
      RestID_tp restaurantID = getRestaurantID(restaurateurID);
      return storage.setBio(restaurantID, newBio);
   }


   // menu:

   MenuID_tp AdminOfSystem::getMenuID(ManagerID_tp restaurateurID) const
   {
      RestaurantStorage storage;
      RestID_tp restaurantID = getRestaurantID(restaurateurID);
      return storage.getMenuID(restaurantID);
   }
   

   bool AdminOfSystem::addItemToMenu(ManagerID_tp restaurateurID, const MenuItem& item)
   {
      if(!hasRestaurateur(restaurateurID)) return false;
      RestaurantStorage storage;
      RestID_tp restaurantID = getRestaurantID(restaurateurID);
      MenuID_tp menuID = storage.getMenuID(restaurantID);
      MenuStorage mStorage;
      return mStorage.addItem(menuID, item);
   }

   bool AdminOfSystem::removeItemFromMenu(ManagerID_tp restaurateurID, ItemID_tp itemID)
   {
      if(!hasRestaurateur(restaurateurID)) return false;
      RestaurantStorage storage;
      RestID_tp restaurantID = getRestaurantID(restaurateurID);
      MenuID_tp menuID = storage.getMenuID(restaurantID);
      MenuStorage mStorage;
      return mStorage.deleteItem(menuID, itemID);
   }

   bool AdminOfSystem::replaceItemInMenu(ManagerID_tp restaurateurID, ItemID_tp previousItemID, const MenuItem& replacedItem)
   {      
      if(!hasRestaurateur(restaurateurID)) return false;
      RestaurantStorage storage;
      RestID_tp restaurantID = getRestaurantID(restaurateurID);
      MenuID_tp menuID = storage.getMenuID(restaurantID);
      MenuStorage mStorage;
      if(replacedItem.getID() == previousItemID) return false;
      if(mStorage.has(menuID, replacedItem.getID())) return false;
      if(!mStorage.has(menuID, previousItemID)) return false; 
      return mStorage.addItem(menuID,replacedItem) && mStorage.deleteItem(menuID, previousItemID);
   }


   // order:

   bool AdminOfSystem::addItemToOrder(ManagerID_tp restaurateurID, OrderID_tp orderID, const MenuItem& item, double quantity)
   {      
      if(!hasRestaurateur(restaurateurID)) return false;
      OrderStorage storage;
      return storage.addItem(orderID, item.getID(), quantity);
   }

   bool AdminOfSystem::removeItemFromOrder(ManagerID_tp restaurateurID, OrderID_tp orderID, ItemID_tp itemID)
   {      
      if(!hasRestaurateur(restaurateurID)) return false;
      OrderStorage storage;
      return storage.removeItem(orderID, itemID);
   }


   // queue:

   vector<OrderID_tp> AdminOfSystem::getOrderIDs(ManagerID_tp restaurateurID) const
   {      
      if(!hasRestaurateur(restaurateurID)) return {"NotFound"};
      RestaurantStorage storage;
      RestID_tp restaurantID = getRestaurantID(restaurateurID);
      return storage.getOrderIDs(restaurantID);
   }


   bool AdminOfSystem::addOrderToQueue(ManagerID_tp restaurateurID, OrderID_tp orderID)
   {      
      if(!hasRestaurateur(restaurateurID)) return false;
      RestaurantStorage storage;
      RestID_tp restaurantID = getRestaurantID(restaurateurID);
      return storage.addOrderToRestaurant(restaurantID, orderID);
   }

   bool AdminOfSystem::removeOrderFromQueue(ManagerID_tp restaurateurID, OrderID_tp orderID)
   {      
      if(!hasRestaurateur(restaurateurID)) return false;
      RestaurantStorage storage;
      RestID_tp restaurantID = getRestaurantID(restaurateurID);
      return storage.deleteOrderFromRestaurant(restaurantID, orderID);
   }
   
   bool AdminOfSystem::replaceOrderInQueue(ManagerID_tp restaurateurID, OrderID_tp previousOrderID, OrderID_tp newOrderID)
   {      
      if(!hasRestaurateur(restaurateurID)) return false;
      RestaurantStorage storage;
      RestID_tp restaurantID = getRestaurantID(restaurateurID);
      return storage.addOrderToRestaurant(restaurantID, newOrderID) && storage.deleteOrderFromRestaurant(restaurantID, previousOrderID);
   }





void AdminOfSystem::updateAndPrintTotalSaleStatistics(){}
void AdminOfSystem::updateAndPrintTotalCustomerStatistics(){}
