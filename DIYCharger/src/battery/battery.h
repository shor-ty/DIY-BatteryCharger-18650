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

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //


/*---------------------------------------------------------------------------*\
                           Class Battery Declaration
\*---------------------------------------------------------------------------*/

class Battery
{
public:

    // The modes to distiguish between different states
    enum mode { CHARGE, DISCHARGE, EMPTY, FIRST, TESTED};


private:

    // Private class data

        // Status of program (charge / discharge or no battery inserted)
        mode mode_;

        // Battery id
        int id_;

        // Channel for SLI
        int channel_;

        // Oversampling
        int overSampling_;

        // The amount of already performed discharging cycles
        unsigned int nCycles_;

        // Number of total discharge cycles to be perfomed
        unsigned int nTotalDischarges_{2};

        // Old time stamp (ms)
        unsigned int tOld_;

        // Current time stamp (ms)
        unsigned int t_;

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

        // Discharging time (s)
        float tDischarge_;


    // Variables for charging analysis

        // charging time (s)
        float tCharge_;

        // Charging voltage (V)
        float UCharge_;

        // Charging current (mA)
        float ICharge_;

        // Container that stores the last n voltage data
        float UBat_[20];


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

        // Return battery id
        inline int id() const { return id_; };

        // Return the voltage (V)
        inline float U() const { return U_; }

        // Return the time needed for charging
        inline float tCharge() const { return tCharge_; }

        // Return the number of discharges
        unsigned int nDischarges() const;

        // Return the mode
        enum mode mode() const;


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
        void checkIfFullyTested();


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
