#ifndef DATALOGGER_DEBUGGER_H_
#define DATALOGGER_DEBUGGER_H_

#include <WString.h>
#include <Wire.h>

class Debugger {
    public:

    /**
     * Initialises the debugger
     * 
     * Begins a serial connection then
     *  waits for the connection to be 
     *  established
     */
    void init(void);

    /**
     * Wrapper for Serial.print()
     */
    void print(String msg);

    /**
     * Wrapper for Serial.println()
     */
    void println(String msg);

    /**
     * Wrapper for Serial.printf()
     */
    void printf(String msg, ...);
};

/**
 * Scans for all available I2C devices connected
 */
void I2C_scan(void);


#include "../main.h"
#endif