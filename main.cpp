#include <iostream>
#include <iomanip>
#include <string>
#include <cctype>
#include <conio.h>
#include <windows.h>
#include <algorithm>
#include <mysql.h> //mysql lib
#include "DatabaseConnection.h"
#include "Attraction.h"
using namespace std;

string UserID, attrPurchase, purchaseID = "";
int role;
double totalPayment = 0;
linkedList list;

//LIST OF FUNCTION (FUNCTION PROTOTYPE)
void Header();
void HeaderSuggest();
void Register();
string GetState();
void Login();
void ViewAttraction();
void Staff();
void Manager();
void Customer();
void Attraction();
void DetailView(string AttrID);
void DetailAttraction(string AttrID);
void Suggest();
void Search(string AttrName);
void Searching();
void Suggestion();
void Profile();
void ProfileDetail();
void EditProfile();
void AttrAdmin();
void AddAttraction();
void AddPrice();
void DetailEdit(string AttrID);
void EditPrice(string AttrID);
void DeleteAttraction();
void Report();
void Purchase();
void Payment();
void ViewStaff();
void ViewCustomer();
void ReportAge();
void ReportState();
void ReportCitizen();
void ReportSale();
void ReportSold();

int main()
{
    system("title Melaka Tourism Digital Kiosk");
    db_response::ConnectionFunction(); //to call a class
    system("cls");

    int menu;
    role = 0;
    UserID = "";

    Header();

    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 240);
    qstate = mysql_query(conn, "SELECT a.Name, TIME_FORMAT(a.Open_Time, '%H:%i'), TIME_FORMAT(a.Close_Time, '%H:%i'), FORMAT(MIN(p.PricePerUnit), 2) FROM attraction a INNER JOIN Price p ON a.AttractionID = p.AttractionID INNER JOIN purchase pr ON a.AttractionID = pr.AttractionID GROUP BY a.AttractionID ORDER BY COUNT(pr.AttractionID) DESC LIMIT 5");

    if (!qstate)
    {
        cout << setfill(' ') << setw(100) << "The Most Popular Attraction" << endl;
        cout << "\t\t\t" << setfill('_') << setw(120) << "_" << endl;
        cout << "\t\t\t" << setfill(' ') << setw(4) << "No" << setw(50) << "Attraction" << setw(20) << "Opening Time" << setw(20) << "Closing Time" << setw(20) << "Price" << endl;
        cout << "\t\t\t" << setfill('_') << setw(120) << "_" << endl;

        res = mysql_store_result(conn);
        int i = 1;
        while (row = mysql_fetch_row(res))
        {
            cout << "\t\t\t" << setfill(' ') << setw(4) << i << setw(50) << row[0] << setw(20) << row[1] << setw(20) << row[2] << setw(20) << "From RM " << row[3] << endl;
            i++;
        }
    }
    else
        cout << "Query Execution Problem!" << mysql_errno(conn) << endl;

    cout << "\n\n\t\t1. Login" << endl << endl;
    cout << "\t\t2. Register" << endl<< endl;
    cout << "\t\t3. Attraction Menu" << endl << endl;
    cout << "\t\tChoose Menu (1-3): ";
    cin >> menu;

    switch (menu)
    {
    case 1:
        Login();
        break;
    case 2:
        Register();
        break;
    case 3:
        Attraction();
        break;
    default:
        cout << "\n\t\t\tPlease choose between 1 - 3. Press Enter To Continue...";
        _getch(); //get char - pause console
        main();
        break;
    }
    return 0;
}

void Header()
{
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 241);
    cout << "\t" << setfill('_') << setw(155) << "_" << endl << endl;
    cout << setfill(' ') << setw(100) << "Melaka Tourism Digital Kiosk" << endl;
    cout << "\t" << setfill('_') << setw(155) << "_" << endl << endl << endl;
}

void HeaderSuggest()
{
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 249);
    cout << "\t\t\t\t" << setfill('*') << setw(100) << "*" << endl;
    cout << setfill(' ') << setw(95) << "Attraction Menu (Suggestion)" << endl;
    cout << "\t\t\t\t" << setfill('*') << setw(100) << "*" << endl << endl;
}

void Register()
{
    system("cls");
    string name, username, password, year, gender, citizen, state;
    string staff = "";
    int option;

    Header();

    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 249);
    cout << "\t\t\t\t" << setfill('*') << setw(100) << "*" << endl;
    cout << setfill(' ') << setw(90) << "Registration Menu" << endl;
    cout << "\t\t\t\t" << setfill('*') << setw(100) << "*" << endl << endl;

    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 240);

    cin.ignore(1, '\n');
    cout << "\t\tUsername(for staff insert Staff no) : ";
    getline(cin, username);
    transform(username.begin(), username.end(), username.begin(), ::toupper);

    if (username.size() < 5)
    {
        cout << "\n\t\t\tInvalid characters length for username. Minimum length is 5. Press to continue...";
        _getch();
        Register();
    }

    for (int i = 0; i < 5; i++)
        staff = staff + username.at(i);

    if (staff == "STAFF")
    {
        string checkUser_query = "select * from Admin where Username = '" + username + "' AND Active = TRUE";
        const char* cu = checkUser_query.c_str(); // convert the query into char
        qstate = mysql_query(conn, cu); // execute query
        res = mysql_store_result(conn);

        if (res->row_count == 1)
        {
            cout << "\n\n\t\t\tUsername already exist. Please choose difference username..."<< endl << endl;
            cout << "\t\t1. Register again" << endl << endl;
            cout << "\t\t2. Login" << endl << endl;
            cout << "\t\t3. Back to Main Page" << endl << endl;
            cout << "\t\tChoose option (1-3): ";
            cin >> option;

            switch (option)
            {
            case 1:
                Register();
                break;
            case 2:
                Login();
                break;
            case 3:
                main();
                break;
            default:
                cout << "\n\t\t\tPlease choose between 1 - 3. Press Enter To Continue...";
                _getch();
                main();
            }

        }

        cout << "\n\t\tFull Name\t\t\t    : ";
        getline(cin, name);
        cout << "\n\t\tYear of Bitrh\t\t\t    : ";
        getline(cin, year);

        string checkYear_query = "SELECT YEAR(CURDATE())";
        const char* c = checkYear_query.c_str(); // convert the query into char
        qstate = mysql_query(conn, c); // execute query
        res = mysql_store_result(conn);
        row = mysql_fetch_row(res);

        string curYear = row[0];
        int age = stoi(curYear) - stoi(year);

        // check age
        while (age < 18)
        {
            cout << "\n\n\t\t\tToo young to be a staff. Insert again...";
            cout << endl << endl << "\t\tYear of Bitrh\t\t\t    : ";
            getline(cin, year);
            string checkYear_query = "SELECT YEAR(CURDATE())";
            const char* c = checkYear_query.c_str(); // convert the query into char
            qstate = mysql_query(conn, c); // execute query
            res = mysql_store_result(conn);
            row = mysql_fetch_row(res);

            string curYear = row[0];
            int age = stoi(curYear) - stoi(year);

            if (age > 18)
                break;
        }

        while (year > curYear || year < "1900")
        {
           
            cout << "\n\n\t\t\tInvalid year. Insert again...";

            cout << endl << endl << "\t\tYear of Bitrh\t\t\t    : ";
            getline(cin, year);

            string checkYear_query = "SELECT YEAR(CURDATE())";
            const char* c = checkYear_query.c_str(); // convert the query into char
            qstate = mysql_query(conn, c); // execute query
            res = mysql_store_result(conn);
            row = mysql_fetch_row(res);

            string curYear = row[0];
            int age = stoi(curYear) - stoi(year);
            cout << (year > curYear || year < "1900" || age < 18);
        }


        cout << "\n\t\tGender (F/M)\t\t\t    : ";
        getline(cin, gender);
        transform(gender.begin(), gender.end(), gender.begin(), ::toupper);

        while (gender != "F" && gender != "M")
        {
            cout << "\n\n\t\t\tInvalid gender. Choose between 'F' for female or 'M' for male. Enter again" << endl << endl;
            cout << "\n\n\t\tGender (F/M)\t\t\t    : ";
            getline(cin, gender);
            transform(gender.begin(), gender.end(), gender.begin(), ::toupper);
        }

        cout << "\n\t\tPassword\t\t\t    : ";
        //cin.ignore(1, '\n');
        getline(cin, password);

        string insertCustomer_query = "INSERT INTO Admin(Name, StaffNo, Username, Password, YearOfBirth, Gender, Position, Active) values ('" + name + "', '" + username + "', '" + username + "', '" + password + "', '" + year + "', '" + gender + "', 'Staff', TRUE)";
        const char* q = insertCustomer_query.c_str();
        qstate = mysql_query(conn, q);

        if (!qstate)
        {
            cout << endl << "\n\t\t\tYou have been registered. Press Enter to Continue...";
            _getch();
            main();
        }
        else
        {
            cout << "Query Execution Problem!" << mysql_errno(conn) << endl;
        }
    }
    else
    {
        string checkUser_query = "select * from Customer where Username = '" + username + "' AND Active = TRUE";
        const char* cu = checkUser_query.c_str(); // convert the query into char
        qstate = mysql_query(conn, cu); // execute query
        res = mysql_store_result(conn);

        if (res->row_count == 1)
        {
            cout << "\n\n\t\t\tUsername already exist. Please choose difference username..." << endl << endl;
            cout << "\t\t1. Register again" << endl << endl;
            cout << "\t\t2. Login" << endl<< endl;
            cout << "\t\t3. Back to Main Page" << endl << endl;
            cout << "\t\tChoose option (1-3): ";
            cin >> option;

            switch (option)
            {
            case 1:
                Register();
                break;
            case 2:
                Login();
                break;
            case 3:
                main();
                break;
            default:
                cout << "\n\n\t\t\tPlease choose between 1 - 3. Press Enter To Continue...";
                _getch();
                main();
            }
        }

        cout << "\n\t\tFull Name\t\t\t    : ";
        getline(cin, name);
        cout << "\n\t\tYear of Bitrh\t\t\t    : ";
        getline(cin, year);

        string checkYear_query = "SELECT YEAR(CURDATE())";
        const char* c = checkYear_query.c_str(); // convert the query into char
        qstate = mysql_query(conn, c); // execute query
        res = mysql_store_result(conn);
        row = mysql_fetch_row(res);

        string curYear = row[0];

        while (year > curYear || year < "1900")
        {
            
            cout << "\n\n\t\t\tInvalid year. Insert again...";

            cout << "\n\n\t\tYear of Bitrh\t\t\t    : ";
            getline(cin, year);

            string checkYear_query = "SELECT YEAR(CURDATE())";
            const char* c = checkYear_query.c_str(); // convert the query into char
            qstate = mysql_query(conn, c); // execute query
            res = mysql_store_result(conn);
            row = mysql_fetch_row(res);

            string curYear = row[0];
        }

        cout << "\n\t\tGender (F/M)\t\t\t    : ";
        getline(cin, gender);
        transform(gender.begin(), gender.end(), gender.begin(), ::toupper);

        while (gender != "F" && gender != "M")
        {
            cout << "\n\t\t\tInvalid gender. Choose between 'F' for female or 'M' for male. Enter again" << endl;
            cout << "\n\t\tGender (F/M)\t\t\t    : ";
            getline(cin, gender);
            transform(gender.begin(), gender.end(), gender.begin(), ::toupper);
        }

        cout << "\n\t\tPassword\t\t\t    : ";
        getline(cin, password);

        cout << "\n\t\tCitizen (Y/N)\t\t\t    : ";
        getline(cin, citizen);

        if (citizen == "y" || citizen == "Y")
            citizen = "TRUE";
        else
            citizen = "FALSE";

        state = GetState();

        string insertCustomer_query = "INSERT INTO Customer(Name, Username, Password, YearOfBirth, Gender, Citizens, State, Active) VALUES ('" + name + "', '" + username + "', '" + password + "', '" + year + "', '" + gender + "', " + citizen + ", '" + state + "', TRUE)";
        const char* q = insertCustomer_query.c_str();
        qstate = mysql_query(conn, q);

        if (!qstate)
        {
            cout << endl << "\n\t\t\tYou have been registered. Press Enter to Continue...";
            _getch();
            main();
        }
        else
            cout << "Query Execution Problem!" << mysql_errno(conn) << endl;
    }
   
}

