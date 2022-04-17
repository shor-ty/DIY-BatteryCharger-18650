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

        // Return the current battery id of the file batteryID
        // Note: This file stores simply an integer that is increased
        // per new battery analysis if it was TESTED or FAILED
        // The number cannot be changed or decremented by default. However
        // a simple push button on any digital input can handle such behavior
        int actualCellID();


    // Public Member Functions


private:

    // Private Member Functions


};

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
