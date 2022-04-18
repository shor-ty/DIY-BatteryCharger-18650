/*---------------------------------------------------------------------------*\
    =====\\  || \\    //  |
    ||    \\ ||  \\  //   | Project: BatteryCharger using D1 Wemos
    ||    || ||   \\//    | Webiste: https://Holzmann-cfd.com
    ||    // ||    ||     | Copyright (C) 2022 Tobias Holzmann
    =====//  ||    ||     |
-------------------------------------------------------------------------------
License
    This file is part of the BatteryCharger DIY project and is distributed
    under the terms of the GNU General Public License version 3

Description
    This class is used to write and read the data needed in the battery class
    It takes the FileSystem class as base for the IO operations but manipulates
    the data first for the correct handling, structur and formatation

SourceFiles
    writeReader.cpp

\*---------------------------------------------------------------------------*/

#ifndef writerReader_h
#define writerReader_h

#include "../filesystem/filesystem.h"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //


/*---------------------------------------------------------------------------*\
                           Class WriterReader Declaration
\*---------------------------------------------------------------------------*/

class WriterReader
:
    public FileSystem
{
    // Private class data


public:

    // Constructor
    WriterReader();

    // Destroctor
    ~WriterReader();


    // Public Setter Functions


    // Public Return Functions



    // Public Member Functions

        // Write the di/charging data to the slot name (the name will be
        // changed at the end of the test to fit the correct battery ID)
        void writeData
        (
            const String,
            const float,
            const float,
            const float,
            const float,
            const float,
            const float,
            const float
        ) const;

        // Write the temperature data (FAILED) to the end of the file
        void writeTemperatureData (const String, const float) const;

        // Write a 80 character line based on '-' signs
        void insertHorizontalLineToFile (const String) const;

        // Remove the specified file from the system
        void removeDataFile(const String) const;

        // Show the content of the data file
        void showDataFileContent(const String) const;

        // Add final file content such as summaries
        void addFinalDataToFile
        (
            const String,
            const unsigned int,
            const float,
            const float,
            const float
        ) const;

        // Update the file name to 'battery_<ID>' and update the cellID file
        void updateFileName(const String) const;


private:

    // Private Member Functions

        // Return the current battery id of the file batteryID
        // Note: This file stores simply an integer that is increased
        // per new battery analysis if it was TESTED or FAILED
        // The number cannot be changed or decremented by default. However
        // a simple push button on any digital input can handle such behavior
        int actualCellID() const;
};

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
