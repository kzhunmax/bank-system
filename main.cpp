// Code have features that were added in c++17, thats why compile with c++17 flags using clang++, g++ or other compilers
#include <cstdio>
#include <iostream>
#include <sstream> 
#include <fstream>
#include <string>
#include <random>
#include <map>

using std::string;
using std::cin;
using std::cout;
using std::endl;

class Account {
public:
    std::map<unsigned int, bool> cardNumbers;
    string username;
    string fullName;
    string password;
    double balance;
    Account* next;

    void deposit(double amount) {
        balance += amount;
    }

    bool withdraw(double amount) {
        if (balance >= amount) {
            balance -= amount;
            return true;
        } else {
            return false;
        }
    }
};

// Global Variable
std::map<string, Account*> users;
string dataFile = "accounts.txt";

// Functions Prototypes
bool isUsernameUnique(const string& username);
unsigned int generateCardNumber();
bool isCardNumberUnique(const unsigned int& cardNumber);
void addOneMoreCard(Account* account);
Account* findAccount(const string& username, const string& password);
void showAccountDetails(const string& username, const string& password);
void depositFunds(const string& username, const string& password, double amount);
void withdrawFunds(const string& username, const string& password, double amount);
void displayAccounts();
void saveAccountsToFile();
void loadAccountsFromFile();
void userMenu();
void adminMainMenu();
void adminAccountManagementMenu();
void mainMenu();
void loginInAccount();
void registerAccount();

unsigned int generateCardNumber() {
    std::random_device rd;
    std::mt19937 mersenne(rd()); 
    return mersenne();    
}

void addOneMoreCard(Account* account) {
    if (account != nullptr) {
        unsigned int newCardNumber = generateCardNumber();
        account->cardNumbers[newCardNumber] = true;  // Mark the new card as active
        cout << "\nNew Card Added Successfully!" << endl;
        saveAccountsToFile();
    } else {
        cout << "\nUser not found!" << endl;
    }
}

bool isCardNumberUnique(const unsigned int& cardNumber) {
    for (const auto& pair : users) {
        const auto& cardNumbers = pair.second->cardNumbers;
        if (cardNumbers.find(cardNumber) != cardNumbers.end()) {
            return false;  
        }
    }
    return true;  
}

bool isUsernameUnique(const string& username) {
    return users.find(username) == users.end();
}

Account* findAccount(const string& username, const string& password) {
    auto it = users.find(username);
    if (it != users.end() && it->second->password == password) {
        return it->second;
    }
    return nullptr;
}

Account* findAccountAdminDetails(const string& username) {
    auto it = users.find(username);
    if (it != users.end()) {
        return it->second;
    }
    return nullptr;
}

void showAccountDetails(const string& username, const string& password) {
    Account* account = findAccount(username, password);
    if (account != nullptr) {
        cout << "\n--- Account Details ---" << endl;
        cout << "Username: " << account->username << endl;
        cout << "Full Name: " << account->fullName << endl;
        cout << "Balance: " << account->balance << endl;
        cout << "Card Numbers:" << endl;
        for (const auto& [cardNumber, isActive] : account->cardNumbers) {
            cout << cardNumber << (isActive ? " (Active)" : " (Banned)") << endl;
        }
    } else {
        cout << "\nAccount Not Found or Invalid Password!" << endl;
    }
}

void depositFunds(const string& username, const string& password, double amount) {
    Account* account = findAccount(username, password);
    if (account != nullptr) {
        account->deposit(amount);
        cout << "\nDeposit Successful!" << endl;
        saveAccountsToFile();
    } else {
        cout << "\nAccount Not Found or Invalid Password!" << endl;
    }
}

void withdrawFunds(const string& username, const string& password, double amount) {
    Account* account = findAccount(username, password);
    if (account != nullptr) {
        if (account->withdraw(amount)) {
            cout << "\nWithdrawal Successful!" << endl;
            saveAccountsToFile();
        } else {
            cout << "\nInsufficient Funds!" << endl;
        }
    } else {
        cout << "\nAccount Not Found or Invalid Password!" << endl;
    }
}

