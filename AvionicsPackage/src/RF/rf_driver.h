#ifndef DATALOGGER_RF_DRIVER_H_
#define DATALOGGER_RF_DRIVER_H_

#include <RH_RF95.h>

#include "../globals.h"

// Defines the hardware pins used by the radio module
#define RFM95_CS 10 //chip select
#define RFM95_RST 9 //reset
#define RFM95_INT 28 //interupt

// Specfies the frequency transmitted from the RFM96W radio
#define RF95_FREQ 434.0

/**
 * RF Module wrapper for use in the
 *  software package.
 *
 * Can be used to initialise, write
 *  to and read from the radio module
 */
class RF_Module
{
    public:
    int writes = 0;
    RF_Module(RH_RF95* libObj);

    /**
     * Initialises the radio module and
     *  sets up any requirements the module
     *  needs to perform it's operations
     *
     * @returns bool
     *      true iff rf module initialised correctly
     */
    bool init(void);

    /**
     * Writes a message to the radio module
     * Blocks until the message has sent
     *
     * @params String message
     *      The message you want to broadcast
     */
    String write(String message);

    /**
     * Reads from the radio module
     *  non-blocking
     *
     * @returns String message
     *      Reads from the module
     */
    String read(void);

    private:

    /**
     * Pointer to the RH_RF95 libary module
     *  used to communicate directly with the
     *  rf chip
     */
    RH_RF95* rf95;
};

#endif
