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
        return false;
    }
    else
    {
        return true;
    }
}


void FileSystem::stopFS() const
{
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


bool FileSystem::writeData
(
    const String fileName,
    const String data,
    const String mode,
    const int pos
) const
{
    File f = LittleFS.open(fileName.c_str(), mode.c_str());

    if (!f)
    {
        return false;
    }

    // If pos is set, move to that position first
    if (pos != -1)
    {
        Serial<< "move to pos: " << String(pos) << endl;
        f.seek(pos);
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


File FileSystem::openFile(const String fileName, const String mode) const
{
    File tmp = LittleFS.open(fileName.c_str(), mode.c_str());

    return tmp;
}


bool FileSystem::deleteFile(const String fileName) const
{
    return LittleFS.remove(fileName.c_str());
}


void FileSystem::rename(const String nameOld, const String nameNew) const
{
    startFS();
    LittleFS.rename(nameOld, nameNew);
    stopFS();
}


// * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * * //

// ************************************************************************* //
