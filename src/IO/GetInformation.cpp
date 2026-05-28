#include "GetInformation.h"

string GetInf::customerName(){
    cout << "What is your name? ";
    string enteredName;
    cin >> enteredName;
    return enteredName;
}
int GetInf::loginRule()
{
    int chosenRule{-1};
    cin >> chosenRule;
    while(chosenRule < 1 || chosenRule > 3){
        Printer::InvalidInput();
        cin >> chosenRule;
    }
    return chosenRule;
}

auto GetInf::restaurateurAction(){
    return Printer::RestaurateurChoices();
}