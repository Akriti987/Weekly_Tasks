#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <iomanip>
#include <sstream>
#include <fstream>
#include <limits>
#include <regex>

using namespace std;

// Define a class to represent a hotel room
class Room
{
public:
    string roomNumber;
    string roomType;
    double pricePerNight;

    Room(string roomNumber, string roomType, double pricePerNight)
    {
        this->roomNumber = roomNumber;
        this->roomType = roomType;
        this->pricePerNight = pricePerNight;
    }
};

// Define a class to represent a hotel booking
class Booking
{
public:
    string guestName;
    string roomNumber;
    string checkInDate;
    string checkOutDate;

    Booking(string guestName, string roomNumber, string checkInDate, string checkOutDate)
    {
        this->guestName = guestName;
        this->roomNumber = roomNumber;
        this->checkInDate = checkInDate;
        this->checkOutDate = checkOutDate;
    }
};

// Function to validate date format (YYYY-MM-DD)
bool isValidDate(const string &date)
{
    const regex pattern("^(\\d{4})-(0[1-9]|1[0-2])-(0[1-9]|[1-2][0-9]|3[0-1])$");
    return regex_match(date, pattern);
}

// Helper function to compare dates (YYYY-MM-DD format)
bool isDateBefore(const string &date1, const string &date2)
{
    int year1 = stoi(date1.substr(0, 4));
    int month1 = stoi(date1.substr(5, 2));
    int day1 = stoi(date1.substr(8, 2));

    int year2 = stoi(date2.substr(0, 4));
    int month2 = stoi(date2.substr(5, 2));
    int day2 = stoi(date2.substr(8, 2));

    if (year1 < year2)
    {
        return true;
    }
    else if (year1 == year2 && month1 < month2)
    {
        return true;
    }
    else if (year1 == year2 && month1 == month2 && day1 <= day2)
    {
        return true;
    }
    return false;
}

// Helper function to check for date overlap
bool isDateOverlap(const string &checkIn1, const string &checkOut1, const string &checkIn2, const string &checkOut2)
{
    return !(isDateBefore(checkOut1, checkIn2) || isDateBefore(checkOut2, checkIn1));
}

// Define a class to manage hotel operations
class Hotel
{
private:
    vector<Room> rooms;
    vector<Booking> bookings;
    string filename;

public:
    Hotel(string filename)
    {
        this->filename = filename;
        loadBookingsFromFile();
    }

    void addRoom(string roomNumber, string roomType, double pricePerNight)
    {
        rooms.push_back(Room(roomNumber, roomType, pricePerNight));
    }

    void displayAvailableRooms()
    {
        cout << "\n=================== Available Rooms ===================\n";
        for (Room room : rooms)
        {
            cout << "Room Number: " << room.roomNumber << endl;
            cout << "Room Type: " << room.roomType << endl;
            cout << "Price Per Night: Rs. " << fixed << setprecision(2) << room.pricePerNight << endl;
            cout << "----------------------------------------------------\n";
        }
    }

    bool bookRoom(string guestName, string roomNumber, string checkInDate, string checkOutDate)
    {
        for (Room &room : rooms)
        {
            if (room.roomNumber == roomNumber)
            {
                // Check for date overlap with existing bookings
                bool overlap = false;
                for (const Booking& booking : bookings)
                {
                    if (booking.roomNumber == roomNumber &&
                        isDateOverlap(checkInDate, checkOutDate, booking.checkInDate, booking.checkOutDate))
                    {
                        overlap = true;
                        break;
                    }
                }

        
                if (isDateBefore(checkOutDate, checkInDate))
                {
                    cout << "Invalid dates: Check-out date must be after check-in date.\n";
                    cout << "Enter Check-in Date (YYYY-MM-DD): ";
                    while (!(cin >> checkInDate) || !isValidDate(checkInDate))
                    {
                        cout << "Invalid Date Format. Please enter in YYYY-MM-DD format: ";
                        cin.clear();
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    }
                    cout << "Enter Check-out Date (YYYY-MM-DD): ";
                    while (!(cin >> checkOutDate) || !isValidDate(checkOutDate))
                    {
                        cout << "Invalid Date Format. Please enter in YYYY-MM-DD format: ";
                        cin.clear();
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    }
                    // Recursive call to check again
                    return bookRoom(guestName, roomNumber, checkInDate, checkOutDate);
                }

                if (!overlap)
                {
                    bookings.push_back(Booking(guestName, roomNumber, checkInDate, checkOutDate));
                    saveBookingsToFile();
                    return true;
                }
                else
                {
                    cout << "\nRoom is already booked for the specified dates.\n";
                    return false;
                }
            }
        }
        return false;
    }

