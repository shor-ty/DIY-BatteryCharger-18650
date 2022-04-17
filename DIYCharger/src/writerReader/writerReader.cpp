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

\*---------------------------------------------------------------------------*/

#include "writerReader.h"

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * ///

WriterReader::WriterReader()
{}


WriterReader::~WriterReader()
{}


// * * * * * * * * * * * * Public Return Functions * * * * * * * * * * * * * //

int WriterReader::actualCellID()
{
    // Codes
    // -1 := Error
    // -3 := anything was not handled correctly
    // >0 := cell id
    int result = -3;

    // We could open the file system
    if (startFS())
    {
        // Check if file exists
        if (!fileExist("cellID"))
        {
            // Creat the file
            if(!createFile("cellID"))
            {
                Serial.println("ERROR: File 'cellID' could not be created");
                result = -1;
            }
            else
            {
                // Open the file, and set the id = 1
                if (!writeData("cellID", "1"))
                {
                    Serial.println("ERROR: File 'cellID' not written");
                    result = -1;
                }
            }
        }

        // If file already exist, read data, increment the number and
        // save the new integer
        String data = "";

        if (readFirstLine("cellID", data))
        {
            const int cellID = data.toInt() + 1;

            result = cellID;

            if (!writeData("cellID", String(cellID)))
            {
                Serial.println("ERROR: File 'cellID' not written");
                result = -1;
            }
        }
        else
        {
            Serial.println("ERROR: Could not read file 'cellID'");
            result = -1;
        }
    }
    else
    {
        Serial.println("ERROR: Could not start LittleFS File System");
        result = -1;
    }

    stopFS();

    return result;
}


// * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * * //


// ************************************************************************* //
