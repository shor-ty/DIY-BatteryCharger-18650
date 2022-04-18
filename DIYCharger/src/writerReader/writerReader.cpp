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

#include <Streaming.h>
#include "writerReader.h"

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * ///

WriterReader::WriterReader()
{}


WriterReader::~WriterReader()
{}


// * * * * * * * * * * * * Public Member Functions * * * * * * * * * * * * * //

void WriterReader::writeData
(
    const String fileName,
    const float t,
    const float U,
    const float I,
    const float P,
    const float C,
    const float e,
    const float T
) const
{
    // Start file system
    if (startFS())
    {
        if (!fileExist(fileName))
        {
            // Create the file
            if(!createFile(fileName))
            {
                Serial
                    << "ERROR: File '" + fileName + "' could not be created"
                    << endl;
            }
            else
            {
                // Create some header information
                // Note, the empty lines at the beginning are going to be
                // replaced at the end of the test
                String header ="";

                // Initialize 10 lines for adding data at the end of the
                // analysis
                for (unsigned int i = 0; i < 8; ++i)
                {
                    header += "#                                           \n";
                }

                header += "#";

                for (unsigned int i = 0; i < 80; ++i)
                {
                    header += "=";
                }

                header += "\n";

                header +=
                    "# t (s)\tU (V)\tI (mA)\tP (mW)\tC (mAh)\te (mWh)\t T (dC)\n#";

                for (unsigned int i = 0; i < 80; ++i)
                {
                    header += "=";
                }

                header += "\n";

                // Open the file, and set the id = 1
                if (!FileSystem::writeData(fileName, header, "w"))
                {
                    Serial
                        << "ERROR: File '" + fileName + " not written"
                        << endl;
                }
            }
        }

        // Create data string
        const String data =
            String(t, 2) + "\t"
          + String(U, 4) + "\t"
          + String(I, 4) + "\t"
          + String(P, 2) + "\t"
          + String(C, 2) + "\t"
          + String(e, 2) + "\t"
          + String(T, 1) + "\n";

        // Write the data
        FileSystem::writeData(fileName, data, "a");

        stopFS();
    }
    else
    {
        Serial << "ERROR: Could not start LittleFS File System" << endl;
    }
}


void WriterReader::insertHorizontalLineToFile(const String fileName) const
{
    // Start file system
    if (startFS())
    {
        if (fileExist(fileName))
        {
            String line = "#";

            for (unsigned int i = 0; i < 80; ++i)
            {
                line += "-";
            }

            line += "\n";

            if (!FileSystem::writeData(fileName, line, "a"))
            {
                Serial
                    << "ERROR: File '" + fileName + " not written"
                    << endl;
            }
        }
    }
    else
    {
        Serial << "ERROR: Could not start LittleFS File System" << endl;
    }
}


void WriterReader::removeDataFile(const String fileName) const
{
    if (startFS())
    {
        if (fileExist(fileName))
        {
            if(!deleteFile(fileName))
            {
                Serial << "ERROR: Could not delete '" + fileName + "'" << endl;
            }
        }

        stopFS();
    }
}


void WriterReader::showDataFileContent(const String fileName) const
{
    if (startFS())
    {
        if (fileExist(fileName))
        {
            Serial << "Show the content of the file" << endl;

            File f = openFile(fileName);

            while (f.available())
            {
                Serial << f.readString() << "\n";
            }

            Serial << endl;
        }
        else
        {
            Serial << "File '" << fileName << "' not available" << endl;
        }

        stopFS();
    }
}


void WriterReader::addFinalDataToFile
(
    const String fileName,
    const unsigned int nCycles,
    const float U,
    const float CAve,
    const float eAve
) const
{
    if (startFS())
    {
        if (fileExist(fileName))
        {
            // Create the string to be added
            // First line will be updated in ::updateFileName with the
            // correct battery id, hence we need to keep it free
            String info =
                "#                                                     \n";

            info +=
                "#----------------------------------------------------------\n";

            info +=
                "# Voltage after last charging (V): " + String(U) + "\n"
             +  "# Discharge cycles      : " + String(nCycles) + "\n"
             +  "# Average energy (mWh)  : " + String(eAve) + "\n"
             +  "# Average capacity (mAh): " + String(CAve) + "\n"
             +  "#----------------------------------------------------------\n";

            FileSystem::writeData(fileName, info, "r+");
        }
        else
        {
            Serial << "File '" << fileName << "' not available" << endl;
        }

        stopFS();
    }
}


void WriterReader::updateFileName(const String fileName) const
{
    // First of all, get the running cell ID (indicates how many cells were
    // already analyzed (function also increments and safes the file)
    const int cellID = actualCellID();

    if (cellID < 0)
    {
        Serial << "ERROR: Cell ID < 0, not possible" << endl;
        return;
    }

    if (startFS())
    {
        if (fileExist(fileName))
        {
            // Add the cell id into the first line
            const String tmp = "# Battery number: " + String(cellID);
            FileSystem::writeData(fileName, tmp, "r+");
        }

        stopFS();
    }

    //rename(fileName, "battery_" + String(cellID));
}


// * * * * * * * * * * * * Private Return Functions  * * * * * * * * * * * * //

int WriterReader::actualCellID() const
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
                Serial << "ERROR: File 'cellID' could not be created" << endl;
                result = -1;
            }
            else
            {
                // Open the file, and set the id = 1
                if (!FileSystem::writeData("cellID", "1", "w"))
                {
                    Serial << "ERROR: File 'cellID' not written" << endl;
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

            if (!FileSystem::writeData("cellID", String(cellID), "w"))
            {
                Serial <<  "ERROR: File 'cellID' not written" << endl;
                result = -1;
            }
        }
        else
        {
            Serial << "ERROR: Could not read file 'cellID'" << endl;
            result = -1;
        }
    }
    else
    {
        Serial << "ERROR: Could not start LittleFS File System" << endl;
        result = -1;
    }

    stopFS();

    return result;
}


// ************************************************************************* //
