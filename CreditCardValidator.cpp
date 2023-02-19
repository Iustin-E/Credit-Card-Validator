#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <ctime>
#include <sstream>

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
    std::string number; // a string with the numbers of the card - no spaces
    short expirMonth = 0, expirYear = 0;
    std::string owner_name;
};
std::vector<std::string> card_options;
std::vector<std::string> start_menu_options;
std::vector<std::string> database_operations;
std::vector<std::string> delete_database_options;
std::vector<std::string> operations_on_card;
std::vector<std::string> modify_card_options;


// FUNCTIONS
void printDevider(int len = 30) {
        for (int i = 0; i < len; i++) {
            std::cout << '-';
        }
        std::cout << '\n';
    }

void setupMenus() {
        card_options.push_back("Save card in database");
        card_options.push_back("Check card info");
        card_options.push_back("Exit");
        start_menu_options.push_back("Credit card operations");
        start_menu_options.push_back("Database operations");
        start_menu_options.push_back("How it works");
        start_menu_options.push_back("Quit");
        database_operations.push_back("Print current database");
        database_operations.push_back("Modity entry from database");
        database_operations.push_back("Delete DATABASE");
        database_operations.push_back("Restore to base test database");
        database_operations.push_back("Return to Main Menu");
        delete_database_options.push_back("YES, delete database");
        delete_database_options.push_back("NO, go back");
        operations_on_card.push_back("Modity card");
        operations_on_card.push_back("Delete card from database");
        operations_on_card.push_back("Return to cards page");
        modify_card_options.push_back("Owner's name");
        modify_card_options.push_back("Card number");
        modify_card_options.push_back("Return to Main Menu");
    }

