#include <iostream>
using std::cin;
using std::cout;
using std::endl;

#include <iomanip>
using std::setw;
using std::setfill;

#include <fstream>
using std::ifstream;
using std::ofstream;
using std::fstream;
using std::ios;

#include <vector>
using std::vector;

struct Date
{
	int year = 0;
	int month = 0;
	int day = 0;
};

struct AvailTable
{
	Date date;
	int numAvailTables[4][6] = { 2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2 }; // the number of available tables,
};  // numAvailTables[ 1 ][ i ] is the number of available type i tables on 11:30
	// numAvailTables[ 2 ][ i ] is the number of available type i tables on 14:30
	// numAvailTables[ 3 ][ i ] is the number of available type i tables on 17:30

struct Reservation
{
	int numCustomers = 0;              // number of customers
	Date date;                         // reservation date
	int time = 0;                      // reservation time code
	char name[8] = "";               // name
	char mobileNumber[12] = "";      // mobile phone number
	char email[40] = "";             // email address
	char password[12] = "";          // reservation password
	char reservationNumber[12] = ""; // reservation number
};

// tableTypes[ i ] is the table type for i customers
const int tableTypes[13] = { 0, 1, 1, 2, 2, 3, 4, 4, 0, 0, 5, 5, 5 };

const int TOTAL_NUM_TABLES = 2; // suppose there are totally 2 tables for all types

// return current date
Date computeCurrentDate();

// return true if and only if year is a leap year
inline bool leapYear(int year);

// input an integer in the range [ begin, end ]
int inputAnInteger(int begin, int end);

// return true if and only if date1 == date2
bool equal(Date date1, Date date2);

// return true if and only if date1 < date2
bool less(Date date1, Date date2);

// return true if and only if date1 <= date2
bool lessEqual(Date date1, Date date2);

// return date + numDays
Date add(Date date, int numDays);

// delete out of date data in availTables, and append new data from rear of availTables
void initAvailTables(vector< AvailTable > &availTables, Date currentDate);

// read available table informations after currentDate, from the file AvailTables.dat
void loadAvailTables(vector< AvailTable > &availTables, Date currentDate);

// read reservation informations after or equal to currentDate, from the file Reservations.dat
void loadReservations(vector< Reservation > &reservations, vector< AvailTable > &availTables, Date currentDate);

// make reservation
void makeReservation(vector< Reservation > &reservations, vector< AvailTable > &availTables, Date currentDate);

// choose a date and a time
void chooseDateTime(vector< AvailTable > &availTables, Reservation &reservation, Date currentDate, int tableType);

// display dates on which there are tables available
void displayAvailDates(vector< AvailTable > &availTables, bool available[], int tableType);

// return true if there are available tables on availTables[ i ].date for corresponding table type
bool availableTables(vector< AvailTable > &availTables, size_t i, int tableType);

// --availTables[ i ].numAvailTables[ timeCode ][ tableType ], where availTables[ i ].date == date
void decreaseAvailTables(vector< AvailTable > &availTables, Date date, int timeCode, int tableType);

// use reservationNumber to make a reservation inquiry, and let user choose whether to cancel it
void reservationInquiry(vector< Reservation > &reservations, vector< AvailTable > &availTables);

// display all fields of reservation which includes
// mobileNumber, name, date, time, email, numCustomers, password and reservationNumber
void displayReservationInfo(const Reservation &reservation);

// display reservations[ i ], where
// reservations[ i ].reservationNumber is equal to the specified reservationNumber
void displayReservationInfo(const vector< Reservation > &reservations, char reservationNumber[]);

// erase reservations[ position ]
void erase(vector< Reservation > &reservations, int position);

// write availTables into the file AvailTables.dat
void saveAvailTables(const vector< AvailTable > &availTables);

