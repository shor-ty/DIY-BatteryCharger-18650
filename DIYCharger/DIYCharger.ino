/*---------------------------------------------------------------------------*\
    =====\\  || \\    //  |
    ||    \\ ||  \\  //   | Project: BatteryCharger using D1 Wemos
    ||    || ||   \\//    | Website: https://DIY.Holzmann-cfd.com
    ||    // ||    ||     | Copyright (C) 2022 Tobias Holzmann
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

#include <LittleFS.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include "src/battery/battery.h"

// * * * * * * * * * * * * * Global Variables  * * * * * * * * * * * * * * * //

// Define how many battery slots your project has
// If you have more than two, you need to take care of MULTIPLEXER and correct
// signal usage
#define slots 1


// The timeInterval describes approximate after how many seconds a new entry
// is added into the measurement file. Low values give higher resolution but
// also increase the data file size. If we charge/discharge commonly within
// 1 to 3h, a interval > 30 is sufficient. This will not influence the analysis
// of the average calculation
#define WRITEINTERVAL 5


// Set how many discharging cycles should be performed. For a more reliable
// analysis, you can do more than one cycle
#define NCYCLES 1


// Temperature sensor input and battery temperature ranges

    // Minimum cell temperature (dC)
    float TMIN{5};

    // Maximum cell temperature (dC)
    float TMAX{28};

    // On which digital input is the data bus of the DS18B20 connected
    #define TBUS D2

    // All DS18B20 sensor addresses related to the single slots
    // We are working with addresses here to have full control
    // You can simply get the sensor addresses by connecting one sensor to
    // your Arduino board and use the DS18x20 example sketch. You will see the
    // address of the sensor in this form:
    // >>> "28 FF 64 2 C9 DF 3B 42"
    // Simply add the 0x to the single values to get
    // >>> "0x28 0xFF 0x66 0x2 0xC9 0xDF 0x3B 0x42"
    const byte TSensorAddresses [slots][8] =
        {
            {0x28,0xFF,0x64,0x2,0xC9,0xDF,0x3B,0x42}    // Sensor-Address #1
        };


unsigned int nSampling = 20;
float Rdiss = 5.41;


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
                //battery->setMode(Battery::FAILED);
                //battery->writeTemperatureData();
            }

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


// ************************************************************************* //
