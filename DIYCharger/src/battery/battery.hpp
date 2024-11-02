/*---------------------------------------------------------------------------*\
    =====\\  || \\    //  |
    ||    \\ ||  \\  //   | Project: BatteryCharger using D1 Wemos
    ||    || ||   \\//    | Website: https://DIY.Holzmann-cfd.com
    ||    // ||    ||     | Copyright (C) 2022 Tobias Holzmann
    =====//  ||    ||     |
-------------------------------------------------------------------------------
License
    This file is part of the BatteryCharger DIY project and is distributed
    under the terms of the GNU General Public License version 3

Description
    This class is the base of the BatteryCharger DIY project. It handles
    all data for each battery slot separately. The functionality is setup in
    a way that a battery object handles the analysis of the
        - battery charging
        - battery discharging
    state and collects all necessary data into a single file. After the test
    for a battery is finished, the data are sent to a webserver.

SourceFiles
    battery.cpp

\*---------------------------------------------------------------------------*/

#ifndef battery_h
#define battery_h

//#include <Streaming.h>
//#include <DallasTemperature.h>
//#include "../writerReader/writerReader.h"
#include "../definitions/definitions.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace LIION
{

/*---------------------------------------------------------------------------*\
                           Class Battery Declaration
\*---------------------------------------------------------------------------*/

class Battery
//:
    //public WriterReader
{
public:

    // The modes to distiguish between different states
    enum mode { CHARGE, DISCHARGE, EMPTY, FIRST, TESTED, FAILED };


private:

    // Private class data

        // Status of program (charge / discharge or no battery inserted)
        mode mode_;

        // The battery slot number
        int slot_;

        // Channel for SLI
        int channel_;

        // Oversampling
        int overSampling_;

        // Number of total discharge cycles to be perfomed
        unsigned int nTotalDischarges_;

        // How many discharge cyclics were already performed
        unsigned int nDischarges_;

        // Old time stamp (ms)
        unsigned long tOld_;

        // Current time stamp (ms)
        unsigned long t_;

        // Offset of time (ms)
        unsigned long tOffset_;


    // Variables for capacity analysis

        // Resistance used (Ohm)
        scalar R_;

        // Actual voltage (V)
        scalar U_;

        // Actual current (mA)
        scalar I_;

        // Actual dissipated power (mW)
        scalar P_;

        // Total battery capacity (mAh)
        scalar C_;

        // Total battery energy (mWh)
        scalar e_;

        // Average capacity if more cycles are performed (mAh)
        scalar CAve_;

        // Average battery energy if more cycles are performed (mWh)
        scalar eAve_;

        // Container that stores the last n voltage data
        // DEPRECIATED: will be replaced by RShunt
        scalar UBat_[2];


    // Temperature sensor data

        // Actual temperature (dC)
        scalar T_;

        // Min-Temperature of cell (dC)
        const scalar TMin_;

        // Max-Temperature of cell (dC)
        const scalar TMax_;

        // Temperature sensor address (DS18B20)
        uint8_t TSensorAddress_[8];

        // Reference to the sensors object
        //DallasTemperature& sensors_;


    // Variables for IO operations

        // File name
        string fileName_;

        // Write interval (s)
        unsigned long writeInterval_;

        // Actual time passed (s)
        unsigned long tPassed_;


public:

    // Constructor for setting up the circuit
    Battery
    (
        const int,
        const int,
        const unsigned long,
        const unsigned long,
        const scalar,
        const scalar,
        const scalar
        //DallasTemperature&
    );

    // Destroctor
    ~Battery();


    // Public Setter Functions

        // Set new offset
        void setOffset(const unsigned long);

        // Set voltage (V)
        void setU(const scalar);

        // Set actual (measured) voltage (V)
        void setU();

        // Set current (mA)
        void setI(const scalar);

        // Set the mode
        void setMode(const mode m);

        // Set bitwise the address of the temperature sensor
        // I am too stupid to do it in the constructor -.-
        void setTSensorAddress(const unsigned int, const uint8_t);


    // Public Return Functions

        // Return the offset time (ms)
        inline unsigned int offset() const { return tOffset_; }

        // Return battery slot
        inline int slot() const { return slot_; };

        // Return the voltage (V)
        inline scalar U() const { return U_; }

        // Return the temperature (dC)
        inline scalar T() const { return T_; }

        // Return the number of discharges
        inline unsigned long nDischarges () const { return nDischarges_; }

        // Return the mode
        inline enum mode mode() const { return mode_; };

        //inline const byte* sensorAddress() { return TSensorAddress_; }


    // Public Member Functions

        // Check if battery was replaced or empty
        bool checkIfReplacedOrEmpty();

        // Increment the number of discharges
        void incrementDischarges();

        // Reset all data to zero
        void reset();

        // Update all data which are needed based on the mode() of
        // the battery (charging/discharging)
        void update();

        // Function that determines if we are still charging
        bool charging();

        // Function that determines if we are still discharging
        bool discharging();

        // Function that determines if the battery is fully tested
        bool checkIfFullyTested() const;

        // Correct the average data (divide by the number of cycles)
        void correctAverageData();


    // Public IO Member Functions

        // Function that removed the battery file from previous analysis, if
        // something went wrong (clearing for startup)
        void removeDataFile() const;

        // Show the data file content
        void showDataFileContent() const;

        // Add the final data to the file such as
        // ++ amount of discharges
        // ++ average values
        // ++ current (from last load) voltage (after battery is switched off
        //    from the power supply - used for the 30-days discharging)
        // ++ Current time-stamp (if available)
        void addFinalDataToFile() const;

        // Update the file name of the measurement data (set the correct name)
        // from 'slot_1' to e.g., 'battery_<ID>'. Furthermore, update the id
        // file
        void updateFileName() const;

        // Function that determines if the battery temperature is okay
        bool temperatureRangeOkay();


private:

    // Private Member Functions

        // Read the digital signal at A0, convert it to a voltage and return it
        scalar readU() const;

        // Convert the digital to an analog value
        // This function provides a mapping from an digital value to an analog
        // value by using an linear interpolation approach
        scalar DtoA
        (
            const unsigned int,
            const int,
            const int,
            const scalar,
            const scalar
        ) const;

        // Read the actual temperature of the sensor at D2 and return the value
        // in [dC]
        scalar readT() const;
};

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace LIION

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