string GetState()
{
    int chooseState;
    string state;

    cout << "\n\n\t\tList of state" << endl;
    cout << "\t\t1. Johor" << endl;
    cout << "\t\t2. Kedah" << endl;
    cout << "\t\t3. Kelantan" << endl;
    cout << "\t\t4. Kuala Lumpur" << endl;
    cout << "\t\t5. Labuan" << endl;
    cout << "\t\t6. Melaka" << endl;
    cout << "\t\t7. Negeri Sembilan" << endl;
    cout << "\t\t8. Pahang" << endl;
    cout << "\t\t9. Perak" << endl;
    cout << "\t\t10. Perlis" << endl;
    cout << "\t\t11. Pulau Pinang" << endl;
    cout << "\t\t12. Putrajaya" << endl;
    cout << "\t\t13. Sabah" << endl;
    cout << "\t\t14. Sarawak" << endl;
    cout << "\t\t15. Selangor" << endl;
    cout << "\t\t16. Terengganu" << endl;
    cout << "\t\t17. Other" << endl;
    cout << "\n\t\tChoose State (1-17): ";
    cin >> chooseState;

    while (chooseState > 17)
    {
        cout << "\n\t\t\tIncorrect input. Choose between 1 - 17..." << endl;
        cout << "\n\t\tChoose State (1-17): ";
        cin >> chooseState;
    }

    switch (chooseState)
    {
    case 1:
        state = "Johor";
        break;
    case 2:
        state = "Kedah";
        break;
    case 3:
        state = "Kelantan";
        break;
    case 4:
        state = "Kuala Lumpur";
        break;
    case 5:
        state = "Labuan";
        break;
    case 6:
        state = "Melaka";
        break;
    case 7:
        state = "Negeri Sembilan";
        break;
    case 8:
        state = "Pahang";
        break;
    case 9:
        state = "Perak";
        break;
    case 10:
        state = "Perlis";
        break;
    case 11:
        state = "Pulau Pinang";
        break;
    case 12:
        state = "Putrajaya";
        break;
    case 13:
        state = "Sabah";
        break;
    case 14:
        state = "Sarawak";
        break;
    case 15:
        state = "Selangor";
        break;
    case 16:
        state = "Terengganu";
        break;
    case 17:
        state = "Other";
        break;
    }

    return state;
}

void Login()
{
    system("cls");
    string password, username;
    int option;

    Header();

    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 249);
    cout << "\t\t\t\t" << setfill('*') << setw(100) << "*" << endl;
    cout << setfill(' ') << setw(90) << "Login Menu" << endl;
    cout << "\t\t\t\t" << setfill('*') << setw(100) << "*" << endl << endl;

    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 240);
    cout << "\t\tUser Role" << endl << endl;
    cout << "\t\t1. Admin" << endl << endl;
    cout << "\t\t2. Customer" << endl << endl;
    cout << "\t\tChoose Role (1/2): ";
    cin >> role;

    if (role != 1 && role != 2)
    {
        cout << "\n\t\t\tPlease choose between 1 or 2. Press Enter To Continue...";
        _getch();
        Login();
    }

    cout << "\n\t\tUsername \t : ";
    cin >> username;
    transform(username.begin(), username.end(), username.begin(), ::toupper);
    cout << "\n\t\tPassword \t : ";

    char ch;
    while ((ch = _getch()) != 13)
    {
        password += ch;
        cout << '*';
    }


    if (role == 1)
    {
        string checkUser_query = "SELECT AdminID, Position FROM Admin WHERE Username ='" + username + "' AND Password = '" + password + "' AND Active = TRUE";
        const char* cu = checkUser_query.c_str();
        qstate = mysql_query(conn, cu);

        if (!qstate)
        {
            res = mysql_store_result(conn);
            if (res->row_count == 1)
            {
                row = mysql_fetch_row(res);
                UserID = row[0]; 
                string position = row[1];

                if (position == "Manager")
                {
                    role = 3;
                    Manager();
                }
                else
                    Staff();
            }
            else
            {
                string checkUsername_query = "SELECT AdminID, Position FROM Admin WHERE Username ='" + username + "' AND Active = TRUE";
                const char* q = checkUsername_query.c_str();
                qstate = mysql_query(conn, q);
                res = mysql_store_result(conn);

                if (res->row_count == 1)
                {
                    cout << "\n\n\t\t\tInvalid password...." << endl;
                }
                else
                {
                    string checkPassword_query = "SELECT AdminID, Position FROM Admin WHERE Password ='" + password + "' AND Active = TRUE";
                    const char* p = checkPassword_query.c_str();
                    qstate = mysql_query(conn, p);
                    res = mysql_store_result(conn);

                    if (res -> row_count != 0)
                        cout << "\n\n\t\t\tInvalid username...." << endl;
                    else
                        cout << "\n\n\t\t\tInvalid username and password...." << endl;
                }

                cout << "\n\t\t1. Login again" << endl;
                cout << "\n\t\t2. Register" << endl;
                cout << "\n\t\t3. Back" << endl;
                cout << "\n\t\tChoose option (1-3): ";
                cin >> option;

                switch (option)
                {
                case 1:
                    Login();
                    break;
                case 2:
                    Register();
                    break;
                case 3:
                    main();
                    break;
                default:
                    cout << "\n\t\t\tPlease choose between 1 - 3. Press Enter To Continue...";
                    _getch();
                    main();
                }
            }
        }
        else
            cout << "Query Execution Problem!" << mysql_errno(conn) << endl;

    }
    else if (role == 2)
    {
        string checkUser_query = "SELECT CustomerID from Customer WHERE Username = '" + username + "' and Password = '" + password + "' AND Active = TRUE";
        const char* cu = checkUser_query.c_str();
        qstate = mysql_query(conn, cu);

        if (!qstate)
        {
            res = mysql_store_result(conn);
            if (res->row_count == 1)
            {
                row = mysql_fetch_row(res);
                UserID = row[0];
                if (attrPurchase == "")
                    Customer();
                else
                    Purchase();
            }
            else
            {
                string checkUsername_query = "SELECT CustomerID from Customer WHERE Username ='" + username + "' AND Active = TRUE";
                const char* q = checkUsername_query.c_str();
                qstate = mysql_query(conn, q);
                res = mysql_store_result(conn);

                if (res->row_count == 1)
                {
                    cout << "\n\n\t\t\tInvalid password...." << endl;
                }
                else
                {
                    string checkPassword_query = "SELECT CustomerID from Customer WHERE Password ='" + password + "' AND Active = TRUE";
                    const char* p = checkPassword_query.c_str();
                    qstate = mysql_query(conn, p);
                    res = mysql_store_result(conn);

                    if (res->row_count != 0)
                        cout << "\n\n\t\t\tInvalid username...." << endl;
                    else
                        cout << "\n\n\t\t\tInvalid username and password...." << endl;
                }
                cout << "\n\t\t1. Login again" << endl;
                cout << "\n\t\t2. Register" << endl;
                cout << "\n\t\t3. Back" << endl;
                cout << "\n\t\tChoose option (1-3): ";
                cin >> option;

                switch (option)
                {
                case 1:
                    Login();
                    break;
                case 2:
                    Register();
                    break;
                case 3:
                    main();
                    break;
                default:
                    cout << "\n\t\t\tPlease choose between 1 - 3. Press Enter To Continue...";
                    _getch();
                    main();
                }
            }
        }
        else
            cout << "Query Execution Problem!" << mysql_errno(conn) << endl;

    }
}

void ViewAttraction()
{
    qstate = mysql_query(conn, "SELECT a.AttractionID, a.Name, TIME_FORMAT(a.Open_Time, '%H:%i'), TIME_FORMAT(a.Close_Time, '%H:%i'), FORMAT(MIN(p.PricePerUnit), 2) FROM Attraction a INNER JOIN Price p ON a.AttractionID = p.AttractionID GROUP BY a.AttractionID");

    if (!qstate)
    { 
        cout << "\t\t\t" << setfill('_') << setw(120) << "_" << endl;
        cout << "\t\t\t" << setfill(' ') << setw(4) << "ID" << setw(50) << "Attraction" << setw(20) << "Opening Time" << setw(20) << "Closing Time" << setw(20) << "Price" << endl;
        cout << "\t\t\t" << setfill('_') << setw(120) << "_" << endl;

        res = mysql_store_result(conn);
        while (row = mysql_fetch_row(res))
        {
            cout << "\t\t\t" << setfill(' ') << setw(4) << row[0] << setw(50) << row[1] << setw(20) << row[2] << setw(20) << row[3] << setw(20) << "From RM " << row[4] << endl;
        }
    }
    else
        cout << "Query Execution Problem!" << mysql_errno(conn) << endl;
}

void Attraction()
{
    system("cls");
    int menu;
    string attrID;

    Header();

    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 249);
    cout << "\t\t\t\t" << setfill('*') << setw(100) << "*" << endl;
    cout << setfill(' ') << setw(90) << "Attraction Menu" << endl;
    cout << "\t\t\t\t" << setfill('*') << setw(100) << "*" << endl << endl;

    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 240);
    ViewAttraction();

    cout << "\n\n\t\t1. More detail about attraction" << endl << endl;
    cout << "\t\t2. Search Attraction" << endl << endl;
    cout << "\t\t3. Suggestion of Attraction" << endl << endl;
    cout << "\t\t4. Back" << endl << endl;
    cout << "\t\tChoose Option (1-4): ";
    cin >> menu;

    if (menu == 1)
    {
        cout << "\n\t\tChoose ID: ";
        cin.ignore(1, '\n');
        getline(cin,attrID);

        //to check Attr ID exist or not
        string checkAttr_query = "SELECT * FROM Attraction WHERE AttractionID = '" + attrID + "'";
        const char* cu = checkAttr_query.c_str();
        qstate = mysql_query(conn, cu);
        res = mysql_store_result(conn);
        row = mysql_fetch_row(res);

        if (res->row_count == 1)
            DetailView(attrID);
        else
        {
            cout << "\n\t\t\tID does not exist. Press Enter To Continue...";
            _getch(); //get char - pause console
            Attraction();
        }
    }
    else if (menu == 2)
        Searching();
    else if (menu == 3)
        Suggestion();
    else if (menu == 4)
    {
        if (role == 0)
            main();
        else
            Customer();
    }
    else
    {
        cout << "\n\t\t\tPlease choose between 1 - 4. Press Enter To Continue...";
        _getch(); //get char - pause console
        Attraction();
    }
}

