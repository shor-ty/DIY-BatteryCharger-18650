/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *\

  Description
    Foobar

  Author
    Tobias Holzmann
  Date
    April 2020

\* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#ifndef battery_h
#define battery_h

#include "Arduino.h"
#include "Streaming.h"

/*****************************************************************************/

class Battery
{
public:

      // The modes to distiguish between different states
      enum mode { CHARGE, DISCHARGE, EMPTY, FIRST, TESTED};


private:

  // Private class data

    // Status of program (charge / discharge or no battery inserted)
    mode mode_;

    // Number of discharges to be perfomed
    unsigned int nTotalDischarges_{2};

    // Battery id
    int id_;

    // Channel
    int channel_;

    // Oversampling
    int overSampling_;

    // n cyclics for discharging
    int nCycles_;

    // Old time stamp (ms)
    unsigned int tOld_;

    // Current time stamp (ms)
    unsigned int t_;

    // Offset of time (ms)
    unsigned long tOffset_;
    

    // Variables for capacity analysis

      // Resistance used (Ohm)
      float R_;
    
      // Actual voltage (V)
      float U_;
    
      // Actual current (mA)
      float I_;
    
      // Actual dissipated power (mW)
      float P_;
    
      // Total battery capacity (mAh)
      float C_;
    
      // Total battery energy (mWh)
      float e_;
    
      // Discharging time (s)
      float tDischarge_;

      // Discharge cyclics
      unsigned int nDischarges_;


    // Variables for charging analysis

      // charging time (s)
      float tCharge_;

      // Charging voltage (V)
      float UCharge_;

      // Charging current (mA)
      float ICharge_;

      // Container that stores the last n voltage data
      float UBat_[20];
   

public:

  // Constructor
  Battery();
  
  Battery
  (
    const int,
    const unsigned long,
    const float
  );

  // Destroctor
  ~Battery();


  // Public functions

    
    // Return battery id
    inline int id() const { return id_; };

    // Check if battery was replaced or empty
    bool checkIfReplacedOrEmpty() const;

    // Set new offset
    void setOffset(const unsigned long);

    // Set voltage (V)
    void setU(const float);

    // Set actual voltage (V)
    void setU();

    // Set current (mA)
    void setI(const float);

    // Increment the number of discharges
    void incrementDischarges();

    // Return the number of discharges
    unsigned int nDischarges() const;

    // Set the mode
    void setMode(const mode m);

    // Return the mode
    enum mode mode() const;

    // Return the voltage (V)
    inline float U() const { return U_; }

    // Return the time needed for charging
    inline float tCharge() const { return tCharge_; }
    
    // Reset all data to zero
    void reset();

    // Update all data which are needed based on the mode() of 
    // the battery (charging/discharging)
    void update();

    // Function that determines if we are still charging or not
    bool charging() const;

    // Function that determines if we are still discharging or not
    bool discharging() const;

    // Function that determines if the battery is fully tested
    void checkIfFullyTested();


private:

    // Read the digital signal at A0, convert it to a voltage and return it
    float readU() const;

    // Convert the digital to an analog value
    // This function provides a mapping from an digital value to an analog value
    // by using an linear interpolation approach
    float DtoA
    (
      const unsigned int,
      const int,
      const int,
      const float,
      const float
    );
};

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
