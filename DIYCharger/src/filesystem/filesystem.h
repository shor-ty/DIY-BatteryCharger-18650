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

#include <FS.h>

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


private:

    // Private Member Functions


        // Read the file and return the content


        // Write the given content into the file

};

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
