#include <iostream>
#include <string>
#include <ctime>

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

bool isDigit(char a) {
    if (a >= '0' && a <= '9') {
        return true;
    }
    return false;
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

bool isExpirDateValid(string input) {
    if (input.size() == 4 && isDigit(input[0]) && input[1] == '/' && isDigit(input[2]) && isDigit(input[3])) {
        input.insert(input.begin(), '0');
    }
    if ( input.size() != 5 || (input.size() != 4 && input[0] == 0)) {
        return false;
    }
    if (isDigit(input[0]) && isDigit(input[1]) && isDigit(input[3]) && isDigit(input[4]) && input[2] == '/') {
        int expirMon = ((int)input[0] - '0') * 10 + ((int)input[1]-'0');
        int expirYear = ((int)input[3] - '0') * 10 + ((int)input[4]-'0');
        time_t curr_time = time(nullptr);
        struct tm converted_time;
        localtime_s(&converted_time, &curr_time);
        // the localtime func returns number of years passed till 1970 - unix timestamp
        int currYear = converted_time.tm_year - 100;
        int currMon = converted_time.tm_mon;
        if (currYear <= expirYear && currMon <= expirMon)
            return true;
        return false;
    }
    return false;
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
            // CARD SKIPPER [DEBUG]
            if (input == "-")
                input = "5120415296389632";
            input = formatCardNumber(input);
            cout << "FORMATED CARD NUMBER: " << input << '\n';
            while (!isCardNumberValid(input)) {
                cout << "INVALID: " << input << '\n';
                cout << "Credit card number: ";
                getline(cin, input);
                input = formatCardNumber(input);
                cout << "FORMATED CARD NUMBER: " << input << '\n';
            }
            //cout << "VALID: " << input << '\n';

            cout << "Expiration date(MM/YY): ";
            getline(cin, input);
            while (!isExpirDateValid(input)) {
                cout << "Card NOT valid!\n" << "Expiration date(MM/YY): ";
                getline(cin, input);
            }
            cout << "Card is valid!\n";
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