void DetailView(string AttrID)
{
    system("cls");

    int option;

    Header();

    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 249);
    cout << "\t\t\t\t" << setfill('*') << setw(100) << "*" << endl;
    cout << setfill(' ') << setw(90) << "Attraction Menu (Detail)" << endl;
    cout << "\t\t\t\t" << setfill('*') << setw(100) << "*" << endl << endl;

    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 240);
    DetailAttraction(AttrID);

    if (role == 2 || role == 0)
    {
        cout << "\n\t\t1. Purchase Ticket" << endl;
        cout << "\n\t\t2. Back" << endl;
        cout << "\n\t\tChoose option (1/2): ";
        cin >> option;

        if (option == 1)
        {
            if (role == 0)
            {
                cout << "\n\n\t\t\tYou need to login first before purchase...." << endl << endl;
                cout << "\t\t1. Back" << endl << endl;
                cout << "\t\t2. Login" << endl << endl;
                cout << "\t\t3. Register" << endl << endl;
                cout << "\t\tChoose Option (1-3): ";
                cin >> option;

                switch (option)
                {
                case 1:
                    Attraction();
                    break;
                case 2:
                    Login();
                    break;
                case 3:
                    Register();
                    break;
                default:
                    cout << "\n\t\t\tPlease choose between 1 - 3. Press Enter To Continue...";
                    _getch(); //get char - pause console
                    DetailView(AttrID);
                    break;
                }
            }
            else
                Purchase();
        }
        else if (option == 2)
            Attraction();
        else
        {
            cout << "\n\t\t\tPlease choose between 1 or 2. Press Enter to Continue...";
            _getch();
            DetailView(AttrID);
        }
    }
    else
    {
        cout << "\n\n\t\tPress Enter to back....";
        _getch();
        AttrAdmin();
    }

}

void DetailAttraction(string AttrID)
{
    attrPurchase = AttrID;

    string attrDetail_query = "SELECT a.Name, TIME_FORMAT(a.Open_Time, '%H:%i'), TIME_FORMAT(a.Close_Time, '%H:%i'), a.Location, a.Description, c.Category FROM Attraction a INNER JOIN attrcategory c ON a.CategoryID = c.CategoryID  WHERE a.AttractionID = '" + AttrID +"'";
    const char* q = attrDetail_query.c_str();
    qstate = mysql_query(conn, q);
    res = mysql_store_result(conn);
    row = mysql_fetch_row(res);
    cout << "\n\n\t\tAttraction Name    : " << row[0] << endl;
    cout << "\t\tOpening Time\t   : " << row[1] << endl;
    cout << "\t\tClosing Time\t   : " << row[2] << endl;
    cout << "\t\tAttraction Location: " << row[3] << endl << endl;
    cout << "\t\tAttraction Category: "<< row[5] << endl << endl;
    cout << "\t\tDescription of the Attraction:" << endl;
    cout << "\t\t" << row[4] << endl;

    cout << "\n\n\t\tList of price" << endl << endl;

    string attrPrice_query = "SELECT Price_Category, FORMAT(PricePerUnit,2) FROM Price WHERE AttractionID = '" + AttrID + "'";
    const char* r = attrPrice_query.c_str();
    qstate = mysql_query(conn, r);
    res = mysql_store_result(conn);

    cout << "\t\t\t" << setfill('_') << setw(60) << "_" << endl;
    cout << "\t\t\t" << setfill(' ') << setw(40) << "Category" << setw(20) << "Price (RM)" << endl;
    cout << "\t\t\t" << setfill('_') << setw(60) << "_" << endl;

    while (row = mysql_fetch_row(res))
    {
        cout << "\t\t\t" << setfill(' ') << setw(40) << row[0] << setw(20) << row[1] << endl;
    }

    
}

void Suggest()
{
    system("cls");

    int choose;
    string age, category, catName;
    
    if (list.head != NULL)
    {
        list.attrList = "";
        list.deleteNode();
    }

    Header();
    HeaderSuggest();

    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 240);
    cout << "\t\t1. Suggest by age" << endl << endl;
    cout << "\t\t2. Suggest by category" << endl << endl;
    cout << "\t\tChoose option (1/2): ";
    cin >> choose;

    if (choose == 1)
    {
        cout << "\n\n\t\tInsert age: ";
        cin >> age;

        string checkAge_query = "SELECT a.AttractionID, a.Name, TIME_FORMAT(a.Open_Time, '%H:%i'), TIME_FORMAT(a.Close_Time, '%H:%i'), MIN(FORMAT(p.PricePerUnit, 2)) FROM attrCategory c INNER JOIN Attraction a ON c.CategoryID = a.CategoryID INNER JOIN Price p ON a.AttractionID = p.AttractionID WHERE c.Min_Age <= '" + age + "' AND c.Max_Age >= '" + age + "' GROUP BY a.AttractionID";
        const char* cu = checkAge_query.c_str(); // convert the query into char
        qstate = mysql_query(conn, cu); // execute query

        if (!qstate)
        {
            system("cls");
            Header();
            HeaderSuggest();
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 240);

            cout << "\n\t\t\t\t\t\t\t\tAttraction suit for " << age << " years old and above" << endl << endl;

            cout << "\t\t\t" << setfill('_') << setw(110) << "_" << endl;
            cout << "\t\t\t" << setfill(' ') << setw(4) << "ID" << setw(30) << "Attraction" << setw(20) << "Opening Time" << setw(20) << "Closing Time" << setw(20) << "Price" << endl;
            cout << "\t\t\t" << setfill('_') << setw(110) << "_" << endl;

            res = mysql_store_result(conn);
            while (row = mysql_fetch_row(res))
            {
                cout << "\t\t\t" << setfill(' ') << setw(4) << row[0] << setw(30) << row[1] << setw(20) << row[2] << setw(20) << row[3] << setw(20) << "From RM " << row[4] << endl;
                list.addNode(row[0]);
            }
            list.display();
        }
        else
            cout << "Query Execution Problem!" << mysql_errno(conn) << endl;
    }
    else if (choose == 2)
    {
        cout << "\n\n\t\tList of category" << endl << endl;
        cout << "\t\t1. Zoo" << endl << endl;
        cout << "\t\t2. Museum" << endl << endl;
        cout << "\t\t3. Theme Park" << endl << endl;
        cout << "\t\t4. Adventure" << endl << endl;
        cout << "\t\tChoose category (1-4): ";
        cin >> category;

        while (category < "1" || category > "4")
        {
            cout << "\n\t\t\tCategory not exits. Please choose difference category (1-4)." << endl << endl;
            cout << "\t\tChoose category (1-4): ";
            cin >> category;
        }

        if (category == "1")
            catName = "Zoo";
        else if (category == "2")
            catName = "Museum";
        else if (category == "3")
            catName = "Theme Park";
        else
            catName = "Advanture";

        string checkCategory_query = "SELECT a.AttractionID, a.Name, TIME_FORMAT(a.Open_Time, '%H:%i'), TIME_FORMAT(a.Close_Time, '%H:%i'), MIN(FORMAT(p.PricePerUnit, 2)) FROM Attraction a INNER JOIN Price p ON a.AttractionID = p.AttractionID WHERE a.CategoryID = " + category + " GROUP BY a.AttractionID";
        const char* cu = checkCategory_query.c_str(); // convert the query into char
        qstate = mysql_query(conn, cu); // execute query

        if (!qstate)
        {
            system("cls");
            Header();
            HeaderSuggest();
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 240);

            cout << "\n\t\t\t\t\t\t\t\tAttraction for " << catName << " category" << endl << endl;
            cout << "\t\t\t" << setfill('_') << setw(110) << "_" << endl;
            cout << "\t\t\t" << setfill(' ') << setw(4) << "ID" << setw(30) << "Attraction" << setw(20) << "Opening Time" << setw(20) << "Closing Time" << setw(20) << "Price" << endl;
            cout << "\t\t\t" << setfill('_') << setw(110) << "_" << endl;

            res = mysql_store_result(conn);
            while (row = mysql_fetch_row(res))
            {
                cout << "\t\t\t" << setfill(' ') << setw(4) << row[0] << setw(30) << row[1] << setw(20) << row[2] << setw(20) << row[3] << setw(20) << "From RM " << row[4] << endl;
            }
        }
        else
            cout << "Query Execution Problem!" << mysql_errno(conn) << endl;
    }
    else
    {
        cout << "\n\t\t\tPlease choose between 1 or 2. Press Enter To Continue...";
        _getch();
        Suggestion();
    }

}

void Suggestion()
{
    int option, menu;
    string attrID;

    Suggest();

    cout << "\n\n\t\t1. More detail about attraction" << endl << endl;
    cout << "\t\t2. Suggest other Attraction" << endl << endl;
    cout << "\t\t3. Back" << endl << endl;
    cout << "\t\tChoose option: ";
    cin >> option;

    if (option == 1)
    {
        cout << "\n\t\tChoose ID: ";
        cin.ignore(1, '\n');
        getline(cin, attrID);

        //to check Attr ID exist or not
        string checkAttr_query = "SELECT * FROM Attraction WHERE AttractionID = '" + attrID + "' IN (" + list.attrList + ")";
        const char* cu = checkAttr_query.c_str();
        qstate = mysql_query(conn, cu);
        res = mysql_store_result(conn);
        row = mysql_fetch_row(res);

        if (res->row_count == 1)
            DetailView(attrID);
        else
        {
            cout << "\n\t\t\tID does not exist. Press Enter To Continue...";
            _getch(); //get char - pause console
            Attraction();
        }
    }
    else if (option == 2)
        Suggestion();
    else if (option == 3)
        Attraction();
    else
    {
        cout << "\n\t\t\tPlease choose between 1 - 3. Press Enter To Continue...";
        _getch(); //get char - pause console
        Attraction();
    }
}

void Search(string AttrName)
{
    if (list.head != NULL)
    {
        list.attrList = "";
        list.deleteNode();
    } 

    string checkAttr_query = "SELECT a.AttractionID, a.Name, TIME_FORMAT(a.Open_Time, '%H:%i'), TIME_FORMAT(a.Close_Time, '%H:%i'), FORMAT(MIN(p.PricePerUnit), 2) FROM Attraction a INNER JOIN Price p ON a.AttractionID = p.AttractionID WHERE a.Name LIKE '%" + AttrName + "%' GROUP BY a.AttractionID";
    //cout << checkAttr_query;
    const char* cu = checkAttr_query.c_str(); // convert the query into char
    qstate = mysql_query(conn, cu);
    res = mysql_store_result(conn);

    if (!qstate)
    {
        if (res->row_count > 0)
        {
            cout << "\t\t\t" << setfill('_') << setw(120) << "_" << endl;
            cout << "\t\t\t" << setfill(' ') << setw(4) << "ID" << setw(50) << "Attraction" << setw(20) << "Opening Time" << setw(20) << "Closing Time" << setw(20) << "Price" << endl;
            cout << "\t\t\t" << setfill('_') << setw(120) << "_" << endl;

            while (row = mysql_fetch_row(res))
            {
                cout << "\t\t\t" << setfill(' ') << setw(4) << row[0] << setw(50) << row[1] << setw(20) << row[2] << setw(20) << row[3] << setw(20) << "From RM " << row[4] << endl;
                list.addNode(row[0]);
            }
            list.display();
        }
        else
        {
            cout << "\n\t\t\tAttraction " << AttrName << " cannot be found. Press Enter to continue";
            _getch();
            Searching();
            
        }
        
    }
    else
    {
        cout << "Query Execution Problem!" << mysql_errno(conn) << endl;
    }
        
}

