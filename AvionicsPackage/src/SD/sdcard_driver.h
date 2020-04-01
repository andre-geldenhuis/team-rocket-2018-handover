#ifndef DATALOGGER_SDCARD_DRIVER_H_
#define DATALOGGER_SDCARD_DRIVER_H_

#include <SD.h>

#include "../globals.h"

/**
 * SD Card driver module
 * Handles the initialisation of the SD Card
 * Should be one object for each SD card writer
 */
class SD_Module
{
    public:
    int writes = 0;
    
    /**
     * Initialises the SD card
     * 
     * @returns bool
     *      true iff the SD card initailised successfully
     */
    bool init(void);

    /**
     * Writes a string to the SD Card
     * 
     * @params String filename
     *      The name of the file to write to
     * 
     * @params String str
     *      The string to append to the SD Cards data file
     * 
     */
    bool write(const char* filename, String str);

    /**
     * Reads a file from an SD Card
     * 
     * @params String filename
     *      The filename of the file on the SD Card
     * 
     * @returns String
     *      The String of the entire file from the SD Card
     */
    String read(const char* filename);
};

#endif