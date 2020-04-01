#ifndef DATALOGGER_IMU_DRIVER_H_
#define DATALOGGER_IMU_DRIVER_H_

#include "MPU9250.h" // https://github.com/bolderflight/MPU9250
#include "../globals.h"
#include "../SensorObject/sensor.h"

#define IMU_CS 15

/**
 * Object to hold data from the IMU
 * 
 * Includes a pointer to the next data
 *  point of the linked list
 */
class IMU_Data : public Sensor_Data
{
    public:
    float accel_x, accel_y, accel_z, magn_x, magn_y, magn_z, gyro_x, gyro_y, gyro_z;

    /**
     * Converts the IMU_Data object to a csv string
     * 
     * @returns String
     *      csv formatted string of the Data object
     */
    String to_string(void);
    
};

/**
 * IMU Module wrapper
 * Allows reading and storing IMU data
 *  in a linked list
 */
class IMU_Module : public Sensor
{
    public:
    /**
     * IMU Module constructor
     * Assigns the imu drivers for this this wrapper
     * 
     * @param MPU9250* libObj
     *      Pointer to the MPU9250 object that this 
     *       object will use for reading from the IMU
     */
    IMU_Module(MPU9250* libObj);

    /**
     * Initialises the IMU module and 
     *  any required dependacies
     * 
     *  Warning: if the IMU is not connected properly
     *           then this function will block for undefined
     *           amount of time
     * 
     * @returns bool
     *      true
     * 
     * TODO:   true iff the imu initialised correctly
     */
    bool init(void);

    /**
     * Reads from the IMU and creats an 
     *  IMU data object to store them
     * 
     * @returns IMU_Data
     *      the newly read data from the IMU
     */
    IMU_Data* read(void);

    private:
    MPU9250* imu_device;
};

#endif