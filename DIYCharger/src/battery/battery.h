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

#include <Arduino.h>
#include <Streaming.h>
#include "../writerReader/writerReader.h"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //


/*---------------------------------------------------------------------------*\
                           Class Battery Declaration
\*---------------------------------------------------------------------------*/

class Battery
:
    public WriterReader
{
public:

    // The modes to distiguish between different states
    enum mode { CHARGE, DISCHARGE, EMPTY, FIRST, TESTED};


private:

    // Private class data

        // Status of program (charge / discharge or no battery inserted)
        mode mode_;

        // The index (not changable up to now - its a unique cell id that is
        // increased per finished test, or if the temperature of the cell
        // exceeds the given limit
        int id_;

        // Channel for SLI
        int channel_;

        // Oversampling
        int overSampling_;

        // Number of total discharge cycles to be perfomed
        unsigned int nTotalDischarges_{1};

        // Old time stamp (ms)
        unsigned long tOld_;

        // Current time stamp (ms)
        unsigned long t_;

        // Offset of time (ms)
        unsigned long tOffset_;


    // Variables for capacity analysis

        // Discharge cyclics
        unsigned int nDischarges_;

        // Resistance used (Ohm)
        float R_;

        // Actual voltage (V)
        float U_;

        // Actual current (mA)
        float I_;

        // Actual dissipated power (mW)
        float P_;

        // Total battery capacity (mAh)
        float C_;

        // Total battery energy (mWh)
        float e_;

        // Average capacity if more cycles are performed (mAh)
        float CAve_;

        // Average battery energy if more cycles are performed (mWh)
        float eAve_;

        // Container that stores the last n voltage data
        // DEPRECIATED: will be replaced by RShunt
        float UBat_[2];


    // Variables for IO operations

        // File name
        String fileName_;

public:


    // Constructor
    Battery();

    // Constructor for setting up the circuit
    Battery
    (
        const int,
        const unsigned long,
        const float
    );

    // Destroctor
    ~Battery();


    // Public Setter Functions

        // Set new offset
        void setOffset(const unsigned long);

        // Set voltage (V)
        void setU(const float);

        // Set actual (measured) voltage (V)
        void setU();

        // Set current (mA)
        void setI(const float);

        // Set the mode
        void setMode(const mode m);


    // Public Return Functions

        // Return the offset time (ms)
        inline unsigned int offset() const { return tOffset_; }

        // Return battery id
        inline int id() const { return id_; };

        // Return the voltage (V)
        inline float U() const { return U_; }

        // Return the number of discharges
        inline unsigned long nDischarges () const { return nDischarges_; }

        // Return the mode
        inline enum mode mode() const { return mode_; };


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


private:

    // Private Member Functions

        // Read the digital signal at A0, convert it to a voltage and return it
        float readU() const;

        // Convert the digital to an analog value
        // This function provides a mapping from an digital value to an analog
        // value by using an linear interpolation approach
        float DtoA
        (
            const unsigned int,
            const int,
            const int,
            const float,
            const float
        ) const;
};

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
