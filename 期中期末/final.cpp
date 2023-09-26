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
using std::ios;
//#include <ctime>

#include <vector>
using std::vector;

struct Date
{
    int year = 0;
    int month = 0;
    int day = 0;
};

struct AvailRoom
{
    Date date;
    int numAvailRooms[ 6 ] = {}; // number of available rooms
    // numAvailRooms[ 0 ]: not used
    // numAvailRooms[ 1 ]: the number of available superior rooms
    // numAvailRooms[ 2 ]: the number of available deluxe rooms
    // numAvailRooms[ 3 ]: the number of available deluxe twin rooms
    // numAvailRooms[ 4 ]: the number of available superior suites
    // numAvailRooms[ 5 ]: the number of available deluxe suites
};

struct Reservation
{
    char id[ 12 ] = "";
    char name[ 12 ] = "";
    char mobile[ 12 ] = "";
    int roomType = 0; // 1: superior rooms, 2: deluxe rooms, 3: deluxe twin rooms,
                      // 4: superior suites, 5: deluxe suites
    int numRooms = 0; // number of rooms reserved
    Date checkInDate;
    Date checkOutDate;
};

const int roomRate[ 6 ] = { 0, 5390, 6270, 6270, 6820, 8470 }; // room rate per night for each room type
const int totalNumRooms[ 6 ] = { 0, 9, 5, 3, 5, 4 }; // total number of rooms for each type
const char roomTypeName[ 6 ][ 20 ] = { "", "Superior Room", "Deluxe Room", "Deluxe Twin Room",
                                           "Superior Suite", "Deluxe Suite" };
// return the current date
Date computeCurrentDate();

// return true if and only if year is a leap year
inline bool leapYear( int year );

// return true if and only if date1 == date2
bool equal( Date date1, Date date2 );

// return true if and only if date1 < date2
bool less( const Date &date1, const Date &date2 );

// return true if and only if date1 <= date2
bool lessEqual( Date date1, Date date2 );

// read reservations information whose checkInDate are after or equal to currentDate,
// from the file Reservations.dat
void loadReservations( vector< Reservation > &reservations, const Date &currentDate );

// load all available rooms information after or equal to currentDate,
// from the file Available Rooms.dat
void loadAvailRooms( vector< AvailRoom > &availRooms, const Date &currentDate );

// create available rooms information for 6 months, starting from currentDate
// provided that availRooms.size() == 0
void initAvailRooms( vector< AvailRoom > &availRooms, const Date &currentDate );

// append new available rooms information, provided that availRooms.size() != 0
void completeAvailRooms( vector< AvailRoom > &availRooms, const Date &currentDate );

// input an integer in the range [ begin, end ]
int inputAnInteger( int begin, int end );

void makeReservation( vector< AvailRoom > &availRooms, vector< Reservation > &reservations, const Date &currentDate );

// input check in date and check out date by user, and
// put them into reservation.checkInDate and reservation.checkOutDate, respectively.
void inputDates( const Date &currentDate, Reservation &newReservation );

// return the minimum between availRooms[ first .. last ].numAvailRooms[ roomType ],
// where availRooms[ first ].date == reservation.checkInDate and
// availRooms[ last ].date == reservation.checkOutDate.
int compMinNumRooms( const Reservation &reservation, vector< AvailRoom > &availRooms, int roomType );

void displayAvailRooms( const AvailRoom &availableRoom );

void displayReservations( const Reservation &reservation );

// return the number of nights for reservation
int numNights( Reservation reservation );

void reservationInquiry( const vector< Reservation > &reservations, const Date &currentDate );

bool exist( const vector< Reservation > &reservations, const Date &currentDate, char id[] );

// save all available rooms information into the file Available Rooms.dat
void saveAvailRooms( const vector< AvailRoom > &availRooms );

// save all reservations information into the file Reservations.dat
void saveReservations( const vector< Reservation > &reservations );

