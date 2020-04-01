#ifndef DATALOGGER_SENSOR_H_
#define DATALOGGER_SENSOR_H_
#include "../globals.h"
class Sensor_Data
{
    public:
    Sensor_Data* next = NULL;
    uint32_t timestamp = millis();

    virtual ~Sensor_Data(){}

    /**
     * Converts the GPS_Data object to a csv string
     * 
     * @returns String
     *      csv formatted string of the Data object
     */
    virtual String to_string(void);
};

/**
 * GPS Module wrapper
 * Allows reading and storing IMU data
 *  in a linked list
 */
class Sensor
{
    public:
    int reads = 0;
    virtual ~Sensor(){}
    /**
     * Initialises the GPS module and 
     *  any required dependacies
     * 
     * @returns bool
     *      true
     * 
     * TODO:   true iff the GPS initialised correctly
     */
    virtual bool init(void);

    /**
     * Reads from the GPS and creats an 
     *  GPS data object to store them
     * 
     * @returns GPS_Data
     *      the newly read data from the GPS
     */
    virtual Sensor_Data* read(void);

    /**
     * Reads from the GPS and adds the data 
     *  to the linked list
     */
    void record(void);

    /**
     * Empties the list and properly
     *  disploses of all nodes
     */
    void clear_list(void);

    /**
     * Gets the entire list of data 
     *  objects. Each node can be used
     *  to get the next node
     * 
     * End of the list is when the next node = null
     * 
     * @returns GPS_Data
     *      the head of the list
     */
    Sensor_Data* get_list(void);

    /**
     * Gets the latest node added to the list
     * 
     * @returns GPS_Data
     *      The most recently added data object
     */
    Sensor_Data* get_tail(void);

    String list_to_string(void);

    size_t get_length(void);

    private:
    Sensor_Data* head;     // Head of the data list
    Sensor_Data* tail;     // Last node of the data list
    size_t list_len = 0;
};

#endif