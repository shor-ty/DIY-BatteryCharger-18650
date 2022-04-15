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

#include "src/battery/battery.h"

// * * * * * * * * * * * * * Global Variables  * * * * * * * * * * * * * * * //

#define slots 1
unsigned int nSampling = 20;
float Rdiss = 5.41;


// * * * * * * * * * * * * * * Start Function  * * * * * * * * * * * * * * * //

void setup()
{
  Serial.begin(9600);
  pinMode(D1, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);
}


// * * * * * * * * * * * * * * Loop Function * * * * * * * * * * * * * * * * //

void loop()
{
    Serial << " START PROGRAMM " << endl;
    digitalWrite(D1, HIGH);

    // Create the battery objects
    Battery* batteries[slots];

    for (int id = 0; id < slots; id++)
    {
        Serial << " **** Create the battery object **** " << endl;
        batteries[id] = new Battery(id, millis(), 3.3);
    }

    bool show = true;

    // Own loop in order to not destroy the object
    do
    {
        // Loop through all batteries
        for (auto& battery : batteries)
        {
            // First check if the battery is tested already, if not proceed, otherwise
            // we will do the analysis of the battery
            if (battery->mode() != Battery::TESTED)
            {
                if (battery->mode() != Battery::DISCHARGE && show == true)
                {
                    Serial << " ++ Voltage is       : " << _FLOAT(battery->U(),6) << "\n";
                    Serial << "    Battery slot mode: ";

                    if (battery->mode() == Battery::EMPTY) Serial << "EMPTY\n";
                    else if (battery->mode() == Battery::CHARGE) Serial << "CHARGE\n";
                    else if (battery->mode() == Battery::DISCHARGE) Serial << "DISCHARGE\n";
                    else if (battery->mode() == Battery::FIRST) Serial << "FIRST\n";
                    Serial << "    Digital Pin D1   : " << digitalRead(D1) << "\n";
                    Serial << "    Number of dischar: " << battery->nDischarges() << "\n";
                    Serial << ">>>>>>>" << endl;
                    show = false;
                }
                else
                {
                    delay(2000);
                }

                // Check if new battery was inserted
                if(battery->checkIfReplacedOrEmpty())
                {
                    if (battery->mode() == Battery::FIRST)
                    {
                        Serial<< " +++ NEW BATTERY DETECTED - RESET +++ \n";
                        battery->setOffset(millis());
                        battery->setU();
                        battery->setMode(Battery::CHARGE);
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
                            battery->checkIfFullyTested();
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
                            show = true;
                        }
                    }
                }
            }
            else
            {
                Serial<< "Finished ...";
                delay(60000);
            }
        }

        // Show that the chip is running by simply putting the LED on for 1s
        digitalWrite(LED_BUILTIN, LOW);
        delay(1);
        digitalWrite(LED_BUILTIN, HIGH);
        delay(2000);
    }
    while (true);

    // Destroy all objects if we ever went to this one
    for (int id = 0; id < slots; ++id)
    {
        delete batteries[id];
    }
}


// ************************************************************************* //
