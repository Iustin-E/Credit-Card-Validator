#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <ctime>
#include <sstream>

using namespace std;

// ENABLE/DISABLE FUNCTIONS
#define CARD_DUPLICATE_PROOF 1
#define CARD_NUMBER_HAS_TO_BE_VALID 0

// DATABASE INFO
#define DATABASE_FILE "card_database.txt"
#define DATABASE_COLUMNS 4
#define DATABASE_DELIMITER " : "
#define DATABASE_DELIMITER_SIZE 3

// VARIABLES
int n;
struct Card {
    string number; // a string with the numbers of the card - no spaces
    short expirMonth = 0, expirYear = 0;
    string owner_name;
};

// FUNCTIONS
void printDevider(int len = 30) {
    for (int i = 0; i < len; i++) {
        cout << '-';
    }
    cout << '\n';
}

int menu_select(string title, vector<string> options) {
    cout << title << '\n';
    for (size_t i = 1; i <= options.size(); i++) {
        cout << '[' << i << ']' << ' ' << options[i - 1] << '\n';
    }
    int input;
    while (true) {
        cin >> input;
        if(!cin) { // verifies if the input fails
        cout << "Incorrect input!\n";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        continue;
        }
        if (input <= 0 || (input > options.size())) {
            cout << "Option out of range!\n";
        }
        else break;
    }
    return input;
}

bool isDigit(char a) {
    if (a >= '0' && a <= '9') {
        return true;
    }
    return false;
}

bool isCardNumberValid(string cardnumber) {
    if (!CARD_NUMBER_HAS_TO_BE_VALID)
        return true;
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
    if ( input.size() != 5) {
        return false;
    }
    if (isDigit(input[0]) && isDigit(input[1]) && isDigit(input[3]) && isDigit(input[4]) && input[2] == '/') {
        int expirMon = ((int)input[0] - '0') * 10 + ((int)input[1]-'0');
        int expirYear = ((int)input[3] - '0') * 10 + ((int)input[4]-'0');
        if (expirMon <= 0 || expirMon > 12)
            return false;
        time_t curr_time = time(nullptr);
        struct tm converted_time;
        localtime_s(&converted_time, &curr_time);
        // the localtime func returns number of years passed till 1970 - unix timestamp
        int currYear = converted_time.tm_year - 100;
        int currMon = converted_time.tm_mon;
        if (currYear <= expirYear && currMon <= expirMon) {
            return true;
        }
        return false;
    }
    return false;
}

int save_card(string database_name, Card card) {
    // Verify if name is taken
    // Varify if the card is under another name
    ofstream file;
    file.open(database_name, std::ios_base::app);
    file << card.owner_name << DATABASE_DELIMITER << card.number << DATABASE_DELIMITER << card.expirMonth << '/' << card.expirYear << '\n';
    file.close();
    return 0;
}

bool isInDatabase(string database_name, Card card) {
    if(!CARD_DUPLICATE_PROOF)
        return false;
    ifstream file(database_name);
    string line;
    while (getline(file, line)) {
        cout << line << endl;
        string owner, card_number, expirMon, expirYear;
        // START ESTE AIUREA - DA CV NR MARE 
        size_t start = line.find(DATABASE_DELIMITER);
        line.erase(0, start + DATABASE_DELIMITER_SIZE);
        start = line.find(DATABASE_DELIMITER);
        card_number = line.substr(0, start);
        if (card_number == card.number) {
            return true;
        }
    }
    file.close();
    return false;
}


// MAIN
int main()
{
    cout << "CREDIT CARD VALIDATOR\n";
    printDevider();

    while (true) {
        vector<string> start_menu_options;
        start_menu_options.push_back("Credit card operations");
        start_menu_options.push_back("How it works");
        start_menu_options.push_back("Quit");
        int n = menu_select("CHOOSE OPTION", start_menu_options);
        printDevider();
        if (n == 1) {
            Card curr_card;
            cout << "Enter credit card informations:\n";

            cout << "Credit card's owner name: ";
            cin.ignore();
            getline(cin, curr_card.owner_name);

            cout << "Credit card number: ";
            string input;
            getline(cin, input);
            // CARD SKIPPER [DEBUG]
            if (input == "-")
                input = "5120415296389632";
            input = formatCardNumber(input);
            // cout << "[DEBUG] FORMATED CARD NUMBER: " << input << '\n';
            while (!isCardNumberValid(input)) {
                cout << "INVALID: " << input << '\n';
                cout << "Credit card number: ";
                getline(cin, input);
                input = formatCardNumber(input);
                cout << "FORMATED CARD NUMBER: " << input << '\n';
            }
            //cout << "VALID: " << input << '\n';
            curr_card.number = input;

            cout << "Expiration date(MM/YY): ";
            getline(cin, input);
            if (input.size() == 4)
                input.insert(input.begin(), '0');
            while (!isExpirDateValid(input)) {
                cout << "Card NOT valid!\n" << "Expiration date(MM/YY): ";
                getline(cin, input);
            }
            cout << "Card is valid!\n";
            curr_card.expirMonth = ((int)(input[0] - '0')) * 10 + ((int)(input[1] - '0'));
            curr_card.expirYear = ((int)(input[3] - '0')) * 10 + ((int)(input[4] - '0'));

            // SAVE IN FILE
            while (true) {
                printDevider();
                vector<string> card_options;
                card_options.push_back("Save card in database");
                card_options.push_back("Check card info");
                card_options.push_back("Exit");
                int n2 = menu_select("CHOOSE OPTION", card_options);
                if (n2 == 1) {
                    if (isInDatabase(DATABASE_FILE, curr_card)) {
                        cout << "Card already in database\n";
                        continue;
                    }
                    if (save_card(DATABASE_FILE, curr_card)) {
                        cout << "Couldn't save the card!\n";
                    }
                    else
                    {
                        cout << "Card saved successfully!\n";
                    }
                }
                else if (n2 == 2) {
                    // CHECK CARD INFO:
                    // INPUTED DATA
                    // VISA/MC/AMEXPR
                    // SAVED OR NOT IN DATABASE
                }
                else
                    break;
            }
        }
        else if (n == 2) {
            cout << "How it works:\n";
            // INFO ABOUT HOW IT WORKS
        }
        else if (n == 3) {
            exit(0);
        }
        printDevider();
    }

}