int main()
{
    cout << "Evergreen Laurel Hotel Online Reservation System\n";

    srand( static_cast< unsigned int >( time( 0 ) ) );
    Date currentDate = computeCurrentDate();

    vector< Reservation > reservations;
    loadReservations( reservations, currentDate ); // load all reservation informations

    vector< AvailRoom > availRooms;
    loadAvailRooms( availRooms, currentDate ); // load all available rooms information

 //   for( size_t i = 0; i < availRooms.size(); i++ )
 //      displayAvailRooms( availRooms[ i ] );

    if( availRooms.size() == 0 )
        initAvailRooms( availRooms, currentDate ); // create available rooms information for 6 months
    else
        completeAvailRooms( availRooms, currentDate ); // append new available rooms information

    bool userExited = false; // user has not chosen to exit

    // loop while user has not chosen option to exit system
    while( !userExited )
    {
        // show main menu and get user selection
        cout << "\n1 - Making Reservation\n";
        cout << "2 - Reservation Inquiry\n";
        cout << "3 - End\n\n";

        int mainMenuSelection;
        do cout << "Enter your choice: (1~3): ";
        while( ( mainMenuSelection = inputAnInteger( 1, 3 ) ) == -1 );

        Date temp;
        // decide how to proceed based on user's menu selection
        switch( mainMenuSelection )
        {
        case 1:
            makeReservation( availRooms, reservations, currentDate ); // make reservation
            break;

        case 2:
            reservationInquiry( reservations, currentDate ); // reservation inquiry
            break;

        case 3: // user chose to terminate session
            saveAvailRooms( availRooms );
            saveReservations( reservations );
            userExited = true; // this session should end
            cout << endl;
            break;

        default: // user did not enter an integer from 1-3
            cout << "\nIncorrect choice!" << endl;
        }
    }

    system( "pause" );
}

Date computeCurrentDate()
{
    tm structuredTime;
    time_t rawTime = time( 0 );
    localtime_s( &structuredTime, &rawTime );

    Date currentDate;
    currentDate.year = structuredTime.tm_year + 1900;
    currentDate.month = structuredTime.tm_mon + 1;
    currentDate.day = structuredTime.tm_mday;

    return currentDate;
}

inline bool leapYear( int year )
{
    return ( year % 400 == 0 || ( year % 4 == 0 && year % 100 != 0 ) );
}

bool equal( Date date1, Date date2 )
{
    return ( date1.year == date2.year &&
        date1.month == date2.month &&
        date1.day == date2.day );
}

bool less( const Date &date1, const Date &date2 )
{
    if( date1.year < date2.year )
        return true;
    if( date1.year > date2.year )
        return false;

    if( date1.month < date2.month )
        return true;
    if( date1.month > date2.month )
        return false;

    if( date1.day < date2.day )
        return true;
    else
        return false;
}

bool lessEqual( Date date1, Date date2 )
{
    return less( date1, date2 ) || equal( date1, date2 );
}

void loadReservations( vector< Reservation > &reservations, const Date &currentDate )
{
    ifstream inFile( "Reservations.dat", ios::in | ios::binary );
    if( !inFile ) {
        cout << "File could not be open" << endl;
        exit( 1 );
    }
    Reservation a,v;
    while( !inFile.eof() ) {
        inFile.read( reinterpret_cast< char * >( &a ), sizeof( Reservation ) );
        if( !strcmp(v.id,a.id) )
            break;
        v = a;
        if( lessEqual( currentDate, a.checkInDate ) ) {
            reservations.push_back( a );
        }
    }
    inFile.close();
}

void loadAvailRooms( vector< AvailRoom > &availRooms, const Date &currentDate )
{
    ifstream inFile( "Available Rooms blank.dat", ios::in | ios::binary );
    if( !inFile ) {
        cout << "File could not be open" << endl;
        exit( 1 );
    }
    AvailRoom a, v;
    while( !inFile.eof() ) {
        inFile.read( reinterpret_cast< char * >( &a ), sizeof( AvailRoom ) );
        if( v.date.day == a.date.day ) {
            break;
        }
        if( lessEqual( currentDate, a.date ) ) {
            availRooms.push_back( a );
            v = a;
        }
    }
    inFile.close();
}

