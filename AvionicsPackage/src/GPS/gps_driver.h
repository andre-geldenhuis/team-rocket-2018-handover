#ifndef DATALOGGER_GPS_DRIVER_H_
#define DATALOGGER_GPS_DRIVER_H_

#include "TinyGPS++.h" // http://arduiniana.org/libraries/tinygpsplus/
#include "../globals.h"
#include "../SensorObject/sensor.h"

/**
 * Object to hold data from the GPS
 * 
 * Includes a pointer to the next data
 *  point of the linked list
 */
class GPS_Data : public Sensor_Data
{
    public:
    TinyGPSTime time;
    TinyGPSLocation location;
    String sentence;

    /**
     * Converts the GPS_Data object to a csv string
     * 
     * @returns String
     *      csv formatted string of the Data object
     */
    String to_string(void);
};

/**
 * GPS Module wrapper
 * Allows reading and storing IMU data
 *  in a linked list
 */
class GPS_Module : public Sensor
{
    public:
    GPS_Module(TinyGPSPlus* libObj);
    
    /**
     * Initialises the GPS module and 
     *  any required dependacies
     * 
     * @returns bool
     *      true
     * 
     * TODO:   true iff the GPS initialised correctly
     */
    bool init(void);

    /**
     * Reads from the GPS and creats an 
     *  GPS data object to store them
     * 
     * @returns GPS_Data
     *      the newly read data from the GPS
     */
    GPS_Data* read(void);

    void print_status(void);

    private: 
    bool check_start(void);
    bool useTop = true; // Decides which antenna to use
    bool read_started = false;
    TinyGPSPlus* gps_device;   // GPS libary declaration
};

#endif