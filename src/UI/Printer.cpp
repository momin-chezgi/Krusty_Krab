#include "UI/Printer.h"
#include "Repository/MenuStorage.h"

void Printer::InvalidInput()
{
        cout << "Invalid input, please try again" << endl;
}
void Printer::wellcome()
{
    cout << "Wellcome to Krusty Krab management system" << endl;
    cout << "Please choose your rule(by entering the number of each rule):" << endl;
    cout << "1. Customer" << endl;
    cout << "2. Restaurateur/In-charge of a restaurant" << endl;
    cout << "3. Admin of the system" << endl;
}

void Printer::chooseRestaurant()
{
    cout << "Please choose a restaurant by entering its ID: " << endl;
}

void Printer::menu(MenuID_tp menuID)
{
    MenuStorage storage;

    Menu menu = storage.giveMenu(menuID);

    cout << "Menu: " << endl;
    for (const auto& item : menu.getMenu())
    {
        if(item->isAvailable()){
            cout << item->getName() << " - " << item->getBio() << " - Cost: "<< item->getPricePerUnit() << " - Time: " << item->getPreparationMinutes() << " - ID: "<< item->getID() << endl;
        }
    }
}

void Printer::orderOut()
{
    cout << "Do you want to order? (y/n) " << endl;
}

void Printer::addItemToCart()
{
    cout << "Enter the ID of the item you want to add to your cart(q for finalizing and quitting): " << endl;
}

void Printer::restaurateurDashboard(string nm,
         string ad
         , bool isactive
         , size_t averaget)
{
    cout << "Welcome to your dashboard, " << nm << "!" << endl;
    cout << "Your restaurant is located at: " << ad << endl;
    cout << "Your restaurant is currently " << (isactive ? "active" : "inactive") << endl;
    cout << "The average preparation time for your restaurant is: " << averaget << " minutes" << endl << endl;

}

void Printer::RestaurateurChoices()
{
    cout << "Please choose an option by entering the number of each option:" << endl;
    cout << "1. Edit restaurant name" << endl;
    cout << "2. Edit restaurant address" << endl;
    cout << "3. Activate restaurant" << endl;
    cout << "4. Deactivate restaurant" << endl;
    cout << "5. Edit restaurant preparation time" << endl;
    cout << "6. Edit restaurant phone number" << endl;
    cout << "7. Edit restaurant bio" << endl;

    cout << "\nMenu editing options:" << endl;
    cout << "11. Add an item to menu" << endl;
    cout << "12. Remove an item from menu" << endl;
    cout << "13. Replace an item in menu" << endl;

    cout << "\nOrder editing options:" << endl;
    cout << "21. Add an order to the order queue" << endl;
    cout << "22. Remove an order from the order queue" << endl;
    cout << "23. Replace an order in the order queue" << endl;
}