void Searching()
{
    system("cls");

    int choose, option, menu;
    string AttrName, attrID;

    Header();

    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 249);
    cout << "\t\t\t\t" << setfill('*') << setw(100) << "*" << endl;
    cout << setfill(' ') << setw(95) << "Attraction Menu (Search)" << endl;
    cout << "\t\t\t\t" << setfill('*') << setw(100) << "*" << endl << endl;

    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 240);
    cout << "\n\t\tAttraction Name: ";
    cin.ignore(1, '\n');
    getline(cin, AttrName);

    Search(AttrName);

    cout << "\n\n\t\t1. More detail about attraction" << endl << endl;
    cout << "\t\t2. Search another attraction" << endl << endl;
    cout << "\t\t3. Back" << endl << endl;
    cout << "\t\tChoose Option (1-3): ";
    cin >> option;

    if (option == 1)
    {
        cout << "\n\t\tChoose ID: ";
        cin.ignore(1, '\n');
        getline(cin, attrID);

        //to check Attr ID exist or not
        string checkAttr_query = "SELECT * FROM Attraction WHERE AttractionID = '" + attrID +  "' IN (" + list.attrList + ")";
        const char* cu = checkAttr_query.c_str();
        qstate = mysql_query(conn, cu);
        res = mysql_store_result(conn);
        row = mysql_fetch_row(res);

        if (res->row_count == 1)
            DetailView(attrID);
        else
        {
            cout << "\n\t\t\tID does not exist. Press Enter To Continue...";
            _getch(); //get char - pause console
            if (role == 0 || role == 2)
                Attraction();
            else
                AttrAdmin();
        }
    }
    else if (option == 2)
        Searching();
    else if (option == 3)
    {
        if (role == 0 || role == 2)
            Attraction();
        else
            AttrAdmin();
    }
    else
    {
        cout << "\n\t\t\tPlease choose between 1 - 3. Press Enter To Continue...";
        _getch(); //get char - pause console
        Searching();
    }

}

void Staff()
{
    system("cls");

    int menu, option;

    Header();

    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 249);
    cout << "\t\t\t\t" << setfill('*') << setw(100) << "*" << endl;
    cout << setfill(' ') << setw(90) << "Staff Page" << endl;
    cout << "\t\t\t\t" << setfill('*') << setw(100) << "*" << endl << endl;
    
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 240);
    cout << "\n\t\t1. Profile Menu" << endl << endl;
    cout << "\t\t2. Attraction Menu" << endl << endl;
    cout << "\t\t3. Logout" << endl << endl;
    cout << "\t\tChoose menu (1-3): ";
    cin >> menu;

    switch (menu)
    {
    case 1:
        Profile();
        break;
    case 2:
        AttrAdmin();
        break;
    case 3:
        main();
        break;
    default:
        cout << "\n\t\t\tPlease choose between 1 - 3. Press Enter To Continue...";
        _getch(); //get char - pause console
        Manager();
        break;
    }
}

void Manager()
{
    system("cls");

    int menu;

    Header();

    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 249);
    cout << "\t\t\t\t" << setfill('*') << setw(100) << "*" << endl;
    cout << setfill(' ') << setw(90) << "Manager Page" << endl;
    cout << "\t\t\t\t" << setfill('*') << setw(100) << "*" << endl << endl;

    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 240);
    cout << "\n\t\t1. Profile Menu" << endl << endl;
    cout << "\t\t2. Staff Menu" << endl << endl;
    cout << "\t\t3. Customer Menu" << endl << endl;
    cout << "\t\t4. Attraction Menu" << endl<< endl;
    cout << "\t\t5. Report Menu" << endl << endl;
    cout << "\t\t6. Logout" << endl << endl;
    cout << "\t\tChoose menu (1-6): ";
    cin >> menu;

    switch (menu)
    {
    case 1:
        Profile();
        break;
    case 2:
        ViewStaff();
        break;
    case 3:
        ViewCustomer();
        break;
    case 4:
        AttrAdmin();
        break;
    case 5:
        Report();
        break;
    case 6:
        main();
        break;
    default:
        cout << "\n\t\t\tPlease choose between 1 - 6. Press Enter To Continue...";
        _getch(); //get char - pause console
        Manager();
        break;
    }

}

void Customer()
{
    system("cls");

    int menu;

    Header();

    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 249);
    cout << "\t\t\t\t" << setfill('*') << setw(100) << "*" << endl;
    cout << setfill(' ') << setw(90) << "Customer Page" << endl;
    cout << "\t\t\t\t" << setfill('*') << setw(100) << "*" << endl << endl;

    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 240);
    cout << "\n\t\t1. Profile Menu" << endl << endl;
    cout << "\t\t2. Attraction Menu" << endl << endl;
    cout << "\t\t3. Purchase Menu" << endl << endl;
    cout << "\t\t4. Logout" << endl << endl;
    cout << "\t\tChoose menu: ";
    cin >> menu;

    switch (menu)
    {
    case 1:
        Profile();
        break;
    case 2:
        Attraction();
        break;
    case 3:
        Purchase();
        break;
    case 4:
        main();
        break;
    default:
        cout << "\n\t\t\tPlease choose between 1 - 6. Press Enter To Continue...";
        _getch(); //get char - pause console
        Customer();
        break;
    }
}

void Profile()
{
    system("cls");

    int option;
    string data, attribute, deleteProfile_query;
    char confirm;

    Header();

    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 249);
    cout << "\t\t\t\t" << setfill('*') << setw(100) << "*" << endl;
    cout << setfill(' ') << setw(90) << "Profile Menu" << endl;
    cout << "\t\t\t\t" << setfill('*') << setw(100) << "*" << endl << endl;

    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 240);
    ProfileDetail();
        
    cout << "\n\n\t\t1. Update Detail" << endl << endl;
    cout << "\t\t2. Delete account" << endl << endl;
    cout << "\t\t3. Back" << endl << endl;
    cout << "\t\tChoose option (1-3): ";
    cin >> option;

    if (option == 1)
        EditProfile();
    else if (option == 2)
    {
        cout << "\n\t\tDo you confirm to delete your account? Enter (y/Y) to confirm. If not, Enter other character: ";
        cin >> confirm;

        if (confirm == 'y' || confirm == 'Y')
        {
            if (role == 1)
                deleteProfile_query = "UPDATE Admin SET Active = FALSE WHERE AdminID = '" + UserID + "'";
            else
                deleteProfile_query = "UPDATE Customer SET Active = FALSE WHERE CustomerID = '" + UserID + "'";

            const char* cu = deleteProfile_query.c_str();
            qstate = mysql_query(conn, cu);

            if (!qstate)
            {
                cout << "\n\t\t\tSuccessfully Delete Account. Press Enter to Continue...";
                _getch();
                main();
            }
            else
            {
                cout << "\n\t\t\tUnable to Delete Account. Press Enter to Continue...";
                _getch();
                Profile();
            }
        }
        else
            Profile();
    }
    else if (option == 3)
    {
        switch (role)
        {
        case 1:
            Staff();
            break;
        case 2:
            Customer();
            break;
        case 3:
            Manager();
            break;
        }
    }
    else
    {
        cout << "\n\t\t\tIncorrect input. Plase choose between 1 - 4. Press Enter to Continue...";
        _getch();
        Profile();
    }

}

void ProfileDetail()
{
    string User_query;

    if (role == 1 || role == 3)
    {
        User_query = "SELECT Name, StaffNo, Username, (YEAR(CURDATE())-YearOfBirth), Position, IF(Gender = 'M', 'Male', 'Female') FROM Admin WHERE AdminID ='" + UserID + "'";

        const char* cu = User_query.c_str();
        qstate = mysql_query(conn, cu);
        res = mysql_store_result(conn);
        row = mysql_fetch_row(res);

        cout << "\n\t\tName\t: " << row[0] << endl << endl;
        cout << "\t\tStaff No: " << row[1] << endl << endl;
        cout << "\t\tUsername: " << row[2] << endl << endl;
        cout << "\t\tAge\t: " << row[3] << endl << endl;
        cout << "\t\tGender  : " << row[5] << endl << endl;
        cout << "\t\tPosition: " << row[4] << endl << endl;
    }
    else if (role == 2)
    {
        User_query = "SELECT Name, Username, (YEAR(CURDATE())-YearOfBirth), IF(Gender = 'M', 'Male', 'Female'), IF(Citizens = TRUE, 'Malaysian Citizen', 'Not Malaysian Citizen'), State FROM Customer WHERE CustomerID ='" + UserID + "'";

        const char* cu = User_query.c_str();
        qstate = mysql_query(conn, cu);
        res = mysql_store_result(conn);
        row = mysql_fetch_row(res);

        cout << "\n\t\tName\t: " << row[0] << endl << endl;
        cout << "\t\tUsername: " << row[1] << endl << endl;
        cout << "\t\tAge\t: " << row[2] << endl << endl;
        cout << "\t\tGender  : " << row[3] << endl << endl;
        cout << "\t\tCitizen : " << row[4] << endl << endl;
        cout << "\t\tState   : " << row[5] << endl << endl;
    }
}

