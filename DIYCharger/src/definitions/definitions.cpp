/*---------------------------------------------------------------------------*\
=====\\  || \\      |
||    \\ ||  \\     | Project: BatteryCharger using D1 Wemos
||    || ||   \\    | Website: https://DIY.Holzmann-cfd.com
||     ||    ||     | Copyright (C) 2024 Tobias Holzmann
=====  ||    ||     |
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
std::ostream& TKC::Info = std::cout;

std::ostream& TKC::Error = std::cerr;

std::basic_ostream<char>& (& TKC::endl)(std::basic_ostream<char>&) = std::endl;


void TKC::ErrorMsg
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
<< "Tobias.Holzmann@Holzmann-cfd.de.\n" << endl;

std::terminate();
}
void TKC::Warning(const string msg, const char* file, const unsigned long line)
{
Error<< "\n    * Warning in " << file << " line " << line << "\n" << msg
<< "\n" << endl;
}
//
//
void TKC::NotImplemented(const char* file, const size_t line)
{
Error<< "\n"
<< "    * The functionality is not implemented.\n" << endl;
}
//
//
TKC::string TKC::Header()
{
string header = \
"\
/*------------------------------------------------------------------------*\\\
\n|  c-o-o-c-o-o-o             |                                             |\
\n|  |     |     T hermo       | TKC: The Open Source Thermo-Kinetic Library |\
\n|  c-o-o-c     K inetic      | Version: 1.0.0                              |\
\n|  |     |     C alculator   | Web: www.Holzmann-cfd.com                   |\
\n|  c     c-o-o-o             |                                             |\
\n\
\\*------------------------------------------------------------------------*/\
\n";
//
return header;
}
//
// * * * * * * * * * * * * * * String Conversation * * * * * * * * * * * * * //
//
//
//
void TKC::Footer(const scalar startTime)
{
const scalar execTime = (clock()-startTime) / (scalar) CLOCKS_PER_SEC;
//
Info<< "\n\n c-o Execution: " << execTime  << " s\n\n"
<< " ============================================================\n\n"
<< " c-o Programmed by Tobias Holzmann\n\n"
<< " c-o Tobias.Holzmann@Holzmann-cfd.de\n" << endl;
}
//
//
// ************************************************************************* //V
