#include "Printer.h"

void Printer::InvalidInput() const
{
        cout << "Invalid input, please try again" << endl;
}
void Printer::wellcome(){
    cout << "Wellcome to Krusty Krab managemt system" << endl;
    cout << "Please choose your rule(by entering the number of each rule):" << endl;
    cout << "1. Customer" << endl;
    cout << "2. Restaurateur/In charge of a restaurant" << endl;
    cout << "3. Admin of system" << endl;
}