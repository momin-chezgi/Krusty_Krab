#include "Common/Types.h"
#include "Domain/Admin.h"
#include "Domain/Order.h"
#include "UI/Interface.h"
#include "Utility/IDGenerator.h"

bool CustomerDashboard();
bool RestaurateurDashboard();
bool AdminDashboard();

int main()
{
    // User logins and chooses the rule
    while (true)
    {    
        switch(Login())
        {
            case 1:
                if(CustomerDashboard()){
                    return 0;
                }
                break;
            case 2:
                if(RestaurateurDashboard()){
                    return 0;
                }
                break;
            case 3:
                if(AdminDashboard()){
                    return 0;
                }
                break;
            default:
                Printer::InvalidInput();
        }
    }
    return 0;
}


bool CustomerDashboard(){
    Customer user;
    if (!enterAsCustomer(user)){  
        // If the user doesn't want to enter as a customer,
        //  just return to the main menu
        return false;
    }
    RestID_tp restaurantID = chooseRestaurant();
    MenuID_tp menuID = giveMenu(restaurantID);
    Printer::menu(menuID);
    Order order = Order(user.getID(), {});
    orderOut(restaurantID, menuID, order);
    return true;
}

bool RestaurateurDashboard(){
    
    Restaurateur user = enterAsRestaurateur();

    if(user.getName() == "Quit"){
        return false;
    }

    int chosenOption = restaurateurOptions(
        user.getRestaurantName(),
        user.getRestaurantAddress(),
        user.getRestaurantStatus(),
        user.getRestaurantPreparationMinutes()
    );

    switch(chosenOption){
        
        // Restaurant editing options-----------------------
        case 1: // edit restaurant name
            user.editRestaurantName(GetInf::modifyRestaurantString(chosenOption));
            break;
        case 2: // edit restaurant address
            user.editRestaurantAddress(GetInf::modifyRestaurantString(chosenOption));
            break;
        case 3: // activate restaurant
            user.activateRestaurant();
            break;
        case 4: // deactivate restaurant
            user.deactivateRestaurant();
            break;
        case 5: // edit restaurant preparation time
            user.setPreparationTime(GetInf::modifyRestaurantTime(chosenOption));
            break;
        case 6: // edit restaurant phone number
            user.setPhoneNumber(GetInf::modifyRestaurantString(chosenOption));
            break;
        case 7: // edit restaurant bio
            user.setBio(GetInf::modifyRestaurantString(chosenOption));
            break;
        
        // Menu editing options----------------------------
        case 11: // add an item to menu
            user.addItemToMenu(GetInf::menuItem());
            break;
        case 12: // remove an item from menu
            user.removeItemFromMenu(GetInf::menuItem());
            break;
        case 13: // replace an item from menu
            user.replaceItemInMenu(GetInf::menuItem(), GetInf::menuItem());
            break;
        
        
        // Order editing options----------------------------
        case 21: { // add an order to the order queue
            Order newOrder;
            OrderStorage storage;
            orderOut(user.getRestaurantID(), user.getMenuID(), newOrder);
            storage.saveOrder(newOrder);
            user.addOrderToQueue(newOrder.getID());
            break;
        }
        case 22: { // remove an order from the order queue
            OrderStorage storage;
            OrderID_tp deletingOrderID = GetInf::OrderID();
            storage.deleteOrder(deletingOrderID);
            user.removeOrderFromQueue(deletingOrderID);
            break;
        }
        case 23: // replace an order to the order queue
            user.replaceOrderInQueue(GetInf::OrderID(chosenOption), GetInf::OrderID(chosenOption));
            break;

        // Statistics options----------------------------
        case 31: // update and print sale statistics
            user.updateAndPrintSaleStatistics();
            break;
        case 32: // update and print customer statistics
            user.updateAndPrintCustomerStatistics();
            break;
        
        // Restaurateur editing options-----------------------
        case 41: // edit restaurateur name
            user.setName(GetInf::modifyRestaurantString(chosenOption));
            break;
        case 42: // edit restaurateur's restaurant
            user.setRestaurantID(GetInf::modifyRestaurantString(chosenOption));
            break;
        default:
            Printer::InvalidInput();
    }
    
    return true;

}

bool AdminDashboard()
{
    AdminOfSystem user = enterAsAdmin();

    if(user.getName() == "Quit"){
        return false;
    }

    int chosenOption = adminOptions(user.getRestaurateurIDs());

    switch (chosenOption){
        case 1:         // creates restaurant
            user.addRestaurant(GetInf::restaurateur(),GetInf::newRestaurant());
            break;
        case 2: // activates restaurant (x: id of the restaurant)
            user.activateRestaurant(GetInf::modifyRestaurantString(chosenOption));
            break;
        case 3: // disactivates restaurant
            user.deactivateRestaurant(GetInf::modifyRestaurantString(chosenOption));
            break;
        
        // Statistics options----------------------------
        case 31: // receives sale statistics
            user.updateAndPrintTotalSaleStatistics();
            break;
        case 32: // recieves customers statistics
            user.updateAndPrintTotalCustomerStatistics();
            break;
        case 33: // update and print sale statistics
            user.updateAndPrintTotalSaleStatistics();
            break;
        case 34: // update and print customer statistics
            user.updateAndPrintTotalCustomerStatistics();
            break;
    }
    
    return true;

}