void displayAccounts() {
    cout << "\n--- All Accounts ---" << endl;
    for (const auto& pair : users) {
        Account* account = pair.second;
        cout << "\nUsername: " << account->username << endl;
        cout << "Full Name: " << account->fullName << endl;
        cout << "Balance: " << account->balance << endl;
        cout << "Card Numbers:" << endl;
        for (const auto& [cardNumber, isActive] : account->cardNumbers) {
            cout << cardNumber << (isActive ? " (Active)" : " (Banned)") << endl;
        }
    }
    cout << endl;
}

void searchAccount() {
    string username;
    cout << "Enter username of user you want to find: ";
    cin >> username;
    cin.ignore();
    Account* account = findAccountAdminDetails(username);
    if (account != nullptr) {
        cout << "\n--- Found Account Details ---" << endl;
        cout << "Full Name: " << account->fullName << endl;
        cout << "Balance: " << account->balance << endl;
        cout << "Card Numbers:" << endl;
        for (const auto& [cardNumber, isActive] : account->cardNumbers) {
            cout << cardNumber << (isActive ? " (Active)" : " (Banned)") << endl;
        }
    } else {
        cout << "\nAccount Not Found or Invalid Password!" << endl;
    }
}

void blockUserCard() {
    string username;
    cout << "Enter the username: ";
    cin >> username;
    cin.ignore();
    unsigned int cardNumber;
    cout << "Enter the card number: ";
    cin >> cardNumber;
    cin.ignore();

    auto it = users.find(username);
    if (it != users.end()) {
        auto& cardNumbers = it->second->cardNumbers;
        auto card = cardNumbers.find(cardNumber);
        if (card != cardNumbers.end()) {
            int choice;
            cout << "Select Action:" << endl;
            cout << "1. Ban" << endl;
            cout << "2. Unban" << endl;
            cout << "3. Return" << endl;
            cin >> choice;
            switch(choice) {
                case 1:
                    card->second = false; // Ban the card
                    cout << "Card " << cardNumber << " for user " << username << " has been banned." << endl;
                    break;
                case 2:
                    card->second = true; // Unban the card
                    cout << "Card " << cardNumber << " for user " << username << " has been unbanned." << endl;
                    break;
                case 3:
                    return;
                default:
                    cout << "Invalid Choice!" << endl;
            }
            saveAccountsToFile();
        } else {
            cout << "Card " << cardNumber << " not found for user " << username << endl;
        }
    } else {
        cout << "User " << username << " not found!" << endl;
    }
}

void saveAccountsToFile() {
    std::ofstream outputFile(dataFile);
    if (outputFile.is_open()) {
        for (const auto& pair : users) {
            Account* account = pair.second;
            outputFile << account->username << "," << account->fullName << "," << account->password << "," << account->balance;

            for (const auto& [cardNumber, isActive] : account->cardNumbers) {
                outputFile << "," << cardNumber << "," << (isActive ? "1" : "0");
            }

            outputFile << std::endl;
        }
        outputFile.close();
        cout << "\nAccounts saved to file: " << dataFile << endl;
    } else {
        cout << "\nError: Unable to open file for writing." << endl;
    }
}

void loadAccountsFromFile() {
    std::ifstream inputFile(dataFile);
    if (inputFile.is_open()) {
        string line;
        while (getline(inputFile, line)) {
            std::cout << "Reading line: " << line << std::endl;
            std::stringstream ss(line);
            string username, fullName, password, cardNumber, isActive;
            double balance;

            getline(ss, username, ',');
            getline(ss, fullName, ',');
            getline(ss, password, ',');
            ss >> balance;

            Account* newAccount = new Account;
            newAccount->username = username;
            newAccount->fullName = fullName;
            newAccount->password = password;
            newAccount->balance = balance;
            getline(ss, cardNumber, ',');
            // Reading card numbers and their statuses
            while (getline(ss, cardNumber, ',')) {
                if (!cardNumber.empty()) { // Check if card number is not empty
                    if (getline(ss, isActive, ',')) { // Check if status is available
                            unsigned int card = std::stoul(cardNumber); // Convert string to unsigned integer
                            bool active = (isActive == "1");
                            newAccount->cardNumbers[card] = active;
                    } else {
                        std::cout << "Missing card status for user " << username << " with card number: " << cardNumber << std::endl;
                    }
                } else {
                    std::cout << "Missing card number for user " << username << std::endl;
                }
            }
            users[username] = newAccount;
        }
        inputFile.close();
        cout << "\nAccounts loaded from file: " << dataFile << endl;
    } else {
        cout << "\nFile doesn't exist" << endl;
        std::ofstream outputFile(dataFile); 
        cout << "Automatically created accounts file: " << dataFile << endl;
    }
}

