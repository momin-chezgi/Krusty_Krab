#include "Common/Types.h"
#include "Domain/Admin.h"
#include "Domain/Order.h"
#include "Domain/MenuItem.h"
#include "Repository/AdminStorage.h"
#include "Repository/MenuStorage.h"
#include "Repository/OrderStorage.h"
#include "Repository/RestaurantStorage.h"
#include "Repository/RestaurateurStorage.h"
#include "Repository/CustomerStorage.h"
#include "Repository/MembershipLevelStorage.h"
#include "Database/DatabaseManager.h"
#include "UI/Interface.h"
#include "Utility/IDGenerator.h"

void CustomerDashboard();
void RestaurateurDashboard();
void AdminDashboard();

int main()
{
    DatabaseManager database;
    if (!database.isOpen()) {
        cerr << "Warning: SQLite database is not available." << endl;
    }

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
            case Role::ClearScreen:
                Printer::clearScreen();
                break;
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

    MembershipLevel membership = applyLevelTemplate(Level::Normal, 0.0);
    const bool hasMembershipProfile = ensureRecordForCustomer(user.getID(), membership);
    if (!hasMembershipProfile) {
        cout << "Could not open membership profile. Continuing without loyalty persistence." << endl;
    }
    MembershipSummary membershipSummary = buildSummary(membership);

    while(true){
        CustomerAction action = customerActions(user, membershipSummary);
        switch (action){
            case CustomerAction::ClearScreen:
                Printer::clearScreen();
                break;
            case CustomerAction::Quit:
                return;
            case CustomerAction::DebugStorage :
                Printer::debugStorage();
                break;
            case CustomerAction::PlaceOrder :
            {
                RestID_tp restaurantID = chooseRestaurant();
                MenuID_tp menuID = giveMenu(restaurantID);
                Printer::menu(menuID);
                Order order(user.getID(), {});
                if(orderOut(menuID, order)){
                    user.orderOut(order.getID());
                    OrderStorage oStorage;
                    if (oStorage.saveOrder(order, user.getID(), restaurantID)) {
                        Printer::checkoutInvoice(buildCheckoutSummary(membership, order));

                        const auto loyaltyUpdate = applyOrder(membership, order.getTotalPrice());
                        membership = loyaltyUpdate.next;
                        membershipSummary = buildSummary(membership);

                        if (persistMembershipForCustomer(user.getID(), membership)) {
                            if (loyaltyUpdate.levelUp) {
                                Printer::membershipUpgrade(
                                    upgradeMessage(loyaltyUpdate.previousLevel, loyaltyUpdate.nextLevel)
                                );
                            }
                        } else {
                            cout << "Could not update membership profile." << endl;
                        }
                        cout << "Order successfully created. Order ID: " << order.getID() << endl;
                    } else {
                        cout << "Could not create order at this time." << endl;
                    }
                }
                break;
            }
            case CustomerAction::ViewMyOrders :
            {
                Printer::customerOrders(user.historyOfOrders());
                break;
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
            user.getName(),
            user.getRestaurantAddress(),
            user.getRestaurantStatus(),
            user.getRestaurantPreparationMinutes()
        );

        if(chosenOption == RestaurateurAction::Quit){
            return;
        }
        if(chosenOption == RestaurateurAction::ClearScreen){
            Printer::clearScreen();
            continue;
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
            case RestaurateurAction::AddOrderToQueue:
            {
                RestID_tp restaurantID = user.getRestaurantID();
                MenuID_tp menuID = user.getMenuID();
                Order newOrder(user.getID(), {});
                if (orderOut(menuID, newOrder)) {
                    OrderStorage oStorage;
                    if (oStorage.saveOrder(newOrder, "", restaurantID)) {
                        // Order is already persisted and attached to the restaurant queue.
                    } else {
                        cout << "Could not create order at this time." << endl;
                    }
                }
                break;
            }
            case RestaurateurAction::RemoveOrderFromQueue:
                user.removeOrderFromQueue(GetInf::OrderID(RestaurateurAction::RemoveOrderFromQueue));
                break;
            case RestaurateurAction::SetOrderStatus:
                user.setOrderStatus();
                break;
            case RestaurateurAction::PrintSaleStatistics:
                user.updateAndPrintSaleStatistics();
                break;
            case RestaurateurAction::PrintCustomerStatistics:
                user.updateAndPrintCustomerStatistics();
                break;
            case RestaurateurAction::ShowCurrentOrders:
                user.showCurrentOrders();
                break;
            case RestaurateurAction::ShowOrderHistory:
                user.showOrderHistory();
                break;
            case RestaurateurAction::ShowMenu:
                user.showMenu();
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
    AdminID_tp adminID;
    AdminOfSystem user = enterAsAdmin(adminID);
    if(user.getName() == "Quit"){
        return;
    }
    AdminStorage adminStorage;
    RestaurateurStorage restaurateurStorage;
    RestaurantStorage restaurantStorage;
    MembershipLevelStorage membershipStorage;
    CustomerStorage customerStorage;

    while(true){
        AdminAction chosenOption = adminOptions(user.getRestaurateurIDs());
        if(chosenOption == AdminAction::Quit){
            return;
        }
        if(chosenOption == AdminAction::DebugStorage){
            Printer::debugStorage();
            continue;
        }
        if(chosenOption == AdminAction::ClearScreen){
            Printer::clearScreen();
            continue;
        }

        if(chosenOption == AdminAction::CreateRestaurant){
            cout << "Enter restaurateur ID to link this restaurant. ";
            cout << "(If the restaurateur ID is invalid, the restaurant will not be saved.) ";
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
                cout << "Restaurant created, but restaurateur could not be linked." << endl;
            }
            restaurateurStorage.updateRestaurateur(manager);
            cout << "Restaurant created. New restaurant ID: " << newRestaurantID << endl;
            user.addRestaurateur(restaurateurID);
            adminStorage.updateAdmin(adminID, user);
            continue;
        }
        if(chosenOption == AdminAction::CreateRestaurateur){
            Restaurateur newRestaurateur = GetInf::restaurateurFactory();
            if(restaurateurStorage.saveRestaurateur(newRestaurateur)){
                user.addRestaurateur(newRestaurateur.getID());
                adminStorage.updateAdmin(adminID, user);
                cout << "Restaurateur created. New ID: " << newRestaurateur.getID() << endl;
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
            adminStorage.updateAdmin(adminID, user);
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
        if(chosenOption == AdminAction::PrintMembershipLevelReport){
            Printer::membershipLevelReport(membershipStorage.countCustomersByLevel());
            continue;
        }
        if(chosenOption == AdminAction::ShowMembershipLevelHistory){
            CustID_tp targetCustomerID = GetInf::customerID();
            Printer::membershipLevelHistory(
                membershipStorage.levelHistory(targetCustomerID)
            );
            continue;
        }
        if(chosenOption == AdminAction::ChangeCustomerMembership){
            CustID_tp targetCustomerID = GetInf::customerID();
            if (!customerStorage.isValidCustomer(targetCustomerID)) {
                cout << "Invalid customer ID." << endl;
                continue;
            }
            const bool hadRecord = membershipStorage.hasMembershipLevel(targetCustomerID);
            Level targetLevel = GetInf::membershipLevel();
            point targetPoints = GetInf::membershipPoints();
            MembershipLevel updatedLevel = applyLevelTemplate(targetLevel, targetPoints);
            if (persistMembershipForCustomer(targetCustomerID, updatedLevel)) {
                cout << (hadRecord ? "Membership updated for " : "Membership created and set for ")
                     << "customer: " << targetCustomerID << endl;
            } else {
                cout << "Could not update membership profile." << endl;
            }
            continue;
        }
    }
}