void EditProfile()
{
    system("cls");

    int option;
    string data, attribute, editProfile_query;
    char confirm;

    Header();

    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 249);
    cout << "\t\t\t\t" << setfill('*') << setw(100) << "*" << endl;
    cout << setfill(' ') << setw(100) << "Profile Menu (Update Detail)" << endl;
    cout << "\t\t\t\t" << setfill('*') << setw(100) << "*" << endl << endl;

    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 240);
    ProfileDetail();

    cout << "\n\n\t\t1. Name" << endl << endl;
    cout << "\t\t2. Password" << endl << endl;
    cout << "\t\t3. Age" << endl << endl;
    cout << "\t\t4. Gender" << endl << endl;

    if (role == 2)
    {
        cout << "\t\t5. Citizen" << endl << endl;
        cout << "\t\t6. State" << endl << endl;
        cout << "\t\t7. Back" << endl;
        cout << "\t\tChoose option (1-7): ";
    }
    else
    {
        cout << "\t\t5. Back" << endl << endl;
        cout << "\t\tChoose option (1-5): ";
    }
        
    cin >> option;

    cin.ignore(1, '\n');
    if (role == 2)
    {
        if (option == 1)
        {
            cout << "\n\t\tNew Name: ";
            getline(cin, data);
            attribute = "Name";
        }
        else if (option == 2)
        {
            cout << "\n\t\tNew Password: ";
            getline(cin, data);
            attribute = "Password";
        }
        else if (option == 3)
        {
            cout << "\t\tNew Year of Bitrh: ";
            getline(cin, data);

            string checkYear_query = "SELECT YEAR(CURDATE())";
            const char* c = checkYear_query.c_str(); // convert the query into char
            qstate = mysql_query(conn, c); // execute query
            res = mysql_store_result(conn);
            row = mysql_fetch_row(res);

            string curYear = row[0];
            int age = stoi(curYear) - stoi(data);

            if (data > curYear || data < "1900")
            {
                cout << "\n\t\t\tInvalid year. Press Enter to continue...";
                _getch();
                EditProfile();
            }
            attribute = "YearOfBirth";
        }
        else if (option == 4)
        {
            cout << "\t\tNew Gender (F/M): ";
            getline(cin, data);
            transform(data.begin(), data.end(), data.begin(), ::toupper);

            if (data != "M" && data != "F")
            {
                cout << "\n\t\t\tInvalid gender. Press Enter to continue...";
                _getch();
                EditProfile();
            }

            attribute = "Gender";
        }
        else if (option == 5)
        {
            cout << "\t\tNew Citizen (Y/N): ";
            getline(cin, data);

            if (data == "y" || data == "Y")
                data = "TRUE";
            else
                data = "FALSE";

            attribute = "Citizens";
        }
        else if (option == 6)
        {
            data = GetState();
            attribute = "State";
        }
        else if (option == 7)
            Customer();
        else
        {
            cout << "\n\t\t\tIncorrect input. Plase choose between 1 - 7. Press Enter to Continue...";
            _getch();
            EditProfile();
        }

        editProfile_query = "UPDATE Customer SET " + attribute + " = '" + data + "' WHERE CustomerID = '" + UserID + "'";
        const char* cu = editProfile_query.c_str();
        qstate = mysql_query(conn, cu);

        if (!qstate)
        {
            cout << "\n\t\t\tSuccessful Update. Press Enter to Continue...";
            _getch();
            EditProfile();
        }
        else
        {
            cout << "\n\t\t\tUnable to Update. Press Enter to Continue...";
            _getch();
            EditProfile();
        }
    }
    else
    {
        if (option == 1)
        {
            cout << "\n\t\tNew Name: ";
            getline(cin, data);
            attribute = "Name";
        }
        else if (option == 2)
        {
            cout << "\n\t\tNew Password: ";
            getline(cin, data);
            attribute = "Password";
        }
        else if (option == 3)
        {
            cout << "\t\tNew Year of Bitrh: ";
            getline(cin, data);

            string checkYear_query = "SELECT YEAR(CURDATE())";
            const char* c = checkYear_query.c_str(); // convert the query into char
            qstate = mysql_query(conn, c); // execute query
            res = mysql_store_result(conn);
            row = mysql_fetch_row(res);

            string curYear = row[0];
            int age = stoi(curYear) - stoi(data);

            if (data > curYear || data < "1900")
            {
                cout << "\n\t\t\tInvalid year. Press Enter to continue...";
                _getch();
                EditProfile();
            }

            if (age < 18)
            {
                cout << "\n\t\t\tToo young to be a staff. Press Enter to continue...";
                _getch();
                EditProfile();
            }
            attribute = "YearOfBirth";
        }
        else if (option == 4)
        {
            cout << "\t\tNew Gender (F/M): ";
            getline(cin, data);
            transform(data.begin(), data.end(), data.begin(), ::toupper);

            if (data != "M" && data != "F")
            {
                cout << "\n\t\t\tInvalid gender. Press Enter to continue...";
                _getch();
                EditProfile();
            }

            attribute = "Gender";
        }
        else if (option == 5)
        {
            if (role == 1)
                Staff();
            else
                Manager();
        }
        else
        {
            cout << "\n\t\t\tIncorrect input. Plase choose between 1 - 5. Press Enter to Continue...";
            _getch();
            EditProfile();
        }

        editProfile_query = "UPDATE Admin SET " + attribute + " = '" + data + "' WHERE AdminID = '" + UserID + "'";
        const char* cu = editProfile_query.c_str();
        qstate = mysql_query(conn, cu);

        if (!qstate)
        {
            cout << "\n\t\t\tSuccessful Update. Press Enter to Continue...";
            _getch();
            EditProfile();
        }
        else
        {
            cout << "\n\t\t\tUnable to Update. Press Enter to Continue...";
            _getch();
            EditProfile();
        }
    }
}

void AttrAdmin()
{
    system("cls");

    int menu;
    string attrID;

    Header();

    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 249);
    cout << "\t\t\t\t" << setfill('*') << setw(100) << "*" << endl;
    cout << setfill(' ') << setw(90) << "Attraction Menu" << endl;
    cout << "\t\t\t\t" << setfill('*') << setw(100) << "*" << endl << endl;

    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 240);
    ViewAttraction();

    cout << "\n\n\t\t1. More detail about attraction" << endl;
    cout << "\t\t2. Add New Attraction" << endl;
    cout << "\t\t3. Search Attraction" << endl;
    cout << "\t\t4. Edit Attraction" << endl;
    cout << "\t\t5. Delete Attraction" << endl;
    cout << "\t\t6. Back" << endl;
    cout << "\n\t\tChoose Option (1-6): ";
    cin >> menu;

    if (menu == 1)
    {
        cout << "\n\t\tChoose ID: ";
        cin.ignore(1, '\n');
        getline(cin, attrID);

        //to check Attr ID exist or not
        string checkAttr_query = "SELECT * FROM Attraction WHERE AttractionID = '" + attrID + "'";
        const char* cu = checkAttr_query.c_str();
        qstate = mysql_query(conn, cu);
        res = mysql_store_result(conn);
        row = mysql_fetch_row(res);

        if (res->row_count == 1)
            DetailView(attrID);
        else
        {
            cout << "\n\t\t\tID does not exist. Press Enter To Continue...";
            _getch(); //get char - pause console
            AttrAdmin();
        }
    }
    else if (menu == 2)
        AddAttraction();
    else if (menu == 3)
    {
        Searching();
    }
    else if (menu == 4)
    {
        cout << "\n\t\tChoose ID: ";
        cin.ignore(1, '\n');
        getline(cin, attrID);

        //to check Attr ID exist or not
        string checkAttr_query = "SELECT * FROM Attraction WHERE AttractionID = '" + attrID + "'";
        const char* cu = checkAttr_query.c_str();
        qstate = mysql_query(conn, cu);
        res = mysql_store_result(conn);
        row = mysql_fetch_row(res);

        if (res->row_count == 1)
            DetailEdit(attrID);
        else
        {
            cout << "\n\t\t\tID does not exist. Press Enter To Continue...";
            _getch(); //get char - pause console
            AttrAdmin();
        }
    }
    else if (menu == 5)
        DeleteAttraction();
    else if (menu == 6)
    {
        if (role == 1)
            Staff();
        else
            Manager();
    }  
    else
    {
        cout << "\n\t\t\tPlease choose between 1 or 6. Press Enter To Continue...";
        _getch(); //get char - pause console
        AttrAdmin();
    }
}

void AddAttraction()
{
    system("cls");
    
    string AttrName, Location, OpenTime, CloseTime, Description, Category;

    Header();

    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 249);
    cout << "\t\t\t\t" << setfill('*') << setw(100) << "*" << endl;
    cout << setfill(' ') << setw(90) << "Attraction Menu (Insert Data)" << endl;
    cout << "\t\t\t\t" << setfill('*') << setw(100) << "*" << endl << endl;

    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 240);
    cout << "\t\tAttraction Name\t\t    : ";
    cin.ignore(1, '\n');
    getline(cin, AttrName);
    cout << "\n\t\tLocation\t\t    : ";
    getline(cin, Location);
    cout << "\n\t\tOpening Time (24hrs, hh:mm) : ";
    getline(cin, OpenTime);
    size_t found = OpenTime.find(':');

    while (OpenTime < "00:00" || OpenTime > "23:59" || found == string::npos)
    {
        cout << "\n\t\t\tIncorrect format time. Insert again...";
        cout << "\t\tOpening Time (24hrs, hh:mm): ";
        getline(cin, OpenTime);
        found = OpenTime.find(':');
    }
   
    cout << "\n\t\tClosing Time (24hrs, hh:mm) : ";
    getline(cin, CloseTime);
    found = CloseTime.find(':');

    while (CloseTime < "00:00" || CloseTime > "23:59" || CloseTime < OpenTime || found == string::npos)
    {
        cout << "\n\t\t\tIncorrect format time. Insert again...";
        cout << "\t\tClosing Time (24hrs, hh:mm): ";
        getline(cin, CloseTime);
        size_t found = OpenTime.find(':');
    }

    cout << "\n\t\tDescription\t\t    : ";
    getline(cin, Description);

    cout << "\n\t\tAttraction Category" << endl;
    cout << "\t\t" << setfill('_') << setw(30) << "_" << endl;
    cout << "\t\t1. Zoo" << endl;
    cout << "\t\t2. Museum" << endl;
    cout << "\t\t3. Theme Park" << endl;
    cout << "\t\t4. Adventure" << endl;
    cout << "\t\tChoose Attraction Category (1-4): ";
    getline(cin, Category);

    while (Category < "1" || Category > "4")
    {
        cout << "\n\t\t\tChoose between 1 - 4. Choose again..." << endl;
        cout << "\t\tChoose Attraction Category (1-4): ";
        getline(cin, Category);
    }

    string insertAttr_query = "INSERT INTO Attraction (AdminID, CategoryID, Name, Location, Open_Time, Close_Time, Description) values ('" + UserID + "', '" + Category + "', '" + AttrName + "', '" + Location + "', '" + OpenTime + "', '" + CloseTime + "', '" + Description +"')";
    const char* q = insertAttr_query.c_str();
    qstate = mysql_query(conn, q);

    if (!qstate)
    {
        cout << "\n\t\t\tSuccessful Insert. Press Enter To Insert the Price...";
        _getch(); //get char - pause console
        AddPrice();
        
    }
    else
        cout << "Query Execution Problem!" << mysql_errno(conn) << endl;
}

void AddPrice()
{
    system("cls");

    char option;
    string PriceCategory, Price, AttrID;

    Header();

    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 249);
    cout << "\t\t\t\t" << setfill('*') << setw(100) << "*" << endl;
    cout << setfill(' ') << setw(88) << "Attraction Menu (Insert Data)" << endl;
    cout << "\t\t\t\t" << setfill('*') << setw(100) << "*" << endl << endl;

    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 240);
    string checkAttr_query = "SELECT a.AttractionID, a.Name, a.Open_Time, a.Close_Time,  a.Location, a.Description, c.Category FROM Attraction a, AttrCategory c WHERE a.CategoryID = c.CategoryID ORDER BY AttractionID DESC LIMIT 1";
    const char* cu = checkAttr_query.c_str();
    qstate = mysql_query(conn, cu);

    res = mysql_store_result(conn);
    row = mysql_fetch_row(res);
    AttrID = row[0];

    cout << "\n\n\t\tAttraction Name    : " << row[1] << endl;
    cout << "\t\tOpening Time\t   : " << row[2] << endl;
    cout << "\t\tClosing Time\t   : " << row[3] << endl;
    cout << "\t\tAttraction Location: " << row[4] << endl;
    cout << "\t\t" << row[5] << endl;

    string attrPrice_query = "SELECT Price_Category, FORMAT(PricePerUnit,2) FROM Price WHERE AttractionID = '" + AttrID + "'";
    const char* r = attrPrice_query.c_str();
    qstate = mysql_query(conn, r);
    res = mysql_store_result(conn);

    if (res->row_count > 0)
    {
        cout << "\n\n\t\tList of price" << endl << endl;

        cout << "\t\t\t" << setfill('_') << setw(60) << "_" << endl;
        cout << "\t\t\t" << setfill(' ') << setw(40) << "Category" << setw(20) << "Price (RM)" << endl;
        cout << "\t\t\t" << setfill('_') << setw(60) << "_" << endl;

        while (row = mysql_fetch_row(res))
        {
            cout << "\t\t\t" << setfill(' ') << setw(40) << row[0] << setw(20) << row[1] << endl;
        }

        cin.ignore(1, '\n');
    }

    cout << "\n\t\tPrice of Attraction by category" << endl;
    cout << "\t\tCategory (e.g Adult): ";
    //cin.ignore(1, '\n');
    getline(cin, PriceCategory);
    cout << "\t\tPrice: RM ";
    getline(cin, Price);

    string insertPrice_query = "INSERT INTO Price (AttractionID, Price_Category, PricePerUnit) VALUES ('" + AttrID + "', '" + PriceCategory + "', '" + Price + "')";
    const char* c = insertPrice_query.c_str();
    qstate = mysql_query(conn, c);

    if (!qstate)
    {
        cout << "\n\t\t\tSuccessful Insert Price. Do you want to insert another price? (Y/N): ";
        cin >> option;

        if (option == 'y' || option == 'Y')
            AddPrice();
        else
            AttrAdmin(); 
    }
    else
        cout << "Query Execution Problem!" << mysql_errno(conn) << endl;
}