// write reservations into the file Reservations.dat
void saveReservations(const vector< Reservation > &reservations);
void l();
int main()
{
	cout << "Welcome to Hi-Lai Harbour Taoyuan Location!\n";

	srand(static_cast<unsigned int>(time(0)));
	Date currentDate = computeCurrentDate();

	vector< AvailTable > availTables; // vector of all available table informations
	initAvailTables(availTables, currentDate);

	vector< Reservation > reservations; // vector of all available table informations
	loadReservations(reservations, availTables, currentDate);

	bool userExited = false; // user has not chosen to exit

	// loop while user has not chosen option to exit system
	while (!userExited)
	{
		// show main menu and get user selection
		cout << "\n1 - Book a Table\n";
		cout << "2 - My Bookings\n";
		cout << "3 - End\n\n";

		int mainMenuSelection;
		do cout << "Enter your choice: (1~3): ";
		while ((mainMenuSelection = inputAnInteger(1, 3)) == -1);

		// decide how to proceed based on user's menu selection
		switch (mainMenuSelection)
		{
		case 1:
			makeReservation(reservations, availTables, currentDate); // make reservation
			break;
		case 2:
			reservationInquiry(reservations, availTables); // reservation inquiry
			break;
		case 3: // user chose to terminate session
			saveAvailTables(availTables);
			saveReservations(reservations);
			userExited = true; // this session should end
			cout << endl;
			break;
		default: // user did not enter an integer from 1-3
			cout << "\nIncorrect choice!" << endl;
		}
	}

	system("pause");
}

Date computeCurrentDate()
{
	Date now;
	now.year = 1970;
	now.day = 1;
	now.month = 1;
	int m[13] = { 0,31,28,31,30,31,30,31,31,30,31,30,31 };
	int second = static_cast<unsigned int>(time(0)) + 8 * 60 * 60;//格林威治時間+8hr才是台灣的

	while (second > 365 * 24 * 60 * 60) {//年
		int a = 365 * 24 * 60 * 60;
		if (leapYear(now.year)) {
			a += 24 * 60 * 60;
		}
		second -= a;
		now.year++;
	}

	if (leapYear(now.year))
		m[1] = 29;
	while (second > m[now.month] * 24 * 60 * 60) {//月
		second -= m[now.month] * 24 * 60 * 60;
		now.month++;
	}

	while (second > 24 * 60 * 60) {//日
		second -= 24 * 60 * 60;
		now.day++;
	}

	return now;
}

inline bool leapYear(int year)
{
	return (year % 400 == 0 || (year % 4 == 0 && year % 100 != 0));
}

int inputAnInteger(int begin, int end)
{
	char string[80];
	cin.getline(string, 80, '\n');

	if (strlen(string) == 0)
		exit(0);

	for (unsigned int i = 0; i < strlen(string); i++)
		if (string[i] < '0' || string[i] > '9')
			return -1;

	int number = atoi(string);

	if (number >= begin && number <= end)
		return number;
	else
		return -1;
}

bool equal(Date date1, Date date2)
{
	return (date1.year == date2.year &&
		date1.month == date2.month &&
		date1.day == date2.day);
}

bool less(Date date1, Date date2)
{
	if (date1.year < date2.year)
		return true;
	if (date1.year > date2.year)
		return false;

	if (date1.month < date2.month)
		return true;
	if (date1.month > date2.month)
		return false;

	if (date1.day < date2.day)
		return true;

	return false;
}

bool lessEqual(Date date1, Date date2)
{
	return less(date1, date2) || equal(date1, date2);
}

Date add(Date date, int numDays)
{
	int m[13] = { 0,31,28,31,30,31,30,31,31,30,31,30,31 };
	if (leapYear(date.year))
		m[2] = 29;
	date.day += numDays;
	if (date.day > m[date.month]) {
		date.day = date.day - m[date.month];
		date.month++;
		if (date.month > 12) {
			date.month = 1;
			date.year++;
		}
	}
	return date;
}

