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
    This class is used for all manipulations done in the filesystem (SPIF)
    It handles the correct data manipulation and all IOs which are needed
    for the battery discharge and charging process

SourceFiles
    filesystem.cpp

\*---------------------------------------------------------------------------*/

#ifndef filesystem_h
#define filesystem_h

#include <LittleFS.h>

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //


/*---------------------------------------------------------------------------*\
                           Class FileSystem Declaration
\*---------------------------------------------------------------------------*/

class FileSystem
{
    // Private class data


        // File name that holds the measurement data
        String fileName_;

public:

    // Constructor
    FileSystem();

    // Destroctor
    ~FileSystem();


    // Public Setter Functions


    // Public Return Functions


    // Public Member Functions

        // Start the LittleFS system
        bool startFS() const;

        // Stop the LittleFS system
        void stopFS() const;

        // Check if file exist
        bool fileExist(const String) const;

        // Creat an empty file
        bool createFile(const String) const;

        // Write data into a file
        bool writeData
        (
            const String,
            const String,
            const String mode = "w",
            const int pos = -1
        ) const;

        // Read first line of file
        bool readFirstLine(const String, String&) const;

        // Open the file for reading (default) or writing
        File openFile(const String, const String = "r") const;

        // Remove the specified file from the flash
        bool deleteFile(const String) const;

        // Rename the file
        void rename(const String, const String) const;


private:

    // Private Member Functions

        // Read the actualCellID file and return the integer


        // Write the given content into the file

};

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