void userMenu(const string& username, const string& password) {
    int choice;
    while (true) {
        cout << "\n--- Bank Management System ---" << endl;
        cout << "1. Add One More Card" << endl;
        cout << "2. Account Details" << endl; 
        cout << "3. Deposit Funds" << endl;
        cout << "4. Withdraw Funds" << endl;
        cout << "5. Exit" << endl;
        cout << "Enter Your Choice: ";
        cin >> choice;
        switch(choice) {
            case 1:
                addOneMoreCard(findAccount(username, password));
                break;
            case 2:
                showAccountDetails(username, password);
                break;
            case 3:
                double depositAmount;
                cout << "Enter Deposit Amount: ";
                cin >> depositAmount;
                depositFunds(username, password, depositAmount);
                break;
            case 4:
                double withdrawAmount;
                cout << "Enter Withdrawal Amount: ";
                cin >> withdrawAmount;
                withdrawFunds(username, password, withdrawAmount);
                break;
            case 5:
                return;
            default:
                cout << "Invalid Choice!" << endl;
        }
    }
}

void adminMainMenu() {
    int choice;
    while (true) {
        cout << "\n--- Admin Main Menu ---" << endl;
        cout << "1. Search Account" << endl;
        cout << "2. Display All Accounts" << endl;
        cout << "3. Ban/Unban user" << endl;
        cout << "4. Exit" << endl;
        cout << "Enter Your Choice: ";
        cin >> choice;
        switch(choice) {
            case 1:
                searchAccount();
                break;
            case 2:
                displayAccounts();
                break;
            case 3:
                blockUserCard();
                break;
            case 4:
                return;
            default:
                cout << "Invalid Choice!" << endl;
        }
    }
}

void mainMenu() {
    int choice;
    while (true) {
        cout << "\n--- Bank Management System ---" << endl;
        cout << "1. Register" << endl;
        cout << "2. Login" << endl;
        cout << "3. Exit" << endl;
        cout << "Enter Your Choice: ";
        cin >> choice;
        switch(choice) {
            case 1:
                registerAccount();
                break;
            case 2:
                loginInAccount();
                break;
            case 3:
                return;
            default:
                cout << "Invalid Choice!" << endl;
        }
    }
}

void loginInAccount() {
    string username, password;
    cout << "Enter Username: ";
    cin.ignore();
    getline(cin, username);
    cout << "Enter Password: ";
    getline(cin, password);
    if (username == "admin" && password == "admin") {
        adminMainMenu();
    }
    Account* account = findAccount(username, password);
    if (account != nullptr) {
        cout << "Login Successful!" << endl;
        userMenu(username, password);
    } else {
        cout << "Incorrect Username or Password!" << endl;
    }
}

void registerAccount() {
    cout << "Enter Username: ";
    string username;
    cin.ignore();
    getline(cin, username);

    if (!isUsernameUnique(username)) {
        cout << "Username already exists. Please choose a different one." << endl;
        return;
    }

    string password;
    cout << "Enter Password: ";
    getline(cin, password);

    string fullName;
    cout << "Enter Full Name: ";
    getline(cin, fullName);

    cout << "Enter Initial Balance: ";
    double balance;
    cin >> balance;

    Account* newAccount = new Account;
    newAccount->username = username;
    newAccount->fullName = fullName;
    newAccount->password = password;
    newAccount->balance = balance;

    unsigned int newCardNumber = generateCardNumber();
    newAccount->cardNumbers[newCardNumber] = true;  

    users[username] = newAccount;

    cout << "\nAccount Registered Successfully!" << endl;

    saveAccountsToFile();
}

int main() {
    loadAccountsFromFile(); 
    mainMenu();
    return 0;
}
