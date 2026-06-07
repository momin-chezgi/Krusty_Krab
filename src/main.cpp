#include "Common/Types.h"
#include "Domain/Admin.h"
#include "Domain/Order.h"
#include "Domain/MenuItem.h"
#include "Repository/AdminStorage.h"
#include "Repository/CustomerStorage.h"
#include "Repository/MenuStorage.h"
#include "Repository/OrderStorage.h"
#include "Repository/RestaurantStorage.h"
#include "Repository/RestaurateurStorage.h"
#include "UI/Interface.h"
#include "Utility/IDGenerator.h"

void CustomerDashboard();
void RestaurateurDashboard();
void AdminDashboard();

int main()
{
    while (true)
    {
        switch(Login())
        {
            case Role::Customer:
                CustomerDashboard();
                break;
            case Role::Restaurateur:
                RestaurateurDashboard();
                break;
            case Role::SystemAdmin:
                AdminDashboard();
                break;
            case Role::Quit:
                return 0;
            default:
                Printer::InvalidInput();
        }
    }
}

void CustomerDashboard(){
    Customer user;
    if (!enterAsCustomer(user)){  
        return;
    }
    CustomerStorage cStorage;

    while(true){
        CustomerAction action = customerActions(user.getName());
        if(action == CustomerAction::Quit){
            return;
        }
        if(action == CustomerAction::DebugStorage){
            Printer::debugStorage();
            continue;
        }
        if(action == CustomerAction::PlaceOrder){
            RestID_tp restaurantID = chooseRestaurant();
            MenuID_tp menuID = giveMenu(restaurantID);
            Printer::menu(menuID);
            Order order(user.getID(), {});
            if(orderOut(restaurantID, menuID, order)){
                user.orderOut(order.getID());
                RestaurantStorage rstorage;
                OrderStorage oStorage;
                oStorage.saveOrder(order);
                rstorage.addOrderToRestaurant(restaurantID, order.getID());
                cStorage.updateCustomer(user);
                cout << "Order created with ID: " << order.getID() << endl;
            }
            continue;
        }

        if(action == CustomerAction::ViewMyOrders){
            OrderStorage oStorage;
            if (user.getMyOrders().empty()) {
                cout << "No orders found for this customer." << endl;
                continue;
            }
            cout << "My orders:" << endl;
            for(const auto& orderID : user.getMyOrders()){
                if(!oStorage.isValidOrder(orderID)){
                    continue;
                }
                cout << "- " << orderID << endl;
            }
        }
    }
}

void RestaurateurDashboard(){
    Restaurateur user = enterAsRestaurateur();
    if(user.getName().empty() && user.getRestaurantID().empty()){
        return;
    }
    RestaurateurStorage rStorage;

    while(true){
        RestaurateurAction chosenOption = restaurateurOptions(
            user.getRestaurantName(),
            user.getRestaurantAddress(),
            user.getRestaurantStatus(),
            user.getRestaurantPreparationMinutes()
        );

        if(chosenOption == RestaurateurAction::Quit){
            return;
        }
        switch(chosenOption){
            case RestaurateurAction::EditRestaurantName:
                user.editRestaurantName(GetInf::modifyRestaurantString(chosenOption));
                break;
            case RestaurateurAction::EditRestaurantAddress:
                user.editRestaurantAddress(GetInf::modifyRestaurantString(chosenOption));
                break;
            case RestaurateurAction::ActivateRestaurant:
                user.activateRestaurant();
                break;
            case RestaurateurAction::DeactivateRestaurant:
                user.deactivateRestaurant();
                break;
            case RestaurateurAction::EditRestaurantMinutes:
                user.setPreparationTime(GetInf::modifyRestaurantTime(chosenOption));
                break;
            case RestaurateurAction::EditRestaurantPhone:
                user.setPhoneNumber(GetInf::modifyRestaurantString(chosenOption));
                break;
            case RestaurateurAction::EditRestaurantBio:
                user.setBio(GetInf::modifyRestaurantString(chosenOption));
                break;
            case RestaurateurAction::AddItemToMenu:
            {
                MenuItem* item = GetInf::menuItem();
                if (item) {
                    user.addItemToMenu(item);
                    delete item;
                }
                break;
            }
            case RestaurateurAction::RemoveItemFromMenu:
                user.removeItemFromMenu(GetInf::menuItemID());
                break;
            case RestaurateurAction::ReplaceItemInMenu:
            {
                ItemID_tp oldItemID = GetInf::menuItemID();
                MenuItem* item = GetInf::menuItem();
                if (item) {
                    user.replaceItemInMenu(oldItemID, item);
                    delete item;
                }
                break;
            }
            case RestaurateurAction::AddOrderToQueue:
            {
                RestID_tp restaurantID = user.getRestaurantID();
                MenuID_tp menuID = user.getMenuID();
                Order newOrder(user.getID(), {});
                if (orderOut(restaurantID, menuID, newOrder)) {
                    OrderStorage oStorage;
                    if (oStorage.saveOrder(newOrder)) {
                        user.addOrderToQueue(newOrder.getID());
                    }
                }
                break;
            }
            case RestaurateurAction::RemoveOrderFromQueue:
                user.removeOrderFromQueue(GetInf::OrderID(RestaurateurAction::RemoveOrderFromQueue));
                break;
            case RestaurateurAction::ReplaceOrderInQueue:
            {
                OrderID_tp previousOrderID = GetInf::OrderID(RestaurateurAction::ReplaceOrderInQueue);
                OrderID_tp newOrderID = GetInf::OrderID(RestaurateurAction::ReplaceOrderInQueue);
                user.replaceOrderInQueue(previousOrderID, newOrderID);
                break;
            }
            case RestaurateurAction::PrintSaleStatistics:
                user.updateAndPrintSaleStatistics();
                break;
            case RestaurateurAction::PrintCustomerStatistics:
                user.updateAndPrintCustomerStatistics();
                break;
            case RestaurateurAction::EditRestaurateurName:
                user.setName(GetInf::modifyRestaurantString(chosenOption));
                rStorage.updateRestaurateur(user);
                break;
            case RestaurateurAction::EditManagedRestaurant:
            {
                RestID_tp newRestaurantID = GetInf::modifyRestaurantString(chosenOption);
                user.setRestaurantID(newRestaurantID);
                rStorage.updateRestaurateur(user);
                break;
            }
            case RestaurateurAction::DebugStorage:
                Printer::debugStorage();
                break;
            default:
                break;
        }
    }
}