void DetailEdit(string AttrID)
{
    system("cls");

    int option;
    string data, attribute, editAttr_query;
    size_t found;
    const char* cu;

    Header();

    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 249);
    cout << "\t\t\t\t" << setfill('*') << setw(100) << "*" << endl;
    cout << setfill(' ') << setw(90) << "Attraction Menu (Update Data)" << endl;
    cout << "\t\t\t\t" << setfill('*') << setw(100) << "*" << endl << endl;

    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 240);
    DetailAttraction(AttrID);

    cout << "\n\t\t1. Attraction Name" << endl;
    cout << "\t\t2. Location" << endl;
    cout << "\t\t3. Opening Time" << endl;
    cout << "\t\t4. Closing Time" << endl;
    cout << "\t\t5. Description" << endl;
    cout << "\t\t6. Attraction Category" << endl;
    cout << "\t\t7. Price" << endl;
    cout << "\t\t8. Back" << endl;
    cout << "\n\t\tChoose Option (1-8): ";
    cin >> option;

    cin.ignore(1, '\n');
    if (option == 1)
    {
        cout << "\n\t\tNew Attraction Name: ";
        getline(cin, data);

        editAttr_query = "UPDATE Attraction SET Name = '" + data + "' WHERE AttractionID = '" + AttrID + "'";
        cu = editAttr_query.c_str();
        qstate = mysql_query(conn, cu);

        if (!qstate)
        {
            cout << "\n\t\t\tSuccessfully Update Attraction Name. Press Enter to Continue...";
            _getch();
            DetailEdit(AttrID);
        }
        else
        {
            cout << "\n\t\t\tUnable to Update Attraction Name. Press Enter to Continue...";
            _getch();
            DetailEdit(AttrID);
        }

    }
    else if (option == 2)
    {
        cout << "\n\t\tNew Attraction Location: ";
        getline(cin, data);

        editAttr_query = "UPDATE Attraction SET Location = '" + data + "' WHERE AttractionID = '" + AttrID + "'";
        cu = editAttr_query.c_str();
        qstate = mysql_query(conn, cu);

        if (!qstate)
        {
            cout << "\n\t\t\tSuccessfully Update Attraction Location. Press Enter to Continue...";
            _getch();
            DetailEdit(AttrID);
        }
        else
        {
            cout << "\n\t\t\tUnable to Update Attraction Location. Press Enter to Continue...";
            _getch();
            DetailEdit(AttrID);
        }
    }
    else if (option == 3)
    {
        cout << "\n\t\tNew Opening Time (24hrs, hh:mm): ";
        getline(cin, data);

        found = data.find(':');
        if (data < "00:00" || data > "23:59" || found == string::npos)
        {
            cout << "\n\t\t\tIncorrect format time. Press Enter To Continue...";
            _getch(); //get char - pause console
            DetailEdit(AttrID);
        }

        editAttr_query = "UPDATE Attraction SET Open_Time = '" + data + "' WHERE AttractionID = '" + AttrID + "'";
        cu = editAttr_query.c_str();
        qstate = mysql_query(conn, cu);

        if (!qstate)
        {
            cout << "\n\t\t\tSuccessfully Update Attraction Opening Time. Press Enter to Continue...";
            _getch();
            DetailEdit(AttrID);
        }
        else
        {
            cout << "\n\t\t\tUnable to Update Attraction Opening Time. Press Enter to Continue...";
            _getch();
            DetailEdit(AttrID);
        }

    }
    else if (option == 4)
    {
        cout << "\n\t\tNew Closing Time (24hrs, hh:mm): ";
        getline(cin, data);

        string attrCheck_query = "SELECT Open_Time FROM Attraction WHERE AttractionID = '" + AttrID + "'";
        const char* q = attrCheck_query.c_str();
        qstate = mysql_query(conn, q);
        res = mysql_store_result(conn);
        row = mysql_fetch_row(res);

        string OpenTime = row[0];

        found = data.find(':');
        if (data < "00:00" || data > "23:59" || found == string::npos || data < row[0])
        {
            cout << "\n\t\t\tIncorrect format time. Press Enter To Continue...";
            _getch(); //get char - pause console
            DetailEdit(AttrID);
        }

        editAttr_query = "UPDATE Attraction SET Close_Time = '" + data + "' WHERE AttractionID = '" + AttrID + "'";
        cu = editAttr_query.c_str();
        qstate = mysql_query(conn, cu);

        if (!qstate)
        {
            cout << "\n\t\t\tSuccessfully Update Attraction Closing Time. Press Enter to Continue...";
            _getch();
            DetailEdit(AttrID);
        }
        else
        {
            cout << "\n\t\t\tUnable to Update Attraction Closing Time. Press Enter to Continue...";
            _getch();
            DetailEdit(AttrID);
        }
    }
    else if (option == 5)
    {
        cout << "\n\t\tNew Attraction Description: ";
        getline(cin, data);

        editAttr_query = "UPDATE Attraction SET Description = '" + data + "' WHERE AttractionID = '" + AttrID + "'";
        cu = editAttr_query.c_str();
        qstate = mysql_query(conn, cu);

        if (!qstate)
        {
            cout << "\n\t\t\tSuccessfully Update Attraction Description. Press Enter to Continue...";
            _getch();
            DetailEdit(AttrID);
        }
        else
        {
            cout << "\n\t\t\tUnable to Update Attraction Description. Press Enter to Continue...";
            _getch();
            DetailEdit(AttrID);
        }
    }
    else if (option == 6)
    {
        cout << "\n\t\t1. Zoo" << endl;
        cout << "\t\t2. Meseum" << endl;
        cout << "\t\t3. Theme park" << endl;
        cout << "\t\t4. Adventure" << endl;
        cout << "\n\t\tNew Attraction Category (1-4): ";
        getline(cin, data);

        editAttr_query = "UPDATE Attraction SET CategoryID = '" + data + "' WHERE AttractionID = '" + AttrID + "'";
        cu = editAttr_query.c_str();
        qstate = mysql_query(conn, cu);

        if (!qstate)
        {
            cout << "\n\t\t\tSuccessfully Update Attraction Description. Press Enter to Continue...";
            _getch();
            DetailEdit(AttrID);
        }
        else
        {
            cout << "\n\t\t\tUnable to Update Attraction Description. Press Enter to Continue...";
            _getch();
            DetailEdit(AttrID);
        }
    }
    else if (option == 7)
        EditPrice(AttrID);
    else if (option == 8)
        AttrAdmin();
    else
    {
        cout << "\n\t\t\tPlease choose between 1 - 8. Press Enter To Continue...";
        _getch(); //get char - pause console
        DetailEdit(AttrID);
    }

}

void EditPrice(string AttrID)
{
    system("cls");

    int option;
    string PriceID, data, editPrice_query;

    Header();

    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 249);
    cout << "\t\t\t\t" << setfill('*') << setw(100) << "*" << endl;
    cout << setfill(' ') << setw(90) << "Attraction Menu (Update Data)" << endl;
    cout << "\t\t\t\t" << setfill('*') << setw(100) << "*" << endl << endl;

    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 240);
    string attrDetail_query = "SELECT Name FROM Attraction WHERE AttractionID = '" + AttrID + "'";
    const char* q = attrDetail_query.c_str();
    qstate = mysql_query(conn, q);
    res = mysql_store_result(conn);
    row = mysql_fetch_row(res);

    cout << "\t\t\t" << row[0] << endl << endl;

    string attrPrice_query = "SELECT PriceID, Price_Category, FORMAT(PricePerUnit,2) FROM Price WHERE AttractionID = '" + AttrID + "'";
    const char* p = attrPrice_query.c_str();
    qstate = mysql_query(conn, p);
    res = mysql_store_result(conn);

    cout << "\t\t\t" << setfill('_') << setw(75) << "_" << endl;
    cout << "\t\t\t" << setfill(' ') << setw(8) << "ID" << setw(40) << "Category" << setw(20) << "Price (RM)" << endl;
    cout << "\t\t\t" << setfill('_') << setw(75) << "_" << endl;

    while (row = mysql_fetch_row(res))
    {
        cout << "\t\t\t" << setfill(' ') << setw(8) << row[0] << setw(40) << row[1] << setw(20) << row[2] << endl;
    }

    cout << "\n\t\t1. Edit price" << endl;
    cout << "\t\t2. Back" << endl;
    cout << "\t\tChoose Option(1/2): ";
    cin >> option;

    if (option == 1)
    {
        cout << "\n\t\tChoose ID: ";
        cin >> PriceID;

        //to check Attr ID exist or not
        string checkPrice_query = "SELECT * FROM Price WHERE PriceID = '" + PriceID + "' AND AttractionID = '" + AttrID + "'";
        const char* cu = checkPrice_query.c_str();
        qstate = mysql_query(conn, cu);
        res = mysql_store_result(conn);
        row = mysql_fetch_row(res);

        if (res->row_count == 1)
        {
            cout << "\n\t\t1. Price Category" << endl;
            cout << "\t\t2. Price" << endl;
            cout << "\t\tChoose Option(1/2): ";
            cin >> option;

            cin.ignore(1, '\n');
            if (option == 1)
            {
                cout << "\n\t\tNew Price Category: ";
                getline(cin, data);

                editPrice_query = "UPDATE Price SET Price_Category = '" + data + "' WHERE PriceID = '" + PriceID + "' AND AttractionID = '" + AttrID + "'" ;
                cu = editPrice_query.c_str();
                qstate = mysql_query(conn, cu);

                if (!qstate)
                {
                    cout << "\n\t\t\tSuccessfully Update Attraction Price Category. Press Enter to Continue...";
                    _getch();
                    EditPrice(AttrID);
                }
                else
                {
                    cout << "\n\t\t\tUnable to Update Attraction Price Category. Press Enter to Continue...";
                    _getch();
                    EditPrice(AttrID);
                }
            }
            else if (option == 2)
            {
                cout << "\n\t\tNew Price: RM ";
                getline(cin, data);

                editPrice_query = "UPDATE Price SET PricePerUnit = '" + data + "' WHERE PriceID = '" + PriceID + "' AND AttractionID = '" + AttrID + "'";
                cu = editPrice_query.c_str();
                qstate = mysql_query(conn, cu);

                if (!qstate)
                {
                    cout << "\n\t\t\tSuccessfully Update Attraction Price. Press Enter to Continue...";
                    _getch();
                    EditPrice(AttrID);
                }
                else
                {
                    cout << "\n\t\t\tUnable to Update Attraction Price. Press Enter to Continue...";
                    _getch();
                    EditPrice(AttrID);
                }
            }
            else
            {
                cout << "\n\t\t\tPlease choose between 1 or 2. Press Enter To Continue...";
                _getch(); //get char - pause console
                EditPrice(AttrID);
            }
        }
        else
        {
            cout << "\n\t\t\tID does not exist. Press Enter To Continue...";
            _getch(); //get char - pause console
            EditPrice(AttrID);
        }
    }
    else if (option == 2)
        DetailEdit(AttrID);
    else
    {
        cout << "\n\t\t\tPlease choose between 1 or 2. Press Enter To Continue...";
        _getch(); //get char - pause console
        EditPrice(AttrID);
    }

}

