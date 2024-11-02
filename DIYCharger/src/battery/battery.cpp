/*---------------------------------------------------------------------------*\
    =====\\  || \\    //  |
    ||    \\ ||  \\  //   | Project: LIION::BatteryCharger using D1 Wemos
    ||    || ||   \\//    | Website: https://DIY.Holzmann-cfd.com
    ||    // ||    ||     | Copyright (C) 2022 Tobias Holzmann
    =====//  ||    ||     |
-------------------------------------------------------------------------------
License
    This file is part of the LIION::BatteryCharger DIY project and is distributed
    under the terms of the GNU General Public License version 3

\*---------------------------------------------------------------------------*/

#include "battery.hpp"

// * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * * //

LIION::Battery::Battery
(
    const int slot,
    const int nDischargeCycles,
    const unsigned long tOffset,
    const unsigned long writeInterval,
    const scalar R,
    const scalar TMin,
    const scalar TMax//,
//    DallasTemperature& sensors
)
:
    mode_(LIION::Battery::EMPTY),
    slot_(slot),
    channel_(-1),
    overSampling_(20),
    nTotalDischarges_(nDischargeCycles),
    nDischarges_(0),
    tOld_(0),
    t_(0),
    tOffset_(tOffset),
    R_(R),
    U_(0),
    I_(0),
    P_(0),
    C_(0),
    e_(0),
    T_(0),
    TMin_(TMin),
    TMax_(TMax),
    TSensorAddress_{0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0},
    //sensors_(sensors),
    fileName_("slot_" + toStr(slot)),
    writeInterval_(writeInterval),
    tPassed_(0)
{
    reset();
}


LIION::Battery::~Battery()
{}


// * * * * * * * * * * * Public Setter Functions * * * * * * * * * * * * * * //

void LIION::Battery::setOffset(const unsigned long tOffset)
{
    tOffset_ = tOffset;
    t_ = 0;
    tOld_ = 0;
}


void LIION::Battery::setU(const scalar U)
{
    U_ = U;
}


void LIION::Battery::setU()
{
    U_ = readU();
}


void LIION::Battery::setI(const scalar I)
{
    I_ = I;
}


void LIION::Battery::setMode(const enum mode m)
{
    mode_ = m;

    if (mode_ == LIION::Battery::CHARGE)
    {
        //digitalWrite(D1, HIGH);
    }
    else if (mode_ == LIION::Battery::DISCHARGE)
    {
        //digitalWrite(D1, LOW);
    }
    else if (mode_ == LIION::Battery::EMPTY)
    {
        //digitalWrite(D1, HIGH);
    }
}


void LIION::Battery::setTSensorAddress(const unsigned int i, const uint8_t b)
{
    //TSensorAddress_[i] = b;
}


// * * * * * * * * * * * Public Member Functions * * * * * * * * * * * * * * //

bool LIION::Battery::checkIfReplacedOrEmpty()
{
    // Get actual voltage
    const scalar U = readU();

    // Check if slot is empty (measured voltage < 0.5 V)
    if (U < 0.5)
    {
        setMode(LIION::Battery::EMPTY);
        reset();
        return true;
    }
    else
    {
        // Check if the state was empty, if so, set to first and return true
        if (mode() == LIION::Battery::EMPTY)
        {
            setMode(LIION::Battery::FIRST);

            // It was replaced
            return true;
        }

        // It was not replaced and previously not empty
        return false;
    }

    // TODO: might be optimized
    // Check if current U at A0 is around the last measurement
    // point +- 50 mV, or if we are < 2.4 V its a new one
    /*if (U < 2.4)
    {
        return true;
    }

    if
    (
        (U > (U_ - 0.05) && U < (U_ + 0.051))
    )
    {
        return false;
    }
    else
    {
        return true;
    }*/
}


void LIION::Battery::incrementDischarges()
{
    ++nDischarges_;
}


void LIION::Battery::reset()
{
    // Before resetting, take the last values for averaging process
    // We devide this data after we are finished by nCycles
    CAve_ += C_;
    eAve_ += e_;

    // Reset all data
    tOld_ = 0;
    t_ = 0;
    tOffset_ = t_;
    U_ = 0;
    I_ = 0;
    P_ = 0;
    C_ = 0;
    e_ = 0;
    tPassed_ = 0;

    // Set all points to 0
    for (size_t i = 0; i < sizeof(UBat_)/sizeof(scalar); ++i)
    {
        UBat_[i] = 0;
    }
}


