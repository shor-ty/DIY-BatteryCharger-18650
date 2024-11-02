/*---------------------------------------------------------------------------*\
    =====\\  || \\    //  |
    ||    \\ ||  \\  //   | Project: BatteryCharger using D1 Wemos
    ||    || ||   \\//    | Website: https://DIY.Holzmann-cfd.com
    ||    // ||    ||     | Copyright (C) 2024 Tobias Holzmann
    =====//  ||    ||     |
-------------------------------------------------------------------------------
License
    This file is part of the BatteryCharger DIY project and is distributed
    under the terms of the GNU General Public License version 3

Description
    An automatic charger/discharger software for n cells of type 18650.
    The programming is based on the Wemos D1 Mini Chip (ESP8266) which analysis
    the remaining capacity of each cell by providing as many discharge cycles
    as the uses wishes.

    The charing and discharging data are tracked and stored for each battery
    separately in the SPIFF memory. After the test is successfully performed,
    the data are sent to some remote compute (sending it to some database).

    In order to be able to understand everything, you can checkout my videos
    at YouTube (channel name: HolzmannCFD and the tag DIY).

Author
    Tobias Holzmann

\*---------------------------------------------------------------------------*/

#include "definitions.hpp"

// * * * * * * * * * * * * * * * * Definitions * * * * * * * * * * * * * * * //

std::ostream& LIION::Info = std::cout;

std::ostream& LIION::Error = std::cerr;

std::basic_ostream<char>& (& LIION::endl)(std::basic_ostream<char>&) = std::endl;


void LIION::ErrorMsg
(
    const string msg,
    const char* file,
    const unsigned long line
)
{
    Error<< "\n    *** Error in " << file << " line " << line << "\n"
    << "    " << msg << "\n\n"
    << "    If there is a bug or a problem that you can not solve,\n"
    << "    do not hesitate to write an email to "
    << "community@Holzmann-cfd.com.\n" << endl;

    std::terminate();
}


void LIION::Warning(const string msg, const char* file, const unsigned long line)
{
    Error<< "\n    * Warning in " << file << " line " << line << "\n" << msg
    << "\n" << endl;
}


void LIION::NotImplemented(const char* file, const size_t line)
{
    Error<< "\n"
    << "    * The functionality is not implemented.\n" << endl;
}


void LIION::Header()
{
    Info<<
 "/*------------------------------------------------------------------------*\\\
\n|        ___---___           |                                             |\
\n|        |  |  |  |          | LIION: The Open Source LIION Software       |\
\n|        |  |  |  |          | Version: v2310                              |\
\n|        |  |  |  |          | Web: www.Holzmann-cfd.com                   |\
\n|        | Li-Ion |          |                                             |\
\n|        ---------           |                                             |\
\n\\*------------------------------------------------------------------------*/\
\n" << endl;

}


// * * * * * * * * * * * * * * String Conversation * * * * * * * * * * * * * //

void LIION::Footer(const scalar startTime)
{
    const scalar execTime = (clock()-startTime) / (scalar) CLOCKS_PER_SEC;

    Info<< "\n\n c-o Execution: " << execTime  << " s\n\n"
    << " ============================================================\n\n"
    << " c-o Programmed by Dr.mont. Tobias Holzmann\n\n"
    << " c-o community@Holzmann-cfd.com\n" << endl;
}


// ************************************************************************* //V