void AdminDashboard()
{
    AdminOfSystem user = enterAsAdmin();
    if(user.getName() == "Quit"){
        return;
    }
    AdminStorage adminStorage;
    RestaurateurStorage restaurateurStorage;
    RestaurantStorage restaurantStorage;

    while(true){
        AdminAction chosenOption = adminOptions(user.getRestaurateurIDs());
        if(chosenOption == AdminAction::Quit){
            return;
        }
        if(chosenOption == AdminAction::DebugStorage){
            Printer::debugStorage();
            continue;
        }
        if(chosenOption == AdminAction::CreateRestaurant){
            cout << "Enter restaurateur ID to link this restaurant:(Caution: if your restaurateur ID isn't valid, the restaurant won't save) ";
            ManagerID_tp restaurateurID;
            cin >> restaurateurID;
            Restaurant newRestaurant = GetInf::newRestaurant();
            if(!restaurateurStorage.isValidRestaurateur(restaurateurID)){
                cout << "Invalid restaurateur ID." << endl;
                continue;
            }
            RestID_tp newRestaurantID = restaurantStorage.saveRestaurant(newRestaurant);
            if(newRestaurantID.empty()){
                cout << "Could not save new restaurant." << endl;
                continue;
            }
            Restaurateur manager = restaurateurStorage.giveRestaurateur(restaurateurID);
            if(!manager.setRestaurantID(newRestaurantID)){
                cout << "Restaurant created, but restaurateur couldn't be linked." << endl;
            }
            restaurateurStorage.updateRestaurateur(manager);
            cout << "Restaurant created with ID: " << newRestaurantID << endl;
            user.addRestaurateur(restaurateurID);
            adminStorage.updateAdmin(user);
            continue;
        }
        if(chosenOption == AdminAction::CreateRestaurateur){
            Restaurateur newRestaurateur = GetInf::restaurateurFactory();
            if(restaurateurStorage.saveRestaurateur(newRestaurateur)){
                user.addRestaurateur(newRestaurateur.getID());
                adminStorage.updateAdmin(user);
                cout << "Restaurateur created with ID: " << newRestaurateur.getID() << endl;
            } else {
                cout << "Could not create restaurateur." << endl;
            }
            continue;
        }
        if(chosenOption == AdminAction::ActivateRestaurant
            || chosenOption == AdminAction::DeactivateRestaurant){
            string restaurateurID = GetInf::modifyRestaurantString(RestaurateurAction::EditManagedRestaurant);
            if(chosenOption == AdminAction::ActivateRestaurant){
                user.activateRestaurant(restaurateurID);
            } else {
                user.deactivateRestaurant(restaurateurID);
            }
            adminStorage.updateAdmin(user);
            continue;
        }
        if(chosenOption == AdminAction::PrintTotalSaleStatistics
            || chosenOption == AdminAction::PrintTotalCustomerStatistics
            || chosenOption == AdminAction::PrintRestaurantSaleStatistics
            || chosenOption == AdminAction::PrintRestaurantCustomerStatistics){
            if(chosenOption == AdminAction::PrintTotalSaleStatistics
                || chosenOption == AdminAction::PrintRestaurantSaleStatistics){
                user.updateAndPrintTotalSaleStatistics();
            } else {
                user.updateAndPrintTotalCustomerStatistics();
            }
            continue;
        }
    }
}
