/*
 * Passive Cooling Home - Temperature Data Acquisition
 * Converted from the project pseudocode.
 *
 * Note:
 * This is a C++ implementation template of the pseudocode.
 * Hardware-specific sensor, RTC, and SD-card functions must be
 * connected to the actual modules/libraries used in the prototype.
 */

#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <chrono>
#include <thread>
#include <ctime>

using namespace std;

// -----------------------------
// Configuration
// -----------------------------
const string LOG_FILE = "temperature_log.txt";
const int SAMPLING_INTERVAL_SECONDS = 5;

// -----------------------------
// Function Prototypes
// -----------------------------
bool initializeTemperatureSensors();
bool initializeRTC();
bool initializeSDCard();

bool readTemperatureSensors(double &tempOutdoor, double &tempIndoor);
double calculateTemperatureDifference(double tempOutdoor, double tempIndoor);

string getCurrentTime();
void storeData(const string &time,
               double tempOutdoor,
               double tempIndoor,
               double temperatureDifference);

void systemInitialization();
void systemShutdown();

// -----------------------------
// System Initialization
// -----------------------------
void systemInitialization()
{
    cout << "Initializing Temperature Sensor 1..." << endl;
    cout << "Initializing Temperature Sensor 2..." << endl;
    cout << "Initializing RTC Module..." << endl;
    cout << "Initializing SD Card Module..." << endl;

    if (!initializeTemperatureSensors())
    {
        cerr << "Error: Temperature sensors not detected." << endl;
        exit(1);
    }

    if (!initializeRTC())
    {
        cerr << "Error: RTC module not detected." << endl;
        exit(1);
    }

    if (!initializeSDCard())
    {
        cerr << "Error: SD Card not found." << endl;
        exit(1);
    }

    // Create or open the temperature log file.
    ofstream file(LOG_FILE, ios::app);

    if (!file)
    {
        cerr << "Error: Unable to open temperature log file." << endl;
        exit(1);
    }

    file.close();

    cout << "System Ready" << endl;
}

// -----------------------------
// Temperature Data Acquisition
// -----------------------------
bool readTemperatureSensors(double &tempOutdoor, double &tempIndoor)
{
    /*
     * Replace these example readings with actual
     * temperature sensor code.
     *
     * Sensor 1 -> Outdoor temperature
     * Sensor 2 -> Indoor temperature
     */

    cout << "\nEnter Outdoor Temperature (°C): ";
    cin >> tempOutdoor;

    cout << "Enter Indoor Temperature (°C): ";
    cin >> tempIndoor;

    if (cin.fail())
    {
        cin.clear();
        cin.ignore(1000, '\n');
        return false;
    }

    return true;
}

// -----------------------------
// Temperature Difference
// -----------------------------
double calculateTemperatureDifference(double tempOutdoor,
                                      double tempIndoor)
{
    return tempOutdoor - tempIndoor;
}

// -----------------------------
// Get Current Time
// -----------------------------
string getCurrentTime()
{
    auto now = chrono::system_clock::now();
    time_t currentTime = chrono::system_clock::to_time_t(now);

    tm localTime{};

#ifdef _WIN32
    localtime_s(&localTime, &currentTime);
#else
    localtime_r(&currentTime, &localTime);
#endif

    char buffer[80];

    strftime(buffer, sizeof(buffer),
             "%Y-%m-%d %H:%M:%S",
             &localTime);

    return string(buffer);
}

// -----------------------------
// Data Logging to SD Card/File
// -----------------------------
void storeData(const string &time,
               double tempOutdoor,
               double tempIndoor,
               double temperatureDifference)
{
    ofstream file(LOG_FILE, ios::app);

    if (!file)
    {
        cerr << "Error: Unable to open temperature log file." << endl;
        return;
    }

    file << fixed << setprecision(2);
    file << time << " | "
         << "Outdoor: " << tempOutdoor << " °C | "
         << "Indoor: " << tempIndoor << " °C | "
         << "Difference: " << temperatureDifference << " °C"
         << endl;

    file.close();

    cout << "Data stored successfully." << endl;
}

// -----------------------------
// Main Program
// -----------------------------
int main()
{
    systemInitialization();

    char systemPower = 'Y';

    while (systemPower == 'Y' || systemPower == 'y')
    {
        double tempOutdoor = 0.0;
        double tempIndoor = 0.0;

        // Read temperature sensors
        if (!readTemperatureSensors(tempOutdoor, tempIndoor))
        {
            cerr << "Error: Invalid temperature reading." << endl;
            continue;
        }

        // Calculate temperature difference
        double temperatureDifference =
            calculateTemperatureDifference(
                tempOutdoor,
                tempIndoor
            );

        // Get current time
        string currentTime = getCurrentTime();

        // Store data
        storeData(
            currentTime,
            tempOutdoor,
            tempIndoor,
            temperatureDifference
        );

        cout << fixed << setprecision(2);
        cout << "Temperature Difference: "
             << temperatureDifference
             << " °C" << endl;

        cout << "\nContinue system? (Y/N): ";
        cin >> systemPower;

        /*
         * The project pseudocode specifies a sampling interval.
         * For a real hardware implementation, this delay can be
         * enabled according to the required sampling interval.
         */
        if (systemPower == 'Y' || systemPower == 'y')
        {
            cout << "Waiting for sampling interval..." << endl;
            this_thread::sleep_for(
                chrono::seconds(SAMPLING_INTERVAL_SECONDS)
            );
        }
    }

    systemShutdown();

    return 0;
}

// -----------------------------
// Hardware Initialization
// -----------------------------
// These functions are placeholders for the actual hardware
// initialization code used with the project's temperature sensors,
// RTC module, and SD card module.

bool initializeTemperatureSensors()
{
    return true;
}

bool initializeRTC()
{
    return true;
}

bool initializeSDCard()
{
    return true;
}

// -----------------------------
// System Shutdown
// -----------------------------
void systemShutdown()
{
    cout << "\nClosing files..." << endl;
    cout << "Stopping Temperature Sensors..." << endl;
    cout << "System Stopped" << endl;
}
