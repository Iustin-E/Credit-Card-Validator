#include <iostream>
#include <string>

using namespace std;

// VARIABLES
int n;
struct Card {
    string number; // a string with the numbers of the card - no spaces
    unsigned short expirMonth = 0, expirYear = 0, CVV = 0;
};

// FUNCTIONS
void printDevider(int len = 30) {
    for (int i = 0; i < len; i++) {
        cout << '-';
    }
    cout << '\n';
}

bool isCardNumberValid(string cardnumber) {
    int i = 0;
    // 16 for Visa and MC | 15 for AmExpr
    if (cardnumber.size() != 16 && cardnumber.size() != 15) {
        return false;
    }
    unsigned int sum = 0;
    for (int i = 0; i < 16; i++) {
        int nr = cardnumber[i] - '0';
        if (i % 2 == 0) {
            if (nr * 2 > 9) {
                sum += (nr * 2) % 10 + (nr * 2) / 10;
            }
            else {
                sum += nr * 2;
            }
        }
        else {
            sum += nr;
        }
    }
    //cout << "[DEBUG] SUM = " << sum << '\n';
    if (sum % 10 != 0)
        return false;
    return true;
}

string formatCardNumber(string temporaryInput) {
    string formatedInput = "";
    for (int i = 0; i < temporaryInput.size(); i++) {
        if (temporaryInput[i] != ' ') {
            formatedInput += temporaryInput[i];
        }
    }
    return formatedInput;
}



// MAIN
int main()
{
    cout << "CREDIT CARD VALIDATOR\n";
    printDevider();
    cout << "\n[1] Check info\n[2] How it works\n[3] Quit\nWrite the number of the action you want performed and press ENTER\n";

    while (true) {
        cout << "CHOOSE OPTION: \n";
        cin >> n;
        printDevider();
        if (n == 1) {
            Card card;
            cout << "Check info:\n";
            cout << "Credit card number: ";
            string input;
            cin.ignore();
            getline(cin, input);
            input = formatCardNumber(input);
            cout << "FORMATED CARD NUMBER: " << input << '\n';
            while (!isCardNumberValid(input)) {
                cout << "INVALID: " << input << '\n';
                getline(cin, input);
                input = formatCardNumber(input);
            }
            cout << "VALID: " << input << '\n';
            // temporaryInput trebuie formatat si introdus in card.number
            

            cout << "Expiration date: ";

            
        }
        else if (n == 2) {
            cout << "How it works:\n";
        }
        else if (n == 3) {
            exit(0);
        }
        else {
            cout << "Invalid option. Try again\n";
        }
        printDevider();
    }

}

