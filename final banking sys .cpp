#include <iostream>
#include <vector>
#include <iomanip>
#include <algorithm>
using namespace std;

class Transaction {
private:
    string type;
    double amount;
    string date;
    string description;

public:
    Transaction(string t, double a, string d = "")
        : type(t), amount(a), description(d) {
        // Simple fixed date format (all July)
        date = "16-07-2024";
    }

    void display() const {
        cout << left << setw(15) << type
            << setw(10) << amount
            << setw(15) << date
            << description << endl;
    }
};

class Account {
private:
    string accountNumber;
    double balance;
    vector<Transaction> transactions;

public:
    Account(string accNum, double initialBalance = 0.0)
        : accountNumber(accNum), balance(initialBalance) {
    }

    void deposit(double amount, string desc = "") {
        if (amount > 0) {
            balance += amount;
            transactions.emplace_back("DEPOSIT", amount, desc);
            cout << "Deposit successful. New balance: " << balance << endl;
        }
        else {
            cout << "Invalid deposit amount." << endl;
        }
    }

    bool withdraw(double amount, string desc = "") {
        if (amount > 0 && balance >= amount) {
            balance -= amount;
            transactions.emplace_back("WITHDRAWAL", amount, desc);
            cout << "Withdrawal successful. New balance: " << balance << endl;
            return true;
        }
        else {
            cout << "Insufficient funds or invalid amount." << endl;
            return false;
        }
    }

    void transfer(Account& toAccount, double amount, string desc = "") {
        if (withdraw(amount, "Transfer to " + toAccount.accountNumber)) {
            toAccount.deposit(amount, "Transfer from " + accountNumber);
            transactions.emplace_back("TRANSFER OUT", amount, desc);
            toAccount.transactions.emplace_back("TRANSFER IN", amount, desc);
            cout << "Transfer successful." << endl;
        }
        else {
            cout << "Transfer failed." << endl;
        }
    }

    void displayTransactions(int limit = 5) const {
        cout << "\nTransaction History for Account: " << accountNumber << endl;
        cout << left << setw(15) << "Type" << setw(10) << "Amount"
            << setw(15) << "Date" << "Description" << endl;
        cout << string(50, '-') << endl;

        int start = max(0, static_cast<int>(transactions.size()) - limit);
        for (int i = start; i < transactions.size(); i++) {
            transactions[i].display();
        }
        cout << "\nCurrent Balance: " << balance << endl;
    }

    string getAccountNumber() const { return accountNumber; }
    double getBalance() const { return balance; }
};

class Customer {
private:
    string name;
    string customerId;
    vector<Account> accounts;

public:
    Customer(string n, string id) : name(n), customerId(id) {}

    void createAccount(string accNum, double initialBalance = 0.0) {
        accounts.emplace_back(accNum, initialBalance);
        cout << "Account created successfully. Account Number: " << accNum << endl;
    }

    void displayAccounts() const {
        cout << "\nAccounts for Customer: " << name << " (ID: " << customerId << ")" << endl;
        cout << left << setw(20) << "Account Number" << setw(15) << "Balance" << endl;
        cout << string(35, '-') << endl;

        for (const auto& account : accounts) {
            cout << setw(20) << account.getAccountNumber()
                << setw(15) << account.getBalance() << endl;
        }
    }

    Account* getAccount(string accNum) {
        for (auto& account : accounts) {
            if (account.getAccountNumber() == accNum) {
                return &account;
            }
        }
        return nullptr;
    }

    string getName() const { return name; }
    string getCustomerId() const { return customerId; }
};

int main() {
    cout << "*** Banking System ***\n" << endl;

    // Create customers
    Customer customer1("Mr Chips", "C1001");
    Customer customer2("Katherine", "C1002");

    // Create accounts
    customer1.createAccount("A10001", 1000.0);
    customer1.createAccount("A10002", 500.0);
    customer2.createAccount("A20001", 2000.0);

    // Perform transactions
    cout << "\nPerforming transactions..." << endl;
    customer1.getAccount("A10001")->deposit(500.0, "Salary");
    customer1.getAccount("A10001")->withdraw(200.0, "Rent");
    customer2.getAccount("A20001")->deposit(300.0, "Bonus");

    // Transfer between accounts
    cout << "\nTransferring funds..." << endl;
    customer1.getAccount("A10001")->transfer(*customer2.getAccount("A20001"), 150.0, "Loan repayment");

    // Display account information
    cout << "\nDisplaying account information..." << endl;
    customer1.displayAccounts();
    customer2.displayAccounts();

    // Display transaction history
    cout << "\nDisplaying transaction history..." << endl;
    customer1.getAccount("A10001")->displayTransactions();
    customer2.getAccount("A20001")->displayTransactions();

    return 0;
}