void initAvailTables(vector< AvailTable > &availTables, Date currentDate)
{
	int numDays = 1;
	fstream file_writer("AvailTables1.dat", ios::out);//清空檔案
	availTables.resize(31);
	while (numDays <= 30) {
		availTables[numDays].date = add(currentDate, numDays);
		numDays++;
	}
	loadAvailTables(availTables, currentDate);
}

void loadAvailTables(vector< AvailTable > &availTables, Date currentDate)
{
	fstream ioFile("AvailTables1.dat", ios::in | ios::out | ios::binary);
	if (!ioFile) {
		cout << "File could not be opened." << endl;
		exit(1);
	}
	int n = 1;
	while (n <= 30) {
		ioFile.seekg((n - 1) * sizeof(AvailTable));
		ioFile.write(reinterpret_cast<char *>(&availTables[n]), sizeof(AvailTable));
		n++;
	}
	ioFile.close();
	//l();
}

/*void l() {
	AvailTable  a;
	fstream ioFile("AvailTables1.dat", ios::in | ios::out | ios::binary);
	if (!ioFile) {
		cout << "File could not be opened." << endl;
		exit(1);
	}
	ioFile.seekg(0);
	while (ioFile && !ioFile.eof()) {
		ioFile.read(reinterpret_cast<char *>(&a), sizeof(AvailTable));
		cout << a.date.day << endl;
	}
}*/

void l() {
	Reservation  a;
	fstream ioFile("Reservations.dat", ios::in | ios::out | ios::binary);
	if (!ioFile) {
		cout << "File could not be opened." << endl;
		exit(1);
	}
	ioFile.seekg(0);
	while (ioFile && !ioFile.eof()) {
		ioFile.read(reinterpret_cast<char *>(&a), sizeof(Reservation));
	}
}
void loadReservations(vector< Reservation > &reservations, vector< AvailTable > &availTables, Date currentDate)
{
	Reservation  a, v;
	fstream ioFile("Reservations.dat", ios::in | ios::out | ios::binary);
	if (!ioFile) {
		cout << "File could not be opened." << endl;
		exit(1);
	}
	ioFile.seekg(0);
	int n = 0;
	while (ioFile && !ioFile.eof()) {
		ioFile.read(reinterpret_cast<char *>(&a), sizeof(Reservation));
		if (a.date.day == 0)
			break;
		int gg = 0;
		for (int j = 0; j < 8; j++)
		{
			if (a.reservationNumber[j] == v.reservationNumber[j])
				gg++;
		}
		if (gg == 8)
			continue;
		int i = 1;
		while (i <= 30) {
			if (availTables[i].date.day == a.date.day) {
				availTables[i].numAvailTables[a.time][tableTypes[a.numCustomers]]--;
				break;
			}
			i++;
		}
		reservations.push_back(a);
		if (less(reservations[n].date, currentDate)) {
			reservations.pop_back();
			continue;
		}
		n++;
		v = a;
	}
	ioFile.close();
}

void makeReservation(vector< Reservation > &reservations, vector< AvailTable > &availTables, Date currentDate)
{
	Reservation newReservation;

	do {
		cout << "\nEnter the number of customers (1 - 7, 10 - 12): ";
		newReservation.numCustomers = inputAnInteger(1, 12);
	} while (newReservation.numCustomers == 8 ||
		newReservation.numCustomers == 9 ||
		newReservation.numCustomers == -1);

	int tableType = tableTypes[newReservation.numCustomers];

	chooseDateTime(availTables, newReservation, currentDate, tableType);

	cout << "\nEnter name: ";
	cin >> newReservation.name;

	cout << "\nEnter mobile number: ";
	cin >> newReservation.mobileNumber;

	cout << "\nEnter email address: ";
	cin >> newReservation.email;

	cout << "\nEnter reservation password: ";
	cin >> newReservation.password;
	cin.ignore();

	newReservation.reservationNumber[0] = 1 + rand() % 9 + '0';
	for (int i = 1; i < 8; i++)
		newReservation.reservationNumber[i] = rand() % 10 + '0';

	cout << setfill(' ') << endl << setw(11) << "Mobile No." << setw(10) << "Name"
		<< setw(14) << "Date" << setw(9) << "Time" << setw(30) << "Email"
		<< setw(19) << "No of Customers" << setw(12) << "Password"
		<< setw(19) << "Reservation No." << endl;

	displayReservationInfo(newReservation);

	cout << "\nReservation Completed!\n";

	reservations.push_back(newReservation);

	decreaseAvailTables(availTables, newReservation.date, newReservation.time, tableType);
}