void DeleteAttraction()
{
    string AttrID;
    int option;
    char confirm;

    cout << "\n\t\tChoose ID: ";
    cin >> AttrID;

    //to check Attr ID exist or not
    string checkAttr_query = "SELECT * FROM Attraction WHERE AttractionID = '" + AttrID + "'";
    const char* cu = checkAttr_query.c_str();
    qstate = mysql_query(conn, cu);
    res = mysql_store_result(conn);
    row = mysql_fetch_row(res);

    if (res->row_count == 1)
    {
        cout << "\n\t\tDo you confirm to delete this attraction? Enter (y/Y) to confirm. If not, Enter other character: ";
        cin >> confirm;

        if (confirm == 'y' || confirm == 'Y')
        {
            string deletePrice_query = "DELETE FROM Price WHERE AttractionID = '" + AttrID + "'";
            const char* cu = deletePrice_query.c_str();
            qstate = mysql_query(conn, cu);

            if (!qstate)
            {
                string deleteAttr_query = "DELETE FROM Attraction WHERE AttractionID = '" + AttrID + "'";
                const char* ca = deleteAttr_query.c_str();
                qstate = mysql_query(conn, ca);

                if (!qstate)
                {
                    cout << "\n\t\t\tSuccessfully Delete Attraction. Press Enter to Continue...";
                    _getch();
                    AttrAdmin();
                }
                else
                    cout << "not success" << endl;

            }
            else
            {
                cout << "\n\t\t\tUnable to Delete Attraction. Press Enter to Continue...";
                _getch();
                AttrAdmin();
            }

        }
        else
            AttrAdmin();
    }
    else
    {
        cout << "\n\t\t\tID does not exist. Press Enter To Continue...";
        _getch(); //get char - pause console
        AttrAdmin();
    }

}

void Report()
{
    system("cls");

    int option, choose;

    Header();

    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 249);
    cout << "\t\t\t\t" << setfill('*') << setw(100) << "*" << endl;
    cout << setfill(' ') << setw(90) << "Report Menu" << endl;
    cout << "\t\t\t\t" << setfill('*') << setw(100) << "*" << endl << endl;

    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 240);

    cout << "\n\t\t1. Customer Report" << endl << endl; 
    cout << "\t\t2. Sales Report" << endl << endl;
    cout << "\t\t3. Back" << endl << endl;
    cout << "\t\tChoose option (1-3): ";
    cin >> option;

    if (option == 1)
    {
        cout << "\n\n\t\t1. By Age" << endl << endl;
        cout << "\t\t2. By State" << endl << endl;
        cout << "\t\t3. By Citizen" << endl << endl;
        cout << "\t\tChoose option (1-3): ";
        cin >> choose;

        switch (choose)
        {
        case 1:
            ReportAge();
            break;
        case 2:
            ReportState();
            break;
        case 3:
            ReportCitizen();
            break;
        default:
            cout << "\n\t\t\tPlease choose between 1 - 3. Press Enter To Continue...";
            _getch(); //get char - pause console
            Report();
        }

    }
    else if (option == 2)
    {
        cout << "\n\n\t\t1. Total Sales" << endl << endl;
        cout << "\t\t2. Total Ticket Sold" << endl << endl;
        cout << "\t\tChoose option (1/2): ";
        cin >> choose;

        switch (choose)
        {
        case 1:
            ReportSale();
            break;
        case 2:
            ReportSold();
            break;
        default:
            cout << "\n\t\t\tPlease choose between 1 or 2. Press Enter To Continue...";
            _getch(); //get char - pause console
            Report();
        }
    }
    else if (option == 3)
    {
        if (role == 1)
            Staff();
        else
            Manager();
    }
    else
    {
        cout << "\n\t\t\tPlease choose between 1 - 3. Press Enter To Continue...";
        _getch(); //get char - pause console
        Report();
    }
}

void Purchase()
{
    system("cls");

    int option;
    string catID, pax;
    double totalPrice;
    char choose;

    Header(); 

    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 249);
    cout << "\t\t\t\t" << setfill('*') << setw(100) << "*" << endl;
    cout << setfill(' ') << setw(90) << "Purchase Menu" << endl;
    cout << "\t\t\t\t" << setfill('*') << setw(100) << "*" << endl << endl;

    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 240);
    if (attrPurchase == "")
    {
        ViewAttraction();

        cout << "\n\t\tChoose ID: ";
        cin >> attrPurchase;

        string checkAttr_query = "SELECT * FROM Attraction WHERE AttractionID = '" + attrPurchase + "'";
        const char* cu = checkAttr_query.c_str();
        qstate = mysql_query(conn, cu);
        res = mysql_store_result(conn);
        row = mysql_fetch_row(res);

        if (res->row_count == 0)
        {
            attrPurchase = "";
            cout << "\n\t\t\tID does not exist. Press Enter To Continue...";
            _getch(); //get char - pause console
        }
        else
            Purchase();
    }

    if (purchaseID == "")
    {
        string insertPurchase_query = "INSERT INTO Purchase (CustomerID, AttractionID, Purchase_Date) VALUES ('" + UserID + "', '" + attrPurchase + "', NOW())";
        const char* c = insertPurchase_query.c_str();
        qstate = mysql_query(conn, c);

        if (!qstate)
        {
            string checkPurchase_query = "SELECT PurchaseID FROM Purchase ORDER BY PurchaseID DESC LIMIT 1";
            const char* cu = checkPurchase_query.c_str();
            qstate = mysql_query(conn, cu);
            res = mysql_store_result(conn);
            row = mysql_fetch_row(res);
            purchaseID = row[0];
        }
        else
        {
            cout << "\n\t\t\tUnable to Purchase Attraction. Press Enter to Continue...";
            _getch();
            Purchase();
        }
    }
    
    string attrDetail_query = "SELECT Name FROM Attraction WHERE AttractionID = '" + attrPurchase + "'";
    const char* q = attrDetail_query.c_str();
    qstate = mysql_query(conn, q);
    res = mysql_store_result(conn);
    row = mysql_fetch_row(res);

    cout << "\t\t\t" << row[0] << endl << endl;

    string attrPrice_query = "SELECT PriceID, Price_Category, FORMAT(PricePerUnit,2) FROM Price WHERE AttractionID = '" + attrPurchase + "'";
    const char* p = attrPrice_query.c_str();
    qstate = mysql_query(conn, p);
    res = mysql_store_result(conn);

    cout << "\t\t\t" << setfill('_') << setw(75) << "_" << endl;
    cout << "\t\t\t" << setfill(' ') << setw(8) << "ID" << setw(40) << "Category" << setw(20) << "Price (RM)" << endl;
    cout << "\t\t\t" << setfill('_') << setw(75) << "_" << endl;

    while (row = mysql_fetch_row(res))
    {
        cout << "\t\t\t" << setfill(' ') << setw(8) << row[0] << setw(40) << row[1] << setw(20) << row[2] << endl;
    }

    cout << "\n\t\tChoose Category by ID: ";
    cin >> catID;

    string checkCat_query = "SELECT PricePerUnit FROM Price WHERE AttractionID ='" + attrPurchase + "' AND PriceID = '" + catID + "'";
    const char* cu = checkCat_query.c_str();
    qstate = mysql_query(conn, cu);
    res = mysql_store_result(conn);
    row = mysql_fetch_row(res);

    if (res->row_count == 1)
    {
        string price = row[0];
        cout << "\t\tInsert Pax: ";
        cin >> pax;

        totalPrice = stod(pax) * stod(price);
        totalPayment += totalPrice;

        string insertPurchase_query = "INSERT INTO Payment (PurchaseID, PriceID, Pax, Total_Price) VALUES ('" + purchaseID + "', '" + catID + "', '" + pax + "', (" + pax + " * " + price + "))";
        //cout << insertPurchase_query;
        const char* q = insertPurchase_query.c_str();
        qstate = mysql_query(conn, q);

        if (!qstate)
        {
            cout << "\n\t\t\tAdd to Payment. Do you want to purchase another ticket for this attraction ? (Y/N): ";
            cin >> choose;

            if (choose == 'y' || choose == 'Y')
                Purchase();
            else
                Payment();
        }
        else
        {
            cout << "\n\t\t\tError. Press Enter To Continue...";
            _getch(); //get char - pause console
            Purchase();
        }
    }
    else
    {
        cout << "\n\t\t\tCategory ID does not exist. Press Enter To Continue...";
        _getch(); //get char - pause console
        Purchase();
    }

}

void Payment()
{
    system("cls");

    int option;
    string payment;
    char choose;

    Header();

    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 249);
    cout << "\t\t\t\t" << setfill('*') << setw(100) << "*" << endl;
    cout << setfill(' ') << setw(95) << "Payment & Receipt Menu" << endl;
    cout << "\t\t\t\t" << setfill('*') << setw(100) << "*" << endl << endl;

    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 240);
    payment = to_string(totalPayment);

    string editPrice_query = "UPDATE Purchase SET Total_Payment = '" + payment + "' WHERE PurchaseID = '" + purchaseID + "'";
    const char* cu = editPrice_query.c_str();
    qstate = mysql_query(conn, cu);

    if (!qstate)
    {
        string checkPurchase_query = "SELECT a.Name, a.Location, c.Name, DATE_FORMAT(p.Purchase_Date, '%d/%m/%Y'), DATE_FORMAT(p.Purchase_Date,  '%h:%i %p') FROM Purchase p INNER JOIN Attraction a ON a.AttractionID = p.AttractionID INNER JOIN Customer c ON c.CustomerID = p.CustomerID WHERE p.PurchaseID = '" + purchaseID + "'";
        const char* cu = checkPurchase_query.c_str();
        qstate = mysql_query(conn, cu);
        res = mysql_store_result(conn);
        row = mysql_fetch_row(res);

        cout << setfill(' ') << setw(45) << row[3] << setw(85) << row[4] << " " << endl << endl;
        cout << "\t\t\t\tCustomer Name: " << row[2] << endl << endl;
        cout << "\t\t\t\tAttraction Name: " << row[0] << endl << endl;
        cout << "\t\t\t\tLocation: " << row[1] << endl << endl << endl;

        string checkPayment_query = "SELECT py.Pax, pr.Price_Category, FORMAT(pr.PricePerUnit, 2), FORMAT(py.Total_Price,2) FROM Payment py INNER JOIN Price pr ON pr.PriceID = py.PriceID WHERE py.PurchaseID = '" + purchaseID + "'";
        const char* c = checkPayment_query.c_str();
        qstate = mysql_query(conn, c);
        res = mysql_store_result(conn);

        cout << "\t\t\t\t" << setfill('-') << setw(100) << "-" << endl;
        cout << "\t\t\t\t" << setfill(' ') << setw(4) << "Pax" << setw(40) << "Ticket Category" << setw(25) << "Price Per Pax (RM)" << setw(25) << "Total Price (RM)" << endl << endl;

        while (row = mysql_fetch_row(res))
        {
            cout << "\t\t\t\t" << setfill(' ') << setw(4) << row[0] << setw(40) << row[1] << setw(25) << row[2] << setw(25) << row[3] << endl;
        }

        cout << setprecision(2) << fixed;
        cout << endl << setfill(' ') << setw(105) << "Total Payment (RM)" << setw(20) << totalPayment << endl;
        cout << "\n\t\t\t\t" << setfill('-') << setw(100) << "-" << endl << endl;
        cout << setfill(' ') << setw(98) << "Thank you for using our kiosk" << endl << endl;
        cout << setfill(' ') << setw(95) << "Press Enter to proceed" << endl << endl;
        _getch();
        Customer();
    }
    else
    {
        cout << "\n\t\t\tUnable to Update Attraction Price Category. Press Enter to Continue...";
        _getch();
        //EditPrice(AttrID);
    }
}

