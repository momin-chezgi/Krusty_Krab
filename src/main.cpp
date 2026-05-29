#include "neededLibs.h"
#include "./IO/Interface.h"
#include "./Management/Order.h"
#include "./Utils/IDGenerator.h"
#include "./Management/Admin.h"

void CustomerDashboard();
void RestaurateurDashboard();
void AdminDashboard();

int main()
{
    // User logins and chooses the rule
    switch(Login())
    {
        case 1:
            CustomerDashboard();
            break;
        case 2:
            RestaurateurDashboard();
            break;
        case 3:
            AdminDashboard();
            break;
    }

    return 0;
}


void CustomerDashboard(){
    Customer *user = enterAsCustomer();
    if(!user) return;   
    size_t restaurantID = chooseRestaurant();
    Menu menu = giveMenu(restaurantID);
    Printer::menu(&menu);
    Order order = Order(user);
    while(orderAtGivenBuffer(&order));
    return;
}
void RestaurateurDashboard(){
    Restaurateur *user = enterAsRestaurateur();
    if (!user){
        return;
    }
    auto decision = restaurateurOptions(user->getRestaurantName(), user->getRestaurantAddress(), user->getRestaurantStatus(), user->getRestaurantPreparationMinutes());
    int chosenOption = decision[0];
    auto x = decision[1];
    auto y = decision[2];
    switch (chosenOption)
    {
    case 1: // Change price of an item(x:MenuItem ptr)
        x->setPrice(y);
        break;
    case 2: // Change bio of an item(x:MenuItem ptr, y:the string of bio)
        x.setBio(y);
        break;
    case 3: // Change availability status of an item
        // It is not implemented yet, because we relate the availability
        //of an item to its quantity(grams, slices, litres) not in the 
        //restaurateur order(and likely won't)
        break;
    case 4: // remove an item from menu(x:MenuItem ptr, y:the menu ptr)
        y->removeItem(x);
        break;
    case 11: // change an order totaly(x:Order ptr, y: the new order)
        x->copyFromOrder(y);
        break;
    case 12: // remove an order from the order queue
        user->removeOrderFromQueue(y);
        break;
    case 13: // change the status of an order( x: Order ptr, y: string of the order status)
        x->setOrderStatus(y);
        break;
    }
}
void AdminDashboard(){
    AdminOfSystem* user = enterAsAdmin();
    if(!user){
        return;
    }
    auto decision = adminOptions();
    int chosenOption = decision[0];
    auto x = decision[1];
    switch (chosenOption){
        case 1:         // creates restaurant
            user->addRestaurant(x);
            break;
        case 2: // activates restaurant (x: id of the restaurant)
            user->accessRestaurant(x)->activateRestaurant();
            break;
        case 3: // disactivates restaurant
            user->accessRestaurant(x)->deactivateRestaurant();
            break;
        case 11: // receives sale statistics
            user->updateAndPrintTotalSaleStatistics();
            break;
        case 12: // recieves customers statistics
            user->updateAndPrintTotalCustomerStatistics();
            break;
        }
}