int menu_select(std::string title, std::vector<std::string> options) {
        printDevider();
        std::cout << title << '\n';
        for (size_t i = 1; i <= options.size(); i++) {
            std::cout << '[' << i << ']' << ' ' << options[i - 1] << '\n';
        }
        int input;
        while (true) {
            std::cin >> input;
            if (!std::cin) { // verifies if the input fails
                std::cout << "Incorrect input!\n";
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                continue;
            }
            if (input <= 0 || (input > options.size())) {
                std::cout << "Option out of range!\n";
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

bool isCardNumberValid(std::string cardnumber) {
        if (!CARD_NUMBER_HAS_TO_BE_VALID)
            return true;
        int i = 0;
        // 16 for Visa and MC | 15 for AmExpr
        if (cardnumber.size() != 16 && cardnumber.size() != 15) {
            return false;
        }
        unsigned int sum = 0;
        for (int i = 0; i < 16; i++) {
            if (!isDigit(cardnumber[i]))
                return false;
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

std::string formatCardNumber(std::string temporaryInput) {
        std::string formatedInput = "";
        for (int i = 0; i < temporaryInput.size(); i++) {
            if (temporaryInput[i] != ' ') {
                formatedInput += temporaryInput[i];
            }
        }
        return formatedInput;
    }

bool isExpirDateValid(std::string input) {
        if (input.size() != 5) {
            return false;
        }
        if (isDigit(input[0]) && isDigit(input[1]) && isDigit(input[3]) && isDigit(input[4]) && input[2] == '/') {
            int expirMon = ((int)input[0] - '0') * 10 + ((int)input[1] - '0');
            int expirYear = ((int)input[3] - '0') * 10 + ((int)input[4] - '0');
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

int save_card(std::string database_name, Card card) {
        // Verify if name is taken
        // Varify if the card is under another name
        std::ofstream file;
        file.open(database_name, std::ios_base::app);
        file << card.owner_name << DATABASE_DELIMITER << card.number << DATABASE_DELIMITER << card.expirMonth << '/' << card.expirYear << '\n';
        file.close();
        return 0;
    }

bool isInDatabase(std::string database_name, Card card) {
        if (!CARD_DUPLICATE_PROOF)
            return false;
        std::ifstream file(database_name);
        std::string line;
        while (getline(file, line)) {
            // cout << line << endl;
            std::string owner, card_number, expirMon, expirYear;
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

std::vector<std::string> loadCards(std::string database) {
        std::vector<std::string> cards;
        std::ifstream file(database);
        std::string line;
        while (getline(file, line)) {
            cards.push_back(line);
        }
        return cards;
    }

void deleteDatabase(std::string database) {
        std::ifstream file(database);
        file.close();
    }

Card stringToCard(std::string line) {
    Card card;
    size_t pos;
    pos = line.find(DATABASE_DELIMITER) + DATABASE_DELIMITER_SIZE;
    card.owner_name = line.substr(0, pos);
    line.erase(0, pos);

    pos = line.find(DATABASE_DELIMITER) + DATABASE_DELIMITER_SIZE;
    card.number = line.substr(0, pos);
    line.erase(0, pos);

    card.expirMonth = (int)(line[0] - '0') * 10 + (int)(line[1] - '0');
    card.expirYear = (int)(line[3] - '0') * 10 + (int)(line[4] - '0');

    return card;
}

Card deleteAndReturnNthCard(std::string database, size_t n) {
    std::ifstream file(database);
    std::ofstream temp("tempfile.txt");
    Card card;
    size_t cnt = 0;
    std::string line;
    while (getline(file, line)) {
        if (cnt == n-1) {
            card = stringToCard(line);
        }
        else
            temp << line << '\n';
        cnt++;
    }
    temp.close();
    file.close();
    remove(database.c_str());
    if (rename("tempfile.txt", database.c_str())) {
        std::cout << "ERROR RENAMING!\n";
    }
    return card;
}

void printDatabase(std::string database) {
    // TODO print on columns and rows 
    std::ifstream file(database);
    std::string line;
    while (getline(file, line)) {
        std::cout << line << '\n';
    }
    file.close();
}

// MAIN
int main()
{
    setupMenus();
    std::cout << "CREDIT CARD VALIDATOR\n";
    while (true) {
        n = menu_select("MAIN MENU", start_menu_options);
        // INSERT CARD
        if (n == 1) // ENTER CREDIT CARD
        {
            Card curr_card;
            std::cout << "Enter credit card informations:\n";

            std::cout << "Credit card's owner name: ";
            std::cin.ignore();
            getline(std::cin, curr_card.owner_name);

            std::cout << "Credit card number: ";
            std::string input;
            getline(std::cin, input);
            // CARD SKIPPER [DEBUG]
            if (input == "-")
                input = "5120415296389632";
            input = formatCardNumber(input);
            // cout << "[DEBUG] FORMATED CARD NUMBER: " << input << '\n';
            while (!isCardNumberValid(input)) {
                std::cout << "INVALID: " << input << '\n';
                std::cout << "Credit card number: ";
                getline(std::cin, input);
                input = formatCardNumber(input);
                std::cout << "FORMATED CARD NUMBER: " << input << '\n';
            }
            //cout << "VALID: " << input << '\n';
            curr_card.number = input;

            std::cout << "Expiration date(MM/YY): ";
            getline(std::cin, input);
            if (input.size() == 4)
                input.insert(input.begin(), '0');
            while (!isExpirDateValid(input)) {
                std::cout << "Card NOT valid!\n" << "Expiration date(MM/YY): ";
                getline(std::cin, input);
            }
            std::cout << "Card is valid!\n";
            curr_card.expirMonth = ((int)(input[0] - '0')) * 10 + ((int)(input[1] - '0'));
            curr_card.expirYear = ((int)(input[3] - '0')) * 10 + ((int)(input[4] - '0'));

            // CREDIT CARD OPERATIONS
            while (true) {
                n = menu_select("CREDIT CARD OPERATIONS", card_options);
                if (n == 1) { // SAVE CARD
                    if (isInDatabase(DATABASE_FILE, curr_card)) {
                        std::cout << "Card already in database\n";
                        continue;
                    }
                    if (save_card(DATABASE_FILE, curr_card)) {
                        std::cout << "Couldn't save the card!\n";
                    }
                    else
                    {
                        std::cout << "Card saved successfully!\n";
                    }
                }

                else if (n == 2) { // CREDIT CARD INFORMATION
                    std::cout << "Current card information:\n";
                    std::cout << "Owner's name: " << curr_card.owner_name << '\n';
                    std::cout << "Card number: " << curr_card.number << '\n';
                    std::cout << "Expiration date: " << curr_card.expirMonth << '/' << curr_card.expirYear << '\n';

                    std::cout << "Type: ";
                    if (curr_card.number[0] == '4') std::cout << "Visa\n";
                    else if (curr_card.number[0] == '5') std::cout << "Mastercard\n";
                    else if (curr_card.number[0] == '3' && (curr_card.number[1] == '4' || curr_card.number[1] == 7))
                        std::cout << "American Express\n";
                    else std::cout << "Unknown\n";

                    std::cout << "The selected card ";
                    if (isInDatabase(DATABASE_FILE, curr_card))
                        std::cout << "IS";
                    else
                        std::cout << "IS NOT";
                    std::cout << " currently in the database\n";
                }
                if (n == 3)
                    break;
            }

        }
        // DATABASE OPERATIONS
        else if (n == 2) // DATABASE OPERATIONS
        {
            while (true) {
                n = menu_select("DATABASE OPERATIONS", database_operations);
                if (n == 1) // PRINT DATABASE
                    printDatabase(DATABASE_FILE);
                else if (n == 2) // MODIFY ENTRY
                {
                    std::vector<std::string> select_card = loadCards(DATABASE_FILE);
                    // SELECT CARD
                    n = menu_select("SELECT CARD", select_card);
                    Card selected_card = deleteAndReturnNthCard(DATABASE_FILE, n);
                    // SELECT OPERATION FOR THE SELECTED CARD
                    n = menu_select("SELECT OPERATION for the selected card", operations_on_card);
                    if (n == 1) // Modify card
                    {
                        while (true) {
                            n = menu_select("Select what you want to modify", modify_card_options);
                            std::string newStr;
                            if (n == 1) { // Modify name
                                std::cout << "Enter new owner name: ";
                                std::cin.ignore();
                                getline(std::cin, newStr);
                                selected_card.owner_name = newStr;
                            }
                            else if (n == 2) { // Modify number 
                                std::cout << "Enter new credit card number\n";
                                std::cin.ignore();
                                getline(std::cin, newStr);
                                newStr = formatCardNumber(newStr);
                                while (!isCardNumberValid(newStr)) {
                                    getline(std::cin, newStr);
                                    if (newStr == "q")
                                        break;
                                    std::cout << "Invalid! Enter \"q\" to leave\n";
                                }
                                if (newStr != "q")
                                    selected_card.number = newStr;
                            }
                            else if (n == 3) { // Leave
                                break;
                            }
                        }
                        save_card(DATABASE_FILE, selected_card);
                    }
                    else if (n == 2) // Delete from database
                    {
                        continue;
                    }
                    else break;
                }

                else if (n == 3)  // DELETE DATABSE
                {
                    int n = menu_select("ARE YOU SURE YOU WANT TO DELETE THE DATABASE?", delete_database_options);
                    if (n == 1) { // YES
                        deleteDatabase(DATABASE_FILE);
                        std::cout << "Database DELETED!\n";
                    }
                    else if (n == 2) // NO
                        continue;
                }
                else if (n == 4) { // TODO restore to base database
                    continue;
                }
                else break;
            }
        }
        // HOW IT WORKS
        else if(n == 3)
        {
            std::cout << "HOW IT WORKS\n";
            std::cout << "You can insert a fictional credit card's information into the system\nand it will verity if it is a valid credit card.\n\nYou can find more about this project on the README file on: \ngithub.com/Iustin-E/Credit-Card-Validator.git\n";
            std::vector<std::string> how_options;
            how_options.push_back("Exit");
            int n = menu_select("", how_options);
            break;
        }
        // EXIT
        else exit(0);
        }
    }

