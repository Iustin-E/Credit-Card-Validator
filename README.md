# Summary
This project is about building a program that can verify and show the details of a credit card by the information you provide.
Some of the options are not completed. The project is still Work in Progress. 
# How to run
This program doesn't use any non-standard libraries, so you don't have to install anything before running.
The program was written for C++14 but it should work in any version.

You can run the program using Visual Studio IDE

# How does it work
The app uses a menu to navigate all the options. You can insert into the terminal the desired number and press `Enter` to select the option.
The app validates the Credit Card Number using the Luhn Algorithm and the Credit Card Expiration Date using the current system time and gives you information about the card and the option to store the card into a local '.txt' file.
You can add entries in the database, remove them or modify them. 

By default, the app only accept valid cards, if you want to test the database you can change the options of the app modifying the `#DEFINE ...` section or when it askes you for the credit card numner you can type `-` and it will automaticaly insert a valid credit card number for you.

# What did I learned while developing this project
In this project I leaned about:
- Usage of STL
- Working with strings
- Working with files
- Setting up more functions for better code readability

References:
- Luhn Algorithm: https://en.wikipedia.org/wiki/Luhn_algorithm
- https://blog.unibulmerchantservices.com/how-to-recognize-a-valid-credit-card/
- Generate cards to test with: https://www.vccgenerator.org/