void ViewStaff()
{
    system("cls");

    int option;
    string AdminID;

    Header();

    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 249);
    cout << "\t\t\t\t" << setfill('*') << setw(100) << "*" << endl;
    cout << setfill(' ') << setw(90) << "Staff Menu" << endl;
    cout << "\t\t\t\t" << setfill('*') << setw(100) << "*" << endl << endl;

    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 240);
    string User_query = "SELECT AdminID, Name, StaffNo, (YEAR(CURDATE())-YearOfBirth), Position, IF(Gender = 'M', 'Male', 'Female') FROM Admin";

    const char* cu = User_query.c_str();
    qstate = mysql_query(conn, cu);
    res = mysql_store_result(conn);
    
    cout << "\t\t\t" << setfill('_') << setw(120) << "_" << endl;
    cout << "\t\t\t" << setfill(' ') << setw(4) << "ID" << setw(40) << "Staff Name" << setw(20) << "Staff No." << setw(10) << "Age" << setw(20) << "Gender" << setw(20) << "Position" <<  endl;
    cout << "\t\t\t" << setfill('_') << setw(120) << "_" << endl;

    while (row = mysql_fetch_row(res))
    {
        cout << "\t\t\t" << setfill(' ') << setw(4) << row[0] << setw(40) << row[1] << setw(20) << row[2] << setw(10) << row[3] << setw(20) << row[5] << setw(20) << row[4] << endl;
    }

    cout << "\n\t\t1. Assign staff to manager" << endl;
    cout << "\t\t2. Back" << endl;
    cout << "\t\tChoose Option (1/2): ";
    cin >> option;

    if (option == 1)
    {
        cout << "\n\t\tChoose ID: ";
        cin >> AdminID;

        string checkUser_query = "SELECT * FROM Admin WHERE AdminID = '" + AdminID + "' AND Position = 'Staff'";
        const char* cu = checkUser_query.c_str(); // convert the query into char
        qstate = mysql_query(conn, cu); // execute query
        res = mysql_store_result(conn);

        if (res->row_count == 1)
        {
            string updateStaff_query = "UPDATE Admin SET Position = 'Manager' WHERE AdminID = '" + AdminID + "'";
            const char* cu = updateStaff_query.c_str();
            qstate = mysql_query(conn, cu);

            if (!qstate)
            {
                cout << "\n\t\t\tSuccessful Update. Press Enter to Continue...";
                _getch();
                ViewStaff();
            }
            else
            {
                cout << "\n\t\t\tUnable to Update. Press Enter to Continue...";
                _getch();
                ViewStaff();
            }
        }
        else
        {
            cout << "\n\t\t\tInvalid ID. Press Enter to Continue...";
            _getch();
            ViewStaff();
        }
    }
    else if (option == 2)
        Manager();
    else
    {
        cout << "\n\t\t\tPlease choose between 1 or 2. Press Enter To Continue...";
        _getch(); //get char - pause console
        ViewStaff();
    }
}

void ViewCustomer()
{
    system("cls");

    Header();

    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 249);
    cout << "\t\t\t\t" << setfill('*') << setw(100) << "*" << endl;
    cout << setfill(' ') << setw(90) << "Customer Menu" << endl;
    cout << "\t\t\t\t" << setfill('*') << setw(100) << "*" << endl << endl;

    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 240);
    string User_query = "SELECT Name, (YEAR(CURDATE())-YearOfBirth), IF(Gender = 'M', 'Male', 'Female'), IF(Citizens = TRUE, 'Malaysian Citizen', 'Not Malaysia Citizen'), State FROM Customer";

    const char* cu = User_query.c_str();
    qstate = mysql_query(conn, cu);
    res = mysql_store_result(conn);

    cout << "\t\t\t" << setfill('_') << setw(120) << "_" << endl;
    cout << "\t\t\t" << setfill(' ') << setw(25) << "Customer Name" << setw(10) << "Age" << setw(20) << "Gender" << setw(30) << "Citizen" << setw(25) << "State" << endl;
    cout << "\t\t\t" << setfill('_') << setw(120) << "_" << endl;

    while (row = mysql_fetch_row(res))
    {
        cout << "\t\t\t" << setfill(' ') << setw(25) << row[0] << setw(10) << row[1] << setw(20) << row[2] << setw(30) << row[3] << setw(25) << row[4] << endl;
    }

    cout << "\n\t\t\tPress Enter to Back...";
    _getch();
    Manager();
}

void ReportAge()
{
    system("cls");

    int n = 0;

    Header();

    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 249);
    cout << "\t\t\t\t" << setfill('*') << setw(100) << "*" << endl;
    cout << setfill(' ') << setw(100) << "Report Menu (Customer Age)" << endl;
    cout << "\t\t\t\t" << setfill('*') << setw(100) << "*" << endl << endl;
    
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 240);
    string attrDetail_query = "SELECT (YEAR(CURDATE()) - YearOfBirth) AS Age, COUNT(YearOfBirth) FROM Customer GROUP BY YearOfBirth ORDER BY(YEAR(CURDATE()) - YearOfBirth) ASC";
    const char* q = attrDetail_query.c_str();
    qstate = mysql_query(conn, q);
    res = mysql_store_result(conn);

    cout << "\t\t\t" << setfill(' ') << setw(20) << "Age" << setw(30) << "Number of Customer" << endl << endl;
    while (row = mysql_fetch_row(res))
    {
        int count;
        count = stoi(row[1]) * 4;
        n += 16;
        cout << "\t\t\t" << setfill(' ') << setw(20) << row[0] << "   ";
        for (int i = 0; i <= count; i++)
        {
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), n);
            cout << " ";
        }
        
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 240);
        cout << "  " << row[1];
        cout << "\n\n";
    }

    cout << "\n\t\t\tPress Enter to Back...";
    _getch();
    Report();
}

void ReportState()
{
    system("cls");

    int n = 0;

    Header();

    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 249);
    cout << "\t\t\t\t" << setfill('*') << setw(100) << "*" << endl;
    cout << setfill(' ') << setw(100) << "Report Menu (Customer State)" << endl;
    cout << "\t\t\t\t" << setfill('*') << setw(100) << "*" << endl << endl;

    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 240);
    string attrDetail_query = "SELECT State, COUNT(State) FROM Customer GROUP BY State ORDER BY State ASC";
    const char* q = attrDetail_query.c_str();
    qstate = mysql_query(conn, q);
    res = mysql_store_result(conn);

    cout << "\t\t\t" << setfill(' ') << setw(20) << "State" << setw(30) << "Number of Customer" << endl << endl;
    while (row = mysql_fetch_row(res))
    {
        int count;
        count = stoi(row[1]) * 4;
        n += 16;
        cout << "\t\t\t" << setfill(' ') << setw(20) << row[0] << "   ";
        for (int i = 0; i <= count; i++)
        {
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), n);
            cout << " ";
        }

        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 240);
        cout << "  " << row[1];
        cout << "\n\n";
    }
    cout << "\n\t\t\tPress Enter to Back...";
    _getch();
    Report();
}

void ReportCitizen()
{
    system("cls");

    int n = 0;

    Header();

    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 249);
    cout << "\t\t\t\t" << setfill('*') << setw(100) << "*" << endl;
    cout << setfill(' ') << setw(100) << "Report Menu (Customer Citizen)" << endl;
    cout << "\t\t\t\t" << setfill('*') << setw(100) << "*" << endl << endl;

    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 240);
    string attrDetail_query = "SELECT IF(Citizens = TRUE, 'Malaysian', 'Not Malaysian'), COUNT(Citizens) FROM Customer GROUP BY Citizens ORDER BY Citizens DESC";
    const char* q = attrDetail_query.c_str();
    qstate = mysql_query(conn, q);
    res = mysql_store_result(conn);

    cout << "\t\t\t" << setfill(' ') << setw(20) << "Citizen" << setw(30) << "Number of Customer" << endl << endl;
    while (row = mysql_fetch_row(res))
    {
        int count;
        count = stoi(row[1]) * 4;
        n += 16;
        cout << "\t\t\t" << setfill(' ') << setw(20) << row[0] << "   ";
        for (int i = 0; i <= count; i++)
        {
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), n);
            cout << " ";
        }

        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 240);
        cout << "  " << row[1];
        cout << "\n\n";
    }
    cout << "\n\t\t\tPress Enter to Back...";
    _getch();
    Report();
}

void ReportSale()
{
    system("cls");

    int n = 0;

    Header();

    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 249);
    cout << "\t\t\t\t" << setfill('*') << setw(100) << "*" << endl;
    cout << setfill(' ') << setw(100) << "Report Menu (Total Sales)" << endl;
    cout << "\t\t\t\t" << setfill('*') << setw(100) << "*" << endl << endl;

    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 240);
    string attrDetail_query = "SELECT a.Name, SUM(p.Total_Payment) FROM Purchase p INNER JOIN Attraction a ON a.AttractionID = p.AttractionID GROUP BY p.AttractionID ORDER BY a.Name ASC";
    const char* q = attrDetail_query.c_str();
    qstate = mysql_query(conn, q);
    res = mysql_store_result(conn);

    cout << "\t\t\t" << setfill(' ') << setw(30) << "Attraction" << setw(30) << "Total Sales (RM)" << endl << endl;
    while (row = mysql_fetch_row(res))
    {
        int count;
        count = stoi(row[1]) / 6;
        n += 16;
        cout << "\t\t\t" << setfill(' ') << setw(30) << row[0] << "   ";
        for (int i = 0; i <= count; i++)
        {
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), n);
            cout << " ";
        }

        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 240);
        cout << "  " << row[1];
        cout << "\n\n";
    }
    cout << "\n\t\t\tPress Enter to Back...";
    _getch();
    Report();
}

void ReportSold()
{
    system("cls");

    int n = 0;

    Header();

    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 249);
    cout << "\t\t\t\t" << setfill('*') << setw(100) << "*" << endl;
    cout << setfill(' ') << setw(100) << "Report Menu (Ticket Sold)" << endl;
    cout << "\t\t\t\t" << setfill('*') << setw(100) << "*" << endl << endl;

    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 240);
    string attrDetail_query = "SELECT a.Name, SUM(py.Pax) FROM Purchase pr INNER JOIN Payment py ON py.PurchaseID = pr.PurchaseID INNER JOIN Attraction a ON a.AttractionID = pr.AttractionID GROUP BY pr.AttractionID ORDER BY a.Name ASC";
    const char* q = attrDetail_query.c_str();
    qstate = mysql_query(conn, q);
    res = mysql_store_result(conn);

    cout << "\t\t\t" << setfill(' ') << setw(30) << "Attraction" << setw(30) << "Ticket Sold" << endl << endl;
    while (row = mysql_fetch_row(res))
    {
        int count;
        count = stoi(row[1]) * 2;
        n += 16;
        cout << "\t\t\t" << setfill(' ') << setw(30) << row[0] << "   ";
        for (int i = 0; i <= count; i++)
        {
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), n);
            cout << " ";
        }

        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 240);
        cout << "  " << row[1];
        cout << "\n\n";
    }
    cout << "\n\t\t\tPress Enter to Back...";
    _getch();
    Report();
}