// choose a date and a time
void chooseDateTime(vector< AvailTable > &availTables, Reservation &reservation, Date currentDate, int tableType)
{
	bool *availableDate[1];
	int a, b = 0, c = 11, g;
	cout << "\nChoose a date " << endl;
	displayAvailDates(availTables, availableDate[0], tableType);
	cin >> a;
	reservation.date = availTables[a].date;
	cout << "\nChoose a time " << endl;
	while (b < 3) {
		b++;
		if (availTables[a].numAvailTables[b][tableType] > 0) {
			cout << b << ". " << c + 3 * (b - 1) << ":" << "30" << endl;
		}
	}
	cin >> g;
	reservation.time = g;
	//add(Date date, int numDays);
}

void displayAvailDates(vector< AvailTable > &availTables, bool availableDate[], int tableType)
{
	size_t n = 1;
	int m = 0;
	while (n <= 30) {
		if (availableTables(availTables, n, tableType)) {
			cout << n << ".\t" << availTables[n].date.year << "/"
				<< availTables[n].date.month << "/"
				<< availTables[n].date.day << "\t";
			m++;
			if (m % 4 == 0)
				cout << endl;
		}
		n++;
	}
	cout << endl;
}

// returns true if there are available tables on availTables[ i ].date for corresponding table type
bool availableTables(vector< AvailTable > &availTables, size_t i, int tableType)
{
	int j = 1;
	while (j <= 3) {
		if (availTables[i].numAvailTables[j][tableType] > 0)
			return 1;
		j++;
	}
	return 0;
}

// --availTables[ i ].numAvailTables[ timeCode ][ tableType ], where availTables[ i ].date == date
void decreaseAvailTables(vector< AvailTable > &availTables, Date date, int timeCode, int tableType)
{
	int i = 1;
	while (i <= 30) {
		if (availTables[i].date.day == date.day) {
			--availTables[i].numAvailTables[timeCode][tableType];
			break;
		}
		i++;
	}
}

void reservationInquiry(vector< Reservation > &reservations, vector< AvailTable > &availTables)
{
	if (reservations.size() == 0)
	{
		cout << "\nNo reservations!\n";
		return;
	}

	cout << "\nEnter reservation number: ";
	char reservationNumber[12];
	cin.getline(reservationNumber, 12);
	int n = 0, c = 0;
	while (n <= reservations.size()) {

		if (n >= reservations.size()) {
			cout << "\nNo reservations with this reservation number!" << endl;
			return;
		}
		int a = 0;
		for (int i = 0; i < 8; i++) {
			if (reservations[n].reservationNumber[i] == reservationNumber[i])
				a++;
		}
		if (a == 8)
			break;
		n++;
	}
	cout << "\nEnter reservation password: ";
	char password[12] = "";
	cin.getline(password, 12);
	for (int i = 0; i < strlen(password); i++) {
		if (reservations[n].password[i] != password[i]) {
			cout << "\nNo reservations with this reservation number!" << endl;
			return;
		}
	}
	cout << "\n" << setfill(' ') << setw(11) << "Mobile No." << setw(10) << "Name"
		<< setw(14) << "Date" << setw(9) << "Time" << setw(30) << "Email"
		<< setw(19) << "No of Customers" << setw(12) << "Password"
		<< setw(19) << "Reservation No." << endl;
	displayReservationInfo(reservations[n]);
	cout << "\nCancel this reservation ( y/n )?";
	char m;
	cin >> m;
	cin.ignore();
	if (m == 'y') {
		for (int i = 0; i < availTables.size(); i++) {
			if (availTables[i].date.day == reservations[n].date.day) {
				availTables[i].numAvailTables[reservations[n].time][tableTypes[reservations[n].numCustomers]]++;
			}
		}
		erase(reservations, n);
		cout << "\nThis reservation has been cancelled.\n";
	}
	else {
		return;
	}
}

