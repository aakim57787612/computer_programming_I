#include <iostream>
#include <fstream>
using namespace std;

struct Account
{
	int accountNumber;    // account number
	int pin;              // personal identification number
	int availableBalance; // funds available for withdrawal
	int totalBalance;     // funds available + funds waiting to clear
};

// retrieve Account object (from Accounts.dat) containing specified accountNumber and pin,
// put this object to currentAccount,
// and put the record number (in Accounts.dat) of this object into recordNumber.
// if this object exists, returns true; otherwise returns false.
bool loadAccount(int accountNumber, int pin, Account &currentAccount, int &recordNumber);

// display the main menu and perform transactions
void performTransactions(Account &currentAccount, int &recordNumber, int &remainingBills);

// perform withdrawal transaction
void withdrawal(Account &currentAccount, int &remainingBills);

// perform deposit transaction
void deposit(Account &currentAccount);

// write currentAccount into the n-th record of Accounts.dat
void saveAccount(Account currentAccount, int n);

const int INITIAL_BILLS = 5; // the number bills loaded each day

int main()
{
	// the number of remaining bills in the cash dispenser
	int remainingBills = INITIAL_BILLS;
	int recordNumber;
	Account currentAccount;

	// welcome and authenticate user; perform transactions
	while (true)
	{
		int accountNumber;
		int pin;

		do {
			cout << "Welcome!\n";
			cout << "\nPlease enter your account number: ";
			cin >> accountNumber;
			cout << "\nEnter your PIN: ";
			cin >> pin; // input PIN
		} while (!loadAccount(accountNumber, pin, currentAccount, recordNumber));

		performTransactions(currentAccount, recordNumber, remainingBills);
		cout << "\nThank you! Goodbye!\n\n";
	}
} // end function main

// retrieve Account object (from Accounts.dat) containing specified accountNumber and pin,
// put this object to currentAccount,
// and put the record number of this object into recordNumber.
// if this object exists, returns true; otherwise returns false.
bool loadAccount(int accountNumber, int pin, Account &currentAccount, int &recordNumber)
{
	recordNumber = -1;
	fstream ioFile("Accounts.dat", ios::in | ios::out | ios::binary);
	if (!ioFile) {
		cerr << "File could not be opened." << endl;
		exit(1);
	}
	while (ioFile && !ioFile.eof()) {
		recordNumber++;
		ioFile.read(reinterpret_cast<char *>(&currentAccount), sizeof(Account));
		if (currentAccount.accountNumber == accountNumber && currentAccount.pin == pin) {
			ioFile.close();
			return true;
		}
	}
	cout << "\nInvalid account number or PIN. Please try again.\n";
	ioFile.close();
	return false;
} // end function loadAccount

// display the main menu and perform transactions
void performTransactions(Account &currentAccount, int &recordNumber, int &remainingBills)
{
	int c;
	cout << "\nMain menu :\n"
		<< "1 - View my balance\n"
		<< "2 - Withdraw cash\n"
		<< "3 - Deposit funds\n"
		<< "4 - Exit\n"
		<< "\nEnter a choice :";
	cin >> c;
	switch (c) {
	case 1:
		cout << "\nBalance Information :\n"
			<< "-Available balance : $" << currentAccount.availableBalance
			<< "\n- Total balance : $" << currentAccount.totalBalance << endl;
		performTransactions(currentAccount, recordNumber, remainingBills);
		break;
	case 2:
		withdrawal(currentAccount, remainingBills);
		performTransactions(currentAccount, recordNumber, remainingBills);
		break;
	case 3:
		deposit(currentAccount);
		performTransactions(currentAccount, recordNumber, remainingBills);
		break;
	case 4:
		cout << "\nExiting the system...\n";
		saveAccount(currentAccount, recordNumber);
		return;
	default:
		cout << "\nYou did not enter a valid selection. Try again.\n";
		performTransactions(currentAccount, recordNumber, remainingBills);
		break;
	}

} // end function performTransactions