void LIION::Battery::update()
{
    // Update the actual voltage - we do it for both modes
    // + charging
    // + discharging
    setU();

    // Calculate the current (mA)
    I_ = U_/R_ * 1000.;

    // Calculate the current dissipation (mW)
    P_ = U_ * I_;

    // Update the time (ms)
    tOld_ = t_;
    //t_ = millis() - tOffset_;
    t_ = 0;

    const scalar dt = t_ - tOld_;
    tPassed_ += dt;

    // Calcualte the capacity (mAh)
    C_ += I_ * dt / 1000. / 3600.;

    // Calculate the energy (mWh)
    e_ += P_ * dt / 1000. / 3600.;

    // Write data to file
    if (tPassed_ > writeInterval_)
    {
        /*
        writeData
        (
            fileName_,
            (t_/scalar(1000)),
            U_,
            I_,
            P_,
            C_,
            e_
        );
        */

        tPassed_ = 0;
    }
}


bool LIION::Battery::charging()
{
    // If current voltage is lower than 4.05V we are not fully charged
    // The charging modules charge up to 4.05V
    if (U_ < 4.1)
    {
        return true;
    }

    // This needs to be done by checking the voltage AND the current through
    // the battery

    // Re-arange UBat
    int i = -1;
    for (auto& value : UBat_)
    {
        if (i >= 0)
        {
            UBat_[i] = value;
        }

        ++i;

        // Last datapoint is the actual measured voltage
        if (i == sizeof(UBat_)/sizeof(scalar) -1)
        {
            UBat_[i] = U_;
        }
    }

    // Check if all values are identical by an error of 1e-5 to the actual value
    scalar tmp = 0;

    for (const auto value : UBat_)
    {
        tmp += value;
    }

    tmp /= sizeof(UBat_)/sizeof(scalar);

    // Charging finished
    if (abs(U_ - tmp) < 1e-5)
    {
        // Add horizontal line to file
        //WriterReader::insertHorizontalLineToFile(fileName_);

        return false;
    }
    // Still charging
    else
    {
        return true;
    }
}


bool LIION::Battery::discharging()
{
    // If the current voltage is lower than 2.5V we stop discharging
    if (U_ < 2.60)
    {
        // Add horizontal line to file
        //WriterReader::insertHorizontalLineToFile(fileName_);

        return false;
    }
    // Still discharging
    else
    {
        return true;
    }
}


bool LIION::Battery::checkIfFullyTested() const
{
    if (nDischarges_ == nTotalDischarges_)
    {
        return true;
    }
    else
    {
        return false;
    }
}


bool LIION::Battery::temperatureRangeOkay()
{
    Info<< " ++ Check temperature range" << endl;

    // FIrst make an temperature update
    T_ = readT();

    Info<< " ++ T = " + toStr(T_) << endl;

    // Handle error codes
    if (T_ == 85 || T_ == -127)
    {
        return false;
    }

    // User defined bounds
    if (T_ < TMin_ || T_ > TMax_)
    {
        return false;
    }
    else
    {
        return true;
    }
}


void LIION::Battery::correctAverageData()
{
    CAve_ /= scalar(nDischarges_);
    eAve_ /= scalar(nDischarges_);
}


// * * * * * * * * * * * Public IO Member Functions  * * * * * * * * * * * * //

void LIION::Battery::removeDataFile() const
{
    //WriterReader::removeDataFile(fileName_);
}


void LIION::Battery::showDataFileContent() const
{
    //WriterReader::showDataFileContent(fileName_);
}


void LIION::Battery::addFinalDataToFile() const
{
    /*
    WriterReader::addFinalDataToFile
    (
        fileName_,
        nDischarges_,
        readU(),
        CAve_,
        eAve_
    );
    */
}


void LIION::Battery::updateFileName() const
{
    //WriterReader::updateFileName(fileName_);
}


// * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * * * //

LIION::scalar LIION::Battery::readU() const
{
    // Make 20 measeurements and create the mean value
    int Udigital = 0;

    // Sampling more values with delay of 25 ms
    for (int k = 0; k < overSampling_; ++k)
    {
      //int tmp = analogRead(A0 - 17);
      int tmp = 0;
      //Udigital += constrain(tmp, 0, 1023);
      //delay(10);
    }

    Udigital /= overSampling_;

    // Convert digital voltage to analog voltage
    // The values given below need to be calibrated
    return DtoA(Udigital, 0, 794, 0, 3.2835);
}


LIION::scalar LIION::Battery::DtoA
(
    const unsigned int digital,
    const int lowD,
    const int highD,
    const scalar lowA,
    const scalar highA
) const
{
    // Steps in digital way
    const unsigned int digStep = highD - lowD;

    // Analog width
    const scalar widthA = highA - lowA;

    // Analogh width per digital step
    const scalar DtoA = widthA / digStep;

    return (DtoA * digital);
}


LIION::scalar LIION::Battery::readT() const
{
    Info<< " ++ Request data by address" << endl;
    // Update the data
    //sensors_.requestTemperaturesByAddress(TSensorAddress_);

    // Take the data
    Info<< " ++ Get temperature-data" << endl;
    //scalar tmp = sensors_.getTempC(TSensorAddress_);

    scalar tmp = 0;

    return tmp;
}


// ************************************************************************* //
