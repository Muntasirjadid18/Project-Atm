#include <bits/stdc++.h>
#include <conio.h>
using namespace std;
class Transaction {
private:
    time_t timestamp;
    string type;
    int amount;
public:
    Transaction(time_t timestamp, string type, int amount) : timestamp(timestamp), type(type), amount(amount) {}
void display() const {
        cout << setw(20) << left << ctime(&timestamp);
        cout << setw(15) << left << type;
        cout << amount << endl;
    }
};
class ATM {
private:
    int deposit_money = 0;
    int withdraw_money = 0;
    int amount_of_balance = 0;
    vector<Transaction> transaction_history;
friend void maskPassword(string& password);
public:
    void deposit(int amount) {
        deposit_money = amount;
        amount_of_balance += deposit_money;
        transaction_history.insert(transaction_history.begin(), Transaction(time(nullptr), "Deposit", deposit_money));
        if (transaction_history.size() > 10) {
            transaction_history.pop_back();
        }
        cout << "-------------Deposit Complete----------------" << endl << endl;
    }

    void withdraw(int amount2) {
        withdraw_money = amount2;
        if (amount_of_balance < withdraw_money) {
            cout << "--------Insufficient Balance---------" << endl << endl;
        } else {
            amount_of_balance -= withdraw_money;
            transaction_history.insert(transaction_history.begin(), Transaction(time(nullptr), "Withdrawal", -withdraw_money));
            if (transaction_history.size() > 10) {
                transaction_history.pop_back();
            }
            cout << "-------------Withdraw complete---------------" << endl << endl;
        }
    }

    void show_balance() const {
        cout << "Your current balance is: " << amount_of_balance << endl << endl;
    }

    void display_transaction_history() const {
        cout << "Transaction History:" << endl;
        cout << setw(20) << left << "Timestamp";
        cout << setw(15) << left << "Type";
        cout << "Amount" << endl;
        cout << string(40, '-') << endl;
        for (const auto& transaction : transaction_history) {
            transaction.display();
        }
        cout << endl;
    }

    void changePIN(string& currentPIN, string& newPIN, const string& userPassword);
};

class User {
private:
    int user_id;
    string password;

public:
    void set_id(int id) {
        user_id = id;
    }

    int get_id() const {
        return user_id;
    }

    void set_password(string pass) {
        password = pass;
    }

    string get_password() const {
        return password;
    }
};

void maskPassword(string& password) {
    password = "";
    char ch;
    while ((ch = _getch()) != '\r') {
        if (ch == '\b') {
            if (!password.empty()) {
                cout << "\b \b";
                password.pop_back();
            }
        } else {
            password.push_back(ch);
            cout << "*";
        }
    }
    cout << endl; // Add a newline after password input
}

void ATM::changePIN(string& currentPIN, string& newPIN, const string& userPassword) {
    string confirmPIN;
    cout << "Enter current PIN: ";
    maskPassword(currentPIN);

    // Check if the current PIN is correct
    if (currentPIN != userPassword) {
        cout << "Incorrect PIN. Returning to main menu." << endl;
        return;
    }

    cout << "Enter new PIN: ";
    maskPassword(newPIN);
    cout << "Confirm new PIN: ";
    maskPassword(confirmPIN);

    while (newPIN != confirmPIN) {
        cout << "PINs do not match. Please try again." << endl;
        cout << "Enter new PIN: ";
        maskPassword(newPIN);
        cout << "Confirm new PIN: ";
        maskPassword(confirmPIN);
    }
    cout << "PIN changed successfully!" << endl;
}

int main() {
    int option, amount, inp_id, number_of_users;

    cout << "Enter number of users: ";
    cin >> number_of_users;
    cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Clear the input buffer

    User users[number_of_users];
    ofstream file("user.txt");

    if (!file.is_open()) {
        cerr << "Error opening file!" << endl;
        return 1;
    }

    for (int i = 0; i < number_of_users; i++) {
        cout << "ID for user " << i + 1 << ": ";
        int inp_id;
        cin >> inp_id;
        users[i].set_id(inp_id);

        string pass, confirmPass;
        do {
            cout << "Password for user " << i + 1 << ": ";
            maskPassword(pass); // Mask password input
            cout << "Confirm Password: ";
            maskPassword(confirmPass);
            if (pass != confirmPass) {
                cout << "Passwords do not match. Please try again." << endl;
            }
        } while (pass != confirmPass);

        users[i].set_password(pass);

        file << inp_id << endl;
        file << pass << endl;
    }
    file.close();

    system("cls");

    cout << "-----------------------------------------------------" << endl;
    cout << "                $$  Welcome To ATM Machine  $$           " << endl;
    cout << "-----------------------------------------------------" << endl;
    cout << endl << endl;

    bool authenticated = false;
    while (!authenticated) {
        cout << "Enter Your User Id: ";
        cin >> inp_id;
        cout << "Enter Your Password: ";

        // Masking password with asterisks
        string pass;
        maskPassword(pass); // Mask password input
        cout << endl;

        for (int i = 0; i < number_of_users; i++) {
            if (inp_id == users[i].get_id() && pass == users[i].get_password()) {
                authenticated = true;
                break;
            }
        }

        if (!authenticated) {
            cout << "-----------Incorrect User Id or Password----------" << endl << endl;
            while (true) {
                cout << "---------------Press 0 to continue----------------" << endl;
                int key;
                cin >> key;
                cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Clear input buffer
                if (key == 0) {
                    system("cls");
                    break; // Break out of the loop if the user enters 0
                } else {
                    cout << "----------Invalid Number-----------" << endl;
                }
            }
        }
    }

    ATM atm;

    while (true) {
        cout << "Menu" << endl;
        cout << "----------------------" << endl;
        cout << "1. Cash Deposit" << endl;
        cout << "2. Cash Withdraw" << endl;
        cout << "3. Check Balance" << endl;
        cout << "4. Change PIN" << endl;
        cout << "5. Transaction History" << endl;
        cout << "6. Exit" << endl << endl;
        cout << "Choose your option: ";
        cin >> option;

        switch (option) {
            case 1:
                cout << "Enter the amount: ";
                cin >> amount;
                atm.deposit(amount);
                break;
            case 2:
                cout << "Enter the amount: ";
                cin >> amount;
                atm.withdraw(amount);
                break;
            case 3:
                atm.show_balance();
                break;
            case 4: {
                string currentPIN, newPIN;
                atm.changePIN(currentPIN, newPIN, users[inp_id - 1].get_password());
                break;
            }
            case 5:
                atm.display_transaction_history();
                break;
            case 6:
                cout << "Exiting program..." << endl;
                return 0;
            default:
                cout << "Invalid option! Please choose a valid option." << endl;
        }
    }

    return 0;
}
