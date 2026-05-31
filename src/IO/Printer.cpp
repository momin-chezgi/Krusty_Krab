#include "Printer.h"

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

void Printer::menu(string menuID)
{
    MenuStorage storage;

    Menu menu = storage.giveMenu(menuID);

    cout << "Menu: " << endl;
    for (const auto& item : menu.getMenu())
    {
        if(item.isAvailable()){
            cout << item.getName() << " - " << item.getBio() << " - Cost: "<< item.getPricePerUnit() << " - Time: " << item.getPreparationMinutes() << " - ID: "<< item.getID() << endl;
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

