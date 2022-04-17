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

#include "filesystem.h"

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * ///

FileSystem::FileSystem()
{}


FileSystem::~FileSystem()
{}


// * * * * * * * * * * * * Public Return Functions * * * * * * * * * * * * * //

bool FileSystem::startFS() const
{
    if (!LittleFS.begin())
    {
        // TODO set some info as LED
        Serial.println("Error mounting the file system");

        return false;
    }
    else
    {
        Serial.println("Mounting the file system");
        return true;
    }
}


void FileSystem::stopFS() const
{
    Serial.println("Unmount the file system");

    LittleFS.end();
}


bool FileSystem::fileExist(const String fileName) const
{
    return LittleFS.exists(fileName.c_str());
}


bool FileSystem::createFile(const String fileName) const
{
    File f = LittleFS.open(fileName.c_str(), "w");

    if (!f)
    {
        return false;
    }
    else
    {
        f.close();
        return true;
    }
}


bool FileSystem::writeData(const String fileName, const String data) const
{
    File f = LittleFS.open(fileName.c_str(), "w");

    if (!f)
    {
        return false;
    }

    f.print(data);

    f.close();

    return true;
}


bool FileSystem::readFirstLine(const String fileName, String& firstLine) const
{
    File f = LittleFS.open(fileName.c_str(), "r");

    if (!f)
    {
        return false;
    }

    firstLine = f.readString();

    f.close();

    return true;
}


File FileSystem::openFile(const String fileName, const String mode)
{
    File tmp = LittleFS.open(fileName.c_str(), mode.c_str());

    return tmp;
}


// * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * * //

// ************************************************************************* //
