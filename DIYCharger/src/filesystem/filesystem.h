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

#include <Arduino.h>
#include <Streaming.h>

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //


/*---------------------------------------------------------------------------*\
                           Class FileSystem Declaration
\*---------------------------------------------------------------------------*/

class FileSystem
{

public:

    // Constructor
    FileSystem();

    // Destroctor
    ~FileSystem();
};

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