// displays all fields of reservation which includes
// mobileNumber, name, date, time, email, numCustomers, password and reservationNumber
void displayReservationInfo(const Reservation &reservation)
{
	char times[4][8] = { "", "11:30", "14:30", "17:30" };
	cout << setfill(' ') << setw(11) << reservation.mobileNumber
		<< setw(10) << reservation.name
		<< "    " << reservation.date.year << "/" << setfill('0')
		<< setw(2) << reservation.date.month << '/'
		<< setw(2) << reservation.date.day
		<< setfill(' ') << setw(9) << times[reservation.time]
		<< setw(30) << reservation.email
		<< setw(19) << reservation.numCustomers
		<< setw(12) << reservation.password
		<< setw(19) << reservation.reservationNumber << endl;
}

// displays reservations[ i ], where
// reservations[ i ].reservationNumber is equal to the specified reservationNumber
void displayReservationInfo(const vector< Reservation > &reservations, char reservationNumber[])
{
	cout << setfill(' ') << setw(11) << "Mobile No." << setw(10) << "Name"
		<< setw(14) << "Date" << setw(9) << "Time" << setw(30) << "Email"
		<< setw(19) << "No of Customers" << setw(12) << "Password"
		<< setw(19) << "Reservation No." << endl;

	for (size_t i = 0; i < reservations.size(); ++i)
		if (strcmp(reservations[i].reservationNumber, reservationNumber) == 0)
			break;

	char times[4][8] = { "", "11:30", "14:30", "17:30" };
	int count = 0;
	for (size_t i = 0; i < reservations.size(); ++i)
		if (strcmp(reservations[i].reservationNumber, reservationNumber) == 0)
			displayReservationInfo(reservations[i]);
}

// erase reservations[ position ]
void erase(vector< Reservation > &reservations, int position)
{
	for (int i = position; i < reservations.size() - 1; i++) {
		reservations[i] = reservations[i + 1];
	}
	reservations.resize(reservations.size() - 1);
}

void saveAvailTables(const vector< AvailTable > &availTables)
{
	fstream ioFile("AvailTables1.dat", ios::in | ios::out | ios::binary);
	if (!ioFile) {
		cout << "File could not be opened." << endl;
		exit(1);
	}
	int n = 1;
	while (n * sizeof(AvailTable) < availTables.size()) {
		ioFile.seekg((n - 1) * sizeof(AvailTable));
		ioFile.write(reinterpret_cast<const char *>(&availTables), sizeof(AvailTable));
	}
	ioFile.close();
}

void saveReservations(const vector< Reservation > &reservations)
{
	fstream ioFile("Reservations.dat", ios::in | ios::out | ios::binary);
	if (!ioFile) {
		cout << "File could not be opened." << endl;
		exit(1);
	}
	int n = 0;
	while (n < reservations.size()) {
		ioFile.seekg(n * sizeof(Reservation));
		ioFile.write(reinterpret_cast<const char *>(&reservations[n]), sizeof(Reservation));
		n++;
	}
	ioFile.close();
	//l();
}

/*void l() {
	Reservation  a;
	fstream ioFile("Reservations.dat", ios::in | ios::out | ios::binary);
	if (!ioFile) {
		cout << "File could not be opened." << endl;
		exit(1);
	}
	ioFile.seekg(0);
	while (ioFile && !ioFile.eof()) {
		ioFile.read(reinterpret_cast<char *>(&a), sizeof(Reservation));
	}
}*/