    bool cancelBooking(string guestName, string roomNumber)
    {
        for (auto it = bookings.begin(); it != bookings.end(); ++it)
        {
            if (it->guestName == guestName && it->roomNumber == roomNumber)
            {
                bookings.erase(it);
                saveBookingsToFile();
                return true;
            }
        }
        return false;
    }

    void displayBookings()
    {
        cout << "\n=================== Booking History ===================\n";
        if (bookings.size() == 0)
        {
            cout << "No Bookings made as of now.\n";
        }
        else
        {
            for (Booking booking : bookings)
            {
                cout << "Guest Name: " << booking.guestName << endl;
                cout << "Room Number: " << booking.roomNumber << endl;
                cout << "Check-in Date: " << booking.checkInDate << endl;
                cout << "Check-out Date: " << booking.checkOutDate << endl;
                cout << "----------------------------------------------------\n";
            }
        }
    }

    void loadBookingsFromFile()
    {
           ifstream infile(filename);
        if (!infile.is_open()) { // Check if file exists and can be opened
            cout << "Warning: Unable to open bookings file. Starting with an empty booking list." << endl;
            return; // Exit the function if file cannot be opened
        }
        string line;
        while (getline(infile, line))
        {
            istringstream iss(line);
            string guestName, roomNumber, checkInDate, checkOutDate;
            getline(iss, guestName, ',');
            getline(iss, roomNumber, ',');
            getline(iss, checkInDate, ',');
            getline(iss, checkOutDate);
            bookings.push_back(Booking(guestName, roomNumber, checkInDate, checkOutDate));
        }
        infile.close();
    }

    void saveBookingsToFile()
    {
        ofstream outfile(filename);
        if (!outfile.is_open()) { // Check if file can be opened for writing
            cout << "Error: Unable to open bookings file for writing." << endl;
            return; // Exit the function if file cannot be opened
        }
        for (Booking booking : bookings)
        {
            outfile << booking.guestName << "," << booking.roomNumber << "," << booking.checkInDate << "," << booking.checkOutDate << endl;
        }
        outfile.close();
    }
};


int main()
{
    Hotel hotel("bookings.txt");

    // Add some sample rooms
    hotel.addRoom("101", "Single", 1000.0);
    hotel.addRoom("102", "Double", 1400.0);
    hotel.addRoom("103", "Suite", 2000.0);

    int choice;
    do
    {
        cout << "\n=================== Hotel Management System ===================\n\n";
        cout << "1. Display Available Rooms" << endl;
        cout << "2. Book a Room" << endl;
        cout << "3. Cancel a Booking" << endl;
        cout << "4. Display Booking History" << endl;
        cout << "5. Exit\n" << endl;
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice)
        {
        case 1:
            hotel.displayAvailableRooms();
            break;
        case 2:
        {
            string guestName, roomNumber, checkInDate, checkOutDate;
            cout << "\nEnter Guest Name: ";
            cin.ignore();
            getline(cin, guestName);
            cout << "Enter Room Number: ";
            cin >> roomNumber;
            cout << "Enter Check-in Date (YYYY-MM-DD): ";
            while (!(cin >> checkInDate) || !isValidDate(checkInDate))
            {
                cout << "Invalid Date Format. Please enter in YYYY-MM-DD format: ";
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            }
            cout << "Enter Check-out Date (YYYY-MM-DD): ";
            while (!(cin >> checkOutDate) || !isValidDate(checkOutDate))
            {
                cout << "Invalid Date Format. Please enter in YYYY-MM-DD format: ";
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            }
            if (hotel.bookRoom(guestName, roomNumber, checkInDate, checkOutDate))
            {
                cout << "Room booked successfully!\n";
            }
            break;
        }
        case 3:
        {
            string guestName, roomNumber;
            cout << "\nEnter Guest Name: ";
            cin.ignore();
            getline(cin, guestName);
            cout << "Enter Room Number: ";
            cin >> roomNumber;
            if (hotel.cancelBooking(guestName, roomNumber))
            {
                cout << "Booking cancelled successfully!\n";
            }
            else
            {
                cout << "Booking not found.\n";
            }
            break;
        }
        case 4:
            hotel.displayBookings();
            break;
        case 5:
            cout << "\nExiting...\n";
            break;
        default:
            cout << "Invalid choice.\n";
        }
    } while (choice != 5);

    return 0;
}