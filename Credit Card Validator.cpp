#include <iostream>

using namespace std;

int n;

void printDevider(int len=30);

int main()
{
    cout << "CREDIT CARD VALIDATOR\n";
    printDevider();
    cout << "\n[1] Check info\n[2] How it works\n[3] Quit\nWrite the number of the action you want performed and press ENTER\n";

    while (true) {
        cin >> n;
        printDevider();
        if (n == 1) {
            cout << "Check info:\n";
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

void printDevider(int len) {
    for (int i = 0; i < len; i++) {
        cout << '-';
    }
    cout << '\n';

}

// TODO
// - Insert the credit card information
// - External cpp file + header file that checks the credit card
// - History of the cards inserted and their info

// KNOWN BUGS
// - Doenst work with non-int inputs
