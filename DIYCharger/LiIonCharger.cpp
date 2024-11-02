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

#include <iostream>
#include <chrono>
//#include <LittleFS.h>
//#include <OneWire.h>
//#include <DallasTemperature.h>
#include "src/battery/battery.hpp"
#include "src/definitions/definitions.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

using namespace LIION;

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

int main (int argc, char** argv)
{
    Header();

    //- Control of the program (should be excluded into a control-file)

        //- Amount of battery slots
        const unsigned int nSlots = 1;

        //- Time when data are written out (s)
        const unsigned int writeInterval = 5;

        //- Charge-Discharge cycles
        const unsigned int nCycles = 1;

        //- Minimum/maximum cell temperature (degC)
        const scalar TCellMin = 5; 
        const scalar TCellMax = 35;

        // On which digital input is the data bus of the DS18B20 connected
        //#define TBUS D2

        //- Addresses of the DS18B20 sensors
        const uint8_t TSensorAddresses [nSlots][8] =
        {
            {0x28,0xFF,0x64,0x2,0xC9,0xDF,0x3B,0x42}    // Sensor-Address #1
        };

        //- Number of n
        const unsigned int nSampling = 20;

        //- The value of the dissipation resistor (ohm)
        const scalar Rdiss [nSlots] =
        {
           5.41
        };

    // ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~

    // Create the battery objects
    Battery* batteries[nSlots];

    for (unsigned int slot = 0; slot < nSlots; slot++)
    {
        Info<< " ++ Initialize battery slot #" + toStr(slot) << endl;

        const auto tNow = std::chrono::system_clock::now();

        const auto tMill =
            std::chrono::duration_cast<std::chrono::milliseconds>
            (
                tNow.time_since_epoch()
            ).count();

        batteries[slot] =
            new Battery
            (
                slot,           // Battery slot
                nCycles,        // Amount of discharge cyclces
                tMill,          // Actual time-stamp
                writeInterval,  // Interval when writting data into file
                3.3,            // Resistance for discharging
                TCellMin,       // Minimum cell temperature
                TCellMax        // Maximum cell temperature
                //TSensors        // Object of the DallasTemperature class
            );

        Info<< " ++ Set the bit-wise address" << endl;;

        // Set bit-wise the address of the temperature sensor
        // I am not able to do it in the constructor via reference nor pointer
        for (unsigned int i = 0; i < 8; ++i)
        {
            batteries[slot]->setTSensorAddress(i, TSensorAddresses[slot][i]);
        }
    }

    return 0;
}

/*



// * * * * * * * * * * * * * * Initialization  * * * * * * * * * * * * * * * //

OneWire TBus(TBUS);
DallasTemperature TSensors(&TBus);


// * * * * * * * * * * * * * * Start Function  * * * * * * * * * * * * * * * //

void setup()
{
    Serial.begin(9600);
    pinMode(D1, OUTPUT);
    pinMode(LED_BUILTIN, OUTPUT);

    if (!LittleFS.begin())
    {
        Serial.println("Error mounting the file system");
        return;
    }

    TSensors.begin();
}


// * * * * * * * * * * * * * * Loop Function * * * * * * * * * * * * * * * * //

void loop()
{
    Serial << " START PROGRAMM " << endl;
    digitalWrite(D1, HIGH);

    // Create the battery objects
    Battery* batteries[slots];

    for (int slot = 0; slot < slots; slot++)
    {
        Serial.println(" ++ Generate battery slot #" + String(slot));
        batteries[slot] =
            new Battery
            (
                slot,           // Battery slot
                NCYCLES,        // Amount of discharge cyclces
                millis(),       // Offset for calculation
                WRITEINTERVAL,  // Interval when writting data into file
                3.3,            // Resistance for discharging
                TMIN,           // Minimum cell temperature
                TMAX,           // Maximum cell temperature
                TSensors        // Object of the DallasTemperature class
            );

        Serial.println(" ++ Set the bit-wise address");
        // Set bit-wise the address of the temperature sensor
        // I am not able to do it in the constructor via reference nor pointer
        for (unsigned int i = 0; i < 8; ++i)
        {
            batteries[slot]->setTSensorAddress(i, TSensorAddresses[slot][i]);
        }
    }
    
    bool finished = false;

    // Own loop in order to not destroy the object
    do
    {
        // Loop through all batteries
        for (auto& battery : batteries)
        {
            // Check if battery is not too hot
            if (!battery->temperatureRangeOkay())
            {
                battery->setMode(Battery::FAILED);
            }

            Serial.println("Temperature = " + String(battery->T()));

            // First check if the battery is already tested or did fail
            // we are finished. Otherwise we will do the analysis of the battery
            if
            (
                (battery->mode() != Battery::TESTED)
             && (battery->mode() != Battery::FAILED)
            )
            {
                // Check if new battery was inserted
                if(battery->checkIfReplacedOrEmpty())
                {
                    if (battery->mode() == Battery::FIRST)
                    {
                        Serial<< " +++ NEW BATTERY DETECTED - RESET +++ \n";
                        battery->setOffset(millis());
                        battery->setU();
                        battery->setMode(Battery::CHARGE);
                        battery->removeDataFile();
                    }
                }

                // Only execute the rest, if a battery is found
                if
                (
                    (battery->mode() != Battery::EMPTY)
                 && (battery->mode() != Battery::FIRST)
                )
                {
                    // Update all data corresponding on the battery mode
                    battery->update();

                    if (battery->mode() == Battery::CHARGE)
                    {
                        if(!battery->charging())
                        {
                            battery->setOffset(millis());
                            if(battery->checkIfFullyTested())
                            {
                                battery->setMode(Battery::TESTED);
                                battery->correctAverageData();
                            }
                            else
                            {
                                battery->setMode(Battery::DISCHARGE);
                            }
                        }
                    }

                    if (battery->mode() == Battery::DISCHARGE)
                    {
                        if(!battery->discharging())
                        {
                            battery->incrementDischarges();
                            battery->setMode(Battery::CHARGE);
                            battery->reset();
                            battery->setOffset(millis());
                        }
                    }
                }
            }
            else
            {
                if (!finished)
                {
                       Serial<< "Finished ...";
                    finished = true;

                    // Add further information to the file, rename it, update
                    // the cellID file and sent it to the server
                    battery->addFinalDataToFile();
                    battery->updateFileName();
                    //battery->sentDataToServer();
                    battery->showDataFileContent();
                }
                delay(60000);
            }
        }

        // Show that the chip is running by simply putting the LED on for 1s
        digitalWrite(LED_BUILTIN, LOW);
        delay(1);
        digitalWrite(LED_BUILTIN, HIGH);
        delay(1000);
    }
    while (true);

    // Destroy all objects if we ever went to this one
    for (int id = 0; id < slots; ++id)
    {
        delete batteries[id];
    }
}
*/


// ************************************************************************* //