void initAvailRooms( vector< AvailRoom > &availRooms, const Date &currentDate )
{
    // number of days in each month
    int days[ 13 ] = { 0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
    AvailRoom a;
    Date b;
    for( int i = 1; i < 6; i++ ) {
        a.numAvailRooms[ i ] = totalNumRooms[ i ];
    }
    int num = 0;
    num = days[ currentDate.month ] - currentDate.day;
    b = currentDate;
    for( int i = 1; i < 6; i++ ) {
        b.month++;
        if( b.month > 12 ) {
            b.year++;
            b.month = 1;
            b.day = 1;
        }
        num += days[ b.month ];
    }
    a.date = currentDate;
    while( num >= 0 ) {
        availRooms.push_back( a );
        a.date.day++;
        if( a.date.day > days[ a.date.month ] ) {
            a.date.month++;
            a.date.day = 1;
            if( a.date.month > 12 ) {
                a.date.year++;
            }
        }
        num--;
    }
}

void completeAvailRooms( vector< AvailRoom > &availRooms, const Date &currentDate )
{
    // number of days in each month
    int days[ 13 ] = { 0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
    AvailRoom a;
    Date b;
    for( int i = 1; i < 6; i++ ) {
        a.numAvailRooms[ i ] = totalNumRooms[ i ];
    }
    int num = 0;
    num = days[ currentDate.month ] - currentDate.day;
    b = currentDate;
    for( int i = 1; i < 6; i++ ) {
        b.month++;
        if( b.month > 12 ) {
            b.year++;
            b.month = 1;
            b.day = 1;
        }
        num += days[ b.month ];
    }
    a.date = currentDate;
    int y = 0;
    while( num >= 0 ) {
        if(y >= availRooms.size() ) {
            availRooms.push_back( a );
        }
        a.date.day++;
        if( a.date.day > days[ a.date.month ] ) {
            a.date.month++;
            a.date.day = 1;
            if( a.date.month > 12 ) {
                a.date.year++;
            }
        }
        num--;
        y++;
    }
}

int inputAnInteger( int begin, int end )
{
    char string[ 80 ];
    cin.getline( string, 80, '\n' );

    if( strlen( string ) == 0 )
        exit( 0 );

    for( unsigned int i = 0; i < strlen( string ); i++ )
        if( string[ i ] < '0' || string[ i ] > '9' )
            return -1;

    int number = atoi( string );

    if( number >= begin && number <= end )
        return number;
    else
        return -1;
}

void makeReservation( vector< AvailRoom > &availRooms, vector< Reservation > &reservations, const Date &currentDate )
{
    Reservation newReservation;

    // input check in date and check out date by user, and
    // put them into reservation.checkInDate and reservation.checkOutDate, respectively.
    inputDates( currentDate, newReservation );

    cout << "\nCheck in date: " << newReservation.checkInDate.year << '-'
        << setw( 2 ) << setfill( '0' ) << newReservation.checkInDate.month << '-'
        << setw( 2 ) << newReservation.checkInDate.day << endl;

    cout << "Check out date: " << newReservation.checkOutDate.year << '-'
        << setw( 2 ) << newReservation.checkOutDate.month << '-'
        << setw( 2 ) << newReservation.checkOutDate.day << endl;

    // input room type and put it into newReservation.roomType
    int num1 = 0;
    int days[ 13 ] = { 0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
    if( newReservation.checkOutDate.month - newReservation.checkInDate.month == 0 ) {
        num1 += newReservation.checkOutDate.day - newReservation.checkInDate.day;
    }
    if( newReservation.checkOutDate.month - newReservation.checkInDate.month == 1 ) {
        num1 += days[ newReservation.checkInDate.month ] - newReservation.checkInDate.day;
        num1 += newReservation.checkOutDate.day;
    }
    if( newReservation.checkOutDate.month - newReservation.checkInDate.month == 2 ) {
        num1 += days[ newReservation.checkInDate.month + 1 ];
        num1 += days[ newReservation.checkInDate.month ] - newReservation.checkInDate.day;
        num1 += newReservation.checkOutDate.day;
    }
    if( newReservation.checkOutDate.month - newReservation.checkInDate.month == 3 ) {
        num1 += days[ newReservation.checkInDate.month + 1 ];
        num1 += days[ newReservation.checkInDate.month + 2 ];
        num1 += days[ newReservation.checkInDate.month ] - newReservation.checkInDate.day;
        num1 += newReservation.checkOutDate.day;
    }
    if( newReservation.checkOutDate.month - newReservation.checkInDate.month == 4 ) {
        num1 += days[ newReservation.checkInDate.month + 1 ];
        num1 += days[ newReservation.checkInDate.month + 2 ];
        num1 += days[ newReservation.checkInDate.month + 3 ];
        num1 += days[ newReservation.checkInDate.month ] - newReservation.checkInDate.day;
        num1 += newReservation.checkOutDate.day;
    }
    if( newReservation.checkOutDate.month - newReservation.checkInDate.month == 5 ) {
        num1 += days[ newReservation.checkInDate.month + 1 ];
        num1 += days[ newReservation.checkInDate.month + 2 ];
        num1 += days[ newReservation.checkInDate.month + 3 ];
        num1 += days[ newReservation.checkInDate.month + 4 ];
        num1 += days[ newReservation.checkInDate.month ] - newReservation.checkInDate.day;
        num1 += newReservation.checkOutDate.day;
    }
    int nn = 0,aaa=0;
    while( nn < availRooms.size() ) {
        if( availRooms[ nn ].date.month == newReservation.checkInDate.month ) {
            if( availRooms[ nn ].date.day == newReservation.checkInDate.day ) {
                for( int i = 0; i < num1; i++ ) {
                    if( availRooms[ nn + i ].numAvailRooms[ 1 ] == 0 ) {
                        aaa++;
                        break;
                    }
                }
                for( int i = 0; i < num1; i++ ) {
                    if( availRooms[ nn + i ].numAvailRooms[ 2 ] == 0 ) {
                        aaa++;
                        break;
                    }
                }
                for( int i = 0; i < num1; i++ ) {
                    if( availRooms[ nn + i ].numAvailRooms[ 3 ] == 0 ) {
                        aaa++;
                        break;
                    }
                }
                for( int i = 0; i < num1; i++ ) {
                    if( availRooms[ nn + i ].numAvailRooms[ 4 ] == 0 ) {
                        aaa++;
                        break;
                    }
                }
                for( int i = 0; i < num1; i++ ) {
                    if( availRooms[ nn + i ].numAvailRooms[ 5 ] == 0 ) {
                        aaa++;
                        break;
                    }
                }
                if( aaa == 5 ) {
                    cout << "\nThere are no rooms available!\n";
                    cin.ignore();
                    return;
                }
                break;
            }
        }
        nn++;
    }
    int a,n=0,num=0;
    cout << "\nThe number of available rooms of each room type:\n";
    cout << "\n      Date"
        << setw( 16 ) << setfill( ' ' ) << "Superior Room"
        << setw( 14 ) << "Deluxe Room"
        << setw( 19 ) << "Deluxe Twin Room"
        << setw( 17 ) << "Superior Suite"
        << setw( 15 ) << "Deluxe Suite" << endl;
        if( newReservation.checkOutDate.month - newReservation.checkInDate.month == 0 ) {
            num += newReservation.checkOutDate.day - newReservation.checkInDate.day;
        }
        if( newReservation.checkOutDate.month - newReservation.checkInDate.month == 1 ) {
            num += days[ newReservation.checkInDate.month ] - newReservation.checkInDate.day;
            num += newReservation.checkOutDate.day;
        }
        if( newReservation.checkOutDate.month - newReservation.checkInDate.month == 2 ) {
            num += days[ newReservation.checkInDate.month + 1 ];
            num += days[ newReservation.checkInDate.month] - newReservation.checkInDate.day;
            num += newReservation.checkOutDate.day;
        }
        if( newReservation.checkOutDate.month - newReservation.checkInDate.month == 3 ) {
            num += days[ newReservation.checkInDate.month + 1 ];
            num += days[ newReservation.checkInDate.month + 2 ];
            num += days[ newReservation.checkInDate.month ] - newReservation.checkInDate.day;
            num += newReservation.checkOutDate.day;
        }
        if( newReservation.checkOutDate.month - newReservation.checkInDate.month == 4 ) {
            num += days[ newReservation.checkInDate.month + 1 ];
            num += days[ newReservation.checkInDate.month + 2 ];
            num += days[ newReservation.checkInDate.month + 3 ];
            num += days[ newReservation.checkInDate.month ] - newReservation.checkInDate.day;
            num += newReservation.checkOutDate.day;
        }
        if( newReservation.checkOutDate.month - newReservation.checkInDate.month == 5 ) {
            num += days[ newReservation.checkInDate.month + 1 ];
            num += days[ newReservation.checkInDate.month + 2 ];
            num += days[ newReservation.checkInDate.month + 3 ];
            num += days[ newReservation.checkInDate.month + 4 ];
            num += days[ newReservation.checkInDate.month ] - newReservation.checkInDate.day;
            num += newReservation.checkOutDate.day;
        }
        int q=0, r=0, x=0, y=0, z=0;
    while( n < availRooms.size() ) {
        if( availRooms[ n ].date.month == newReservation.checkInDate.month ) {
            if( availRooms[ n ].date.day == newReservation.checkInDate.day ) {
                for( int i = 0; i < num; i++ ) {
                    displayAvailRooms( availRooms[ n+i ] );
                    if( availRooms[ n + i ].numAvailRooms[ 1 ] == 0 ) {
                        q++;
                    }
                    if( availRooms[ n + i ].numAvailRooms[ 2 ] == 0 ) {
                        r++;
                    }
                    if( availRooms[ n + i ].numAvailRooms[ 3 ] == 0 ) {
                        x++;
                    }
                    if( availRooms[ n + i ].numAvailRooms[ 4 ] == 0 ) {
                        y++;
                    }
                    if( availRooms[ n + i ].numAvailRooms[ 5 ] == 0 ) {
                        z++;
                    }
                }
                break;
            }
        }
        n++;
    } 
    // input number of rooms and put it into newReservation.numRooms
    while( 1 ) {
        cout << "\nSelect Room Type¡G\n";
        if( q == 0 )cout << "1. Superior Room\n";
        if( r == 0 )cout << "2. Deluxe Room\n";
        if( x == 0 )cout << "3. Deluxe Twin Room\n";
        if( y == 0 )cout << "4. Superior Suite\n";
        if( z == 0 )cout << "5. Deluxe Suite\n";
        cout << "6. Give up\n";
        cout << "?";
        cin >> a;
        cin.ignore();
        if( q != 0 ) {
            if( a == 1 )
                continue;
        }
        if( r != 0 ) {
            if( a == 2 )
                continue;
        }
        if( x != 0 ) {
            if( a == 3 )
                continue;
        }
        if( y != 0 ) {
            if( a == 4 )
                continue;
        }
        if( z != 0 ) {
            if( a == 5 )
                continue;
        }
        if( a < 1 || a>6 )
            continue;
        if( a == 6 ) {
            return;
        }
        while( 1 ) {
            cout << "\nNumber of rooms( " << compMinNumRooms( newReservation, availRooms, a ) << " rooms available ) :";
            cin >> newReservation.numRooms;
            cin.ignore();
            if( newReservation.numRooms > 0 && newReservation.numRooms <= compMinNumRooms( newReservation, availRooms, a ) ){//
                int num = 0;
                if( newReservation.checkOutDate.month - newReservation.checkInDate.month == 0 ) {
                    num += newReservation.checkOutDate.day - newReservation.checkInDate.day;
                }
                if( newReservation.checkOutDate.month - newReservation.checkInDate.month == 1 ) {
                    num += days[ newReservation.checkInDate.month ] - newReservation.checkInDate.day;
                    num += newReservation.checkOutDate.day;
                }
                if( newReservation.checkOutDate.month - newReservation.checkInDate.month == 2 ) {
                    num += days[ newReservation.checkInDate.month + 1 ];
                    num += days[ newReservation.checkInDate.month ] - newReservation.checkInDate.day;
                    num += newReservation.checkOutDate.day;
                }
                if( newReservation.checkOutDate.month - newReservation.checkInDate.month == 3 ) {
                    num += days[ newReservation.checkInDate.month + 1 ];
                    num += days[ newReservation.checkInDate.month + 2 ];
                    num += days[ newReservation.checkInDate.month ] - newReservation.checkInDate.day;
                    num += newReservation.checkOutDate.day;
                }
                if( newReservation.checkOutDate.month - newReservation.checkInDate.month == 4 ) {
                    num += days[ newReservation.checkInDate.month + 1 ];
                    num += days[ newReservation.checkInDate.month + 2 ];
                    num += days[ newReservation.checkInDate.month + 3 ];
                    num += days[ newReservation.checkInDate.month ] - newReservation.checkInDate.day;
                    num += newReservation.checkOutDate.day;
                }
                if( newReservation.checkOutDate.month - newReservation.checkInDate.month == 5 ) {
                    num += days[ newReservation.checkInDate.month + 1 ];
                    num += days[ newReservation.checkInDate.month + 2 ];
                    num += days[ newReservation.checkInDate.month + 3 ];
                    num += days[ newReservation.checkInDate.month + 4 ];
                    num += days[ newReservation.checkInDate.month ] - newReservation.checkInDate.day;
                    num += newReservation.checkOutDate.day;
                }
                int q = 0, r = 0, x = 0, y = 0, z = 0, n = 0;
                while( n < availRooms.size() ) {
                    if( availRooms[ n ].date.month == newReservation.checkInDate.month ) {
                        if( availRooms[ n ].date.day == newReservation.checkInDate.day ) {
                            for( int i = 0; i < num; i++ ) {
                                availRooms[ n + i ].numAvailRooms[ a ]-= newReservation.numRooms;//modify
                            }
                            break;
                        }
                    }
                    n++;
                }
                //
                break;
            }
            cout << endl;

        }
        break;
    }
    newReservation.roomType = a;
    cout << "\nID Number¡G";
    cin >> newReservation.id;
    cout << "\nName¡G";
    cin >> newReservation.name;
    cout << "\nMobile Phone¡G";
    cin >> newReservation.mobile;
    cin.ignore();

    cout << "\nYour reservations:\n\n";
    cout << "    Name   Fare       Mobile          Room type"
        << "   Number of rooms   Check in date   Check out date\n";
    displayReservations( newReservation );

    reservations.push_back( newReservation );
}

// input check in date and check out date by user, and
// put them into reservation.checkInDate and reservation.checkOutDate, respectively.
void inputDates( const Date &currentDate, Reservation &newReservation )
{
    int a,b;
    int days[ 13 ] = { 0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
    Date c,e,in,out;
    vector<Date> d;
    c = currentDate;
    e = currentDate;
    cout << "\nPlease Input Check In Date\n";
    cout << "\nMonth:\n";
    for( int i = 0; i < 6; i++ ) {
        if( c.month + i > 12 ) {
            c.year++;
            c.month = 1;
        }
        if( c.month < 10 ) {
            cout << i + 1 << ". "<< c.year << "-0" << c.month << endl;
            d.push_back( c );
        }
        else {
            cout << i + 1 << ". " << c.year << "-" << c.month << endl;
            d.push_back( c );
        }
        c.month++;
    }
    cout << "?";
    cin >> a;
    cout << endl;
    cin.ignore();
    while( a ) {
        if( a == 1 ) {
            cout << "Day(" << currentDate.day << "~" << days[ currentDate.month ] << "):";
            cin >> in.day;
            cout << endl;
            in.month = currentDate.month;
            in.year = currentDate.year;
            if( in.day < currentDate.day || in.day>days[ currentDate.month ] )
                continue;
            break;
        }
        if( a == 2 ) {
            cout << "Day(1~" << days[ d[ 2 - 1 ].month ] << "):";
            cin >> in.day;
            cout << endl;
            in.month = d[ 2 - 1 ].month;
            in.year = d[ 2 - 1 ].year;
            if( in.day < 1 || in.day> days[ d[ 2 - 1 ].month ] )
                continue;
            break;
        }
        if( a == 3 ) {
            cout << "Day(1~" << days[ d[ 3 - 1 ].month ] << "):";
            cin >> in.day;
            cout << endl;
            in.month = d[ 3 - 1 ].month;
            in.year = d[ 3 - 1 ].year;
            if( in.day < 1 || in.day > days[ d[ 3 - 1 ].month ] )
                continue;
            break;
        }
        if( a == 4 ) {
            cout << "Day(1~" << days[ d[ 4 - 1 ].month ] << "):";
            cin >> in.day;
            cout << endl;
            in.month = d[ 4 - 1 ].month;
            in.year = d[ 4 - 1 ].year;
            if( in.day < 1 || in.day > days[ d[ 4 - 1 ].month ] )
                continue;
            break;
        }
        if( a == 5 ) {
            cout << "Day(1~" << days[ d[ 5 - 1 ].month ] << "):";
            cin >> in.day;
            cout << endl;
            in.month = d[ 5 - 1 ].month;
            in.year = d[ 5 - 1 ].year;
            if( in.day < 1 || in.day > days[ d[ 5 - 1 ].month ] )
                continue;
            break;
        }
        if( a == 6 ) {
            cout << "Day(1~" << days[ d[ 6 - 1 ].month ] - 1 << "):";
            cin >> in.day;
            cout << endl;
            in.month = d[ 6 - 1 ].month;
            in.year = d[ 6 - 1 ].year;
            if( in.day < 1 || in.day > days[ d[ 6 - 1 ].month ]-1 )
                continue;
            break;
        }
    }
    cout << "\nPlease Input Check Out Date\n";
    cout << "\nMonth:\n";
    d.clear();
    c = currentDate;
    Date aa;
    aa = in;
    for( int i = 0; i < 6; i++ ) {
        if( c.month + i > 12 ) {
            c.year++;
            c.month = 1;
        }
        if( c.month < 10 ) {
            if( c.month < in.month ) {
                c.month++;
                continue;
            }    
            if( aa.day == days[ in.month ] ) {
                aa.month++;
                c.month++;
                aa.day = 1;
                continue;
            }
            cout << i + 1 << ". "<< c.year << "-0" << c.month << endl;
            d.push_back( c );
        }
        else {
            if( c.month < in.month ) {
                c.month++;
                continue;
            }
            if( in.day == days[ in.month ] ) {
                c.month++;
                continue;
            }
            cout << i + 1 << ". " << setw( 5 ) << c.year << "-" << c.month << endl;
            d.push_back( c );
        }
        c.month++;
    }
    cout << "?";
    cin >> a;
    cout << endl;
    cin.ignore();
    int gg=1;
    while( a ) {
        if( a == 1 ) {
            if( e.month == in.month ) {
                if( in.day >= e.day ) {
                    e.day = in.day+1;
                }
            }
            cout << "Day(" << e.day << "~" << days[ e.month ] << "):";
            cin >> out.day;
            cout << endl;
            out.month = currentDate.month;
            out.year = currentDate.year;
            if( out.day < currentDate.day || out.day>days[ currentDate.month ] || out.day < e.day || in.day > days[ currentDate.month ] )
                continue;
            break;
        }
        if( a == 2 ) {
            if( d[ d.size() - 5 ].month == in.month ) {
                if( in.day >= gg ) {
                    gg = in.day+1;
                }
            }
            cout << "Day("<< gg <<"~" << days[ d[ d.size() - 5 ].month ] << "):";
            cin >> out.day;
            cout << endl;
            out.month = d[ d.size() - 5 ].month;
            out.year = d[ d.size() - 5 ].year;
            if( out.day < 1 || out.day> days[ d[ d.size() - 5 ].month ] || out.day < gg || gg > days[ d[ d.size() - 5 ].month ] )
                continue;
            break;
        }
        if( a == 3 ) {
            if( d[ d.size() - 4].month == in.month ) {
                if( in.day >= gg ) {
                   gg = in.day+1;
                }
            }
            cout << "Day(" << gg << "~" << days[ d[ d.size() - 4 ].month ] << "):";
            cin >> out.day;
            cout << endl;
            out.month = d[ d.size() - 4 ].month;
            out.year = d[ d.size() - 4 ].year;
            if( out.day < 1 || out.day > days[ d[ d.size() - 4 ].month ] || out.day < gg || gg > days[ d[ d.size() - 4 ].month ] )
                continue;
            break;
        }
        if( a == 4 ) {
            if( d[ d.size() - 3 ].month == in.month ) {
                if( in.day >= gg ) {
                    gg = in.day+1;
                }
            }
            cout << "Day(" << gg << "~" << days[ d[ d.size() - 3 ].month ] << "):";
            cin >> out.day;
            cout << endl;
            out.month = d[ d.size() - 3 ].month;
            out.year = d[ d.size() - 3 ].year;
            if( out.day < 1 || out.day > days[ d[ d.size() - 3 ].month ] || out.day < gg || gg > days[ d[ d.size() - 3 ].month ] )
                continue;
            break;
        }
        if( a == 5 ) {
            if( d[ d.size() - 2 ].month == in.month ) {
                if( in.day >= gg ) {
                    gg = in.day+1;
                }
            }
            cout << "Day(" << gg << "~" << days[ d[ d.size() - 2 ].month ] << "):";
            cin >> out.day;
            cout << endl;
            out.month = d[ d.size() - 2 ].month;
            out.year = d[ d.size() - 2 ].year;
            if( out.day < 1 || out.day > days[ d[ d.size() - 2 ].month ] || out.day < gg || gg > days[ d[ d.size() - 2 ].month ] )
                continue;
            break;
        }
        if( a == 6 ) {
            if( d[ d.size()-1 ].month == in.month ) {
                if( in.day >= gg ) {
                    gg = in.day+1;
                }
            }
            cout << "Day(" << gg << "~" << days[ d[ d.size() - 1 ].month ]  << "):";
            cin >> out.day;
            cout << endl;
            out.month = d[ d.size() - 1 ].month;
            out.year = d[ d.size() - 1 ].year;
            if( out.day < 1 || out.day > days[ d[ d.size() - 1 ].month ] || out.day < gg || gg > days[ d[ d.size() - 1 ].month ] )
                continue;
            break;
        }
    }
    newReservation.checkInDate = in;
    newReservation.checkOutDate = out;
}

int compMinNumRooms( const Reservation &reservation, vector< AvailRoom > &availRooms, int roomType )
{
    int num = 0;
    int days[ 13 ] = { 0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
    if( reservation.checkOutDate.month - reservation.checkInDate.month == 0 ) {
        num += reservation.checkOutDate.day - reservation.checkInDate.day;
    }
    if( reservation.checkOutDate.month - reservation.checkInDate.month == 1 ) {
        num += days[ reservation.checkInDate.month ] - reservation.checkInDate.day;
        num += reservation.checkOutDate.day;
    }
    if( reservation.checkOutDate.month - reservation.checkInDate.month == 2 ) {
        num += days[ reservation.checkInDate.month + 1 ];
        num += days[ reservation.checkInDate.month ] - reservation.checkInDate.day;
        num += reservation.checkOutDate.day;
    }
    if( reservation.checkOutDate.month - reservation.checkInDate.month == 3 ) {
        num += days[ reservation.checkInDate.month + 1 ];
        num += days[ reservation.checkInDate.month + 2 ];
        num += days[ reservation.checkInDate.month ] - reservation.checkInDate.day;
        num += reservation.checkOutDate.day;
    }
    if( reservation.checkOutDate.month - reservation.checkInDate.month == 4 ) {
        num += days[ reservation.checkInDate.month + 1 ];
        num += days[ reservation.checkInDate.month + 2 ];
        num += days[ reservation.checkInDate.month + 3 ];
        num += days[ reservation.checkInDate.month ] - reservation.checkInDate.day;
        num += reservation.checkOutDate.day;
    }
    if( reservation.checkOutDate.month - reservation.checkInDate.month == 5 ) {
        num += days[ reservation.checkInDate.month + 1 ];
        num += days[ reservation.checkInDate.month + 2 ];
        num += days[ reservation.checkInDate.month + 3 ];
        num += days[ reservation.checkInDate.month + 4 ];
        num += days[ reservation.checkInDate.month ] - reservation.checkInDate.day;
        num += reservation.checkOutDate.day;
    }
    int q = 0, r = 0, x = 0, y = 0, z = 0, n = 0;
    int min=0;
    while( n < availRooms.size() ) {
        if( availRooms[ n ].date.month == reservation.checkInDate.month ) {
            if( availRooms[ n ].date.day == reservation.checkInDate.day ) {
                min = availRooms[ n ].numAvailRooms[ roomType ];
                for( int i = 0; i < num; i++ ) {
                    if( availRooms[ n + i ].numAvailRooms[ roomType ] < min ) {
                        min = availRooms[ n + i ].numAvailRooms[ roomType ];
                    }
                }
                break;
            }
        }
        n++;
    }
    return min;
}

void displayAvailRooms( const AvailRoom &availableRoom )
{
    cout << availableRoom.date.year << "-"
        << setw( 2 ) << setfill( '0' ) << availableRoom.date.month << "-"
        << setw( 2 ) << availableRoom.date.day
        << setw( 16 ) << setfill( ' ' ) << availableRoom.numAvailRooms[ 1 ]
        << setw( 14 ) << availableRoom.numAvailRooms[ 2 ]
        << setw( 19 ) << availableRoom.numAvailRooms[ 3 ]
        << setw( 17 ) << availableRoom.numAvailRooms[ 4 ]
        << setw( 15 ) << availableRoom.numAvailRooms[ 5 ] << endl;
}

void displayReservations( const Reservation &reservation )
{
    cout << setw( 8 ) << setfill( ' ' ) << reservation.name
        << setw( 7 ) << reservation.numRooms * roomRate[ reservation.roomType ] * numNights( reservation )
        << setw( 13 ) << reservation.mobile
        << setw( 19 ) << roomTypeName[ reservation.roomType ]
        << setw( 18 ) << reservation.numRooms
        << setw( 10 ) << reservation.checkInDate.year << "-"
        << setw( 2 ) << setfill( '0' ) << reservation.checkInDate.month << "-"
        << setw( 2 ) << reservation.checkInDate.day
        << setw( 11 ) << setfill( ' ' ) << reservation.checkOutDate.year << "-"
        << setw( 2 ) << setfill( '0' ) << reservation.checkOutDate.month << "-"
        << setw( 2 ) << reservation.checkOutDate.day << endl;
}

int numNights( Reservation reservation )
{
    tm time1 = { 0, 0, 8, 0, 0, 0, 0, 0, 0 };
    tm time2 = { 0, 0, 8, 0, 0, 0, 0, 0, 0 };
    time1.tm_year = reservation.checkInDate.year - 1900;
    time1.tm_mon = reservation.checkInDate.month-1;
    time1.tm_mday = reservation.checkInDate.day;
    time2.tm_year = reservation.checkOutDate.year - 1900;
    time2.tm_mon = reservation.checkOutDate.month-1;
    time2.tm_mday = reservation.checkOutDate.day;
    int numSeconds = static_cast< int >( mktime( &time2 ) - mktime( &time1 ) );
    return numSeconds / ( 24 * 3600 );
}

void reservationInquiry( const vector< Reservation > &reservations, const Date &currentDate )
{
    cout << "\nEnter Your ID Number¡G";
    char id[ 12 ];
    cin >> id;
    cin.ignore();

    if( !exist( reservations, currentDate, id ) )
    {
        cout << "\nSorry! You do not have reservations!\n";
        return;
    }

    cout << "\n    Name   Fare       Mobile          Room type"
        << "   Number of rooms   Check in date   Check out date\n";
    for( size_t i = 0; i < reservations.size(); i++ )
        if( strcmp( reservations[ i ].id, id ) == 0 )
            displayReservations( reservations[ i ] );
}

bool exist( const vector< Reservation > &reservations, const Date &currentDate, char id[] )
{
    for( size_t i = 0; i < reservations.size(); i++ )
        if( strcmp( reservations[ i ].id, id ) == 0 &&
            lessEqual( currentDate, reservations[ i ].checkInDate ) )
            return true;

    return false;
}

void saveAvailRooms( const vector< AvailRoom > &availRooms )
{
    ofstream outFile( "Available Rooms blank.dat", ios::out | ios::binary );
    outFile.clear();
    if( !outFile ) {
        cout << "File could not be open" << endl;
        exit( 1 );
    }
    AvailRoom a;
    int n = 0;
    while( n < availRooms.size() ) {
        a = availRooms[ n ];
        outFile.write( reinterpret_cast< char * >( &a ), sizeof( a ) );
        n++;
    }
    outFile.close();
}

void saveReservations( const vector< Reservation > &reservations )
{
    ofstream outFile( "Reservations.dat", ios::out | ios::binary );
    if( !outFile ) {
        cout << "File could not be open" << endl;
        exit( 1 );
    }
    Reservation a;
    int n = 0;
    while( n < reservations .size()) {
        a = reservations[ n ];
        outFile.write( reinterpret_cast< char * >( &a ), sizeof( a ) );
        n++;
    }
    outFile.close();
}