// perform withdrawal transaction
void withdrawal(Account &currentAccount, int &remainingBills)
{
	int c;
	cout << "\nWithdrawal options:\n"
		<< "1 - $20\n"
		<< "2 - $40\n"
		<< "3 - $60\n"
		<< "4 - $100\n"
		<< "5 - $200\n"
		<< "6 - Cancel transaction\n"
		<< "\nEnter a choice :";
	cin >> c;
	switch (c) {
	case 1:
		if (currentAccount.availableBalance - 20 < 0) {
			cout << "\nInsufficient funds in your account.\n"
				<< "\nPlease choose a smaller amount.\n";
			withdrawal(currentAccount, remainingBills);
			break;
		}
		else if (remainingBills - 1 < 0) {
			cout << "\nInsufficient cash available in the ATM.\n"
				<< "\nPlease choose a smaller amount.\n";
			withdrawal(currentAccount, remainingBills);
			break;
		}
		currentAccount.availableBalance = currentAccount.availableBalance - 20;
		currentAccount.totalBalance = currentAccount.totalBalance - 20;
		break;
	case 2:
		if (currentAccount.availableBalance - 40 < 0) {
			cout << "\nInsufficient funds in your account.\n"
				<< "\nPlease choose a smaller amount.\n";
			withdrawal(currentAccount, remainingBills);
			break;
		}
		else if (remainingBills - 2 < 0) {
			cout << "\nInsufficient cash available in the ATM.\n"
				<< "\nPlease choose a smaller amount.\n";
			withdrawal(currentAccount, remainingBills);
			break;
		}
		currentAccount.availableBalance = currentAccount.availableBalance - 40;
		currentAccount.totalBalance = currentAccount.totalBalance - 40;
		break;
	case 3:
		if (currentAccount.availableBalance - 60 < 0) {
			cout << "\nInsufficient funds in your account.\n"
				<< "\nPlease choose a smaller amount.\n";
			withdrawal(currentAccount, remainingBills);
			break;
		}
		else if (remainingBills - 3 < 0) {
			cout << "\nInsufficient cash available in the ATM.\n"
				<< "\nPlease choose a smaller amount.\n";
			withdrawal(currentAccount, remainingBills);
			break;
		}
		currentAccount.availableBalance = currentAccount.availableBalance - 60;
		currentAccount.totalBalance = currentAccount.totalBalance - 60;
		break;
	case 4:
		if (currentAccount.availableBalance - 100 < 0) {
			cout << "\nInsufficient funds in your account.\n"
				<< "\nPlease choose a smaller amount.\n";
			withdrawal(currentAccount, remainingBills);
			break;
		}
		else if (remainingBills - 5 < 0) {
			cout << "\nInsufficient cash available in the ATM.\n"
				<< "\nPlease choose a smaller amount.\n";
			withdrawal(currentAccount, remainingBills);
			break;
		}
		currentAccount.availableBalance = currentAccount.availableBalance - 100;
		currentAccount.totalBalance = currentAccount.totalBalance - 100;
		break;
	case 5:
		if (currentAccount.availableBalance - 200 < 0) {
			cout << "\nInsufficient funds in your account.\n"
				<< "\nPlease choose a smaller amount.\n";
			withdrawal(currentAccount, remainingBills);
			break;
		}
		else if (remainingBills - 10 < 0) {
			cout << "\nInsufficient cash available in the ATM.\n"
				<< "\nPlease choose a smaller amount.\n";
			withdrawal(currentAccount, remainingBills);
			break;
		}
		currentAccount.availableBalance = currentAccount.availableBalance - 200;
		currentAccount.totalBalance = currentAccount.totalBalance - 200;
		break;
	case 6:
		cout << "\nCanceling transaction...\n";
		break;
	default:
		cout << "\nInvalid selection. Try again.\n";
		withdrawal(currentAccount, remainingBills);
		break;
	}

} // end function withdrawal

// perform deposit transaction
void deposit(Account &currentAccount)
{
	cout << "\nPlease enter a deposit amount in dollars (or 0 to cancel): ";
	int amount;
	cin >> amount; // receive input of deposit amount

	// check whether user entered a deposit amount or canceled
	if (amount == 6)
		cout << "\nCanceling transaction...\n";
	else // user canceled instead of entering amount
	{
		// request deposit envelope containing specified amount
		cout << "\nPlease insert a deposit envelope containing ";
		cout << "$" << amount;
		cout << " in the deposit slot.\n";

		cout << "\nYour envelope has been received."
			<< "\nNOTE: The money deposited will not be available until we"
			<< "\nverify the amount of any enclosed cash, and any enclosed checks clear.\n";

		// credit account to reflect the deposit
		currentAccount.totalBalance += amount; // add to total balance
	} // end if 
} // end function deposit

void saveAccount(Account currentAccount, int n)
{
	fstream ioFile("Accounts.dat", ios::in | ios::out | ios::binary);
	if (!ioFile) {
		cerr << "File could not be opened." << endl;
		exit(1);
	}
	ioFile.seekg(n * sizeof(Account));
	ioFile.write(reinterpret_cast<char *>(&currentAccount), sizeof(Account));
	ioFile.close();
}