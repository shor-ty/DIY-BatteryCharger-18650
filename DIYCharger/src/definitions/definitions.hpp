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

#ifndef DEFINITIONS_HPP
#define DEFINITIONS_HPP

#include <time.h>
#include <vector>
#include <string>
#include <sstream>
#include <string.h>
#include <iostream>
#include <iomanip>
#include <memory>
#include <map>

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace LIION
{

/*---------------------------------------------------------------------------*\
	            		LIION Defintions
\*---------------------------------------------------------------------------*/

using ostream = std::ostream;

using fstream = std::fstream;

using scalar = long double;

using word = std::string;

using string = std::string;

extern std::ostream& Info;

extern std::ostream& Error;

extern std::basic_ostream<char>& (&endl)(std::basic_ostream<char>&);

template<class T> using List = std::vector<T>;

template<class T> using smartPtr = std::unique_ptr<T>;

template<class T, class Z> using map = typename std::map<T, Z>;

template<class T, class Z> using mapList = typename std::vector<map<T, Z> >;

#define forEach(Field, i) for (unsigned int i=0; i<Field.size(); i++)
#define forAll(Field, entry) for (auto& entry : Field)
#define forAllConst(Field, entry) for (auto const& entry : Field)
#define loopMap(first, second, Field) for (auto& [first, second] : Field)
#define loopMapConst(first, second, Field) for (auto const& [first, second] : Field)

void Print();

void ErrorMsg(const string, const char*, const unsigned long);

void Warning(const string, const char*, const unsigned long);

void NotImplemented(const char*, const unsigned long);

string Header();

void Footer(const scalar);

// * * * * * * * * * * * * * * String Conversation * * * * * * * * * * * * * //

template <typename T>
string toStr(const T& tmp)
{
    return std::to_string(tmp);
}


template <typename T>
T min(const T tmp1, const T tmp2)
{
return std::min(tmp1, tmp2);
}


template <typename T>
T max(const T tmp1, const T tmp2
)
{
return std::max(tmp1, tmp2);
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace TKC

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif // DEFINITIONS_HPP_INCLUDED

// ************************************************************************* //